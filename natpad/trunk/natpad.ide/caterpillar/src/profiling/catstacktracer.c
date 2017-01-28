/*
   File:    catstacktracer.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Apr 2, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "catstacktracer.h"

#ifdef PREDEF_PLATFORM_UNIX

#ifdef CAT_ENABLE_STACK_TRACING



static int has_been_initialized = 0;
static __thread CatStackTracer *tracer = NULL;


CatStackTracer *cat_stack_tracer_get_or_create() {
	if (tracer == NULL) {
		if (has_been_initialized==0) {
			has_been_initialized = -1;
			bfd_init();
		}
		tracer = malloc(sizeof(CatStackTracer));
		tracer->first_stack_bfd = NULL;
	}

	return tracer;
}


static void l_read_minisymbols(CatStackBfd *stack_bfd) {
	char **matching;

	if (bfd_check_format_matches(stack_bfd->abfd, bfd_object, &matching)) {

		if ((bfd_get_file_flags(stack_bfd->abfd) & HAS_SYMS) != 0) {
			asymbol **symbol_table = NULL;
			unsigned int sym_size;

			long symcount = bfd_read_minisymbols(stack_bfd->abfd, 0, (void *) & symbol_table, &sym_size);
			if (symcount == 0) {
				if (symbol_table != NULL) {
					free(symbol_table);
				}
				symcount = bfd_read_minisymbols(stack_bfd->abfd, -1, (void *) & symbol_table, &sym_size);
			}

			if (symcount>0 && symbol_table) {
				stack_bfd->symbol_table = symbol_table;
				stack_bfd->symbol_count = symcount;
			} else if (symbol_table != NULL) {
				free(symbol_table);
			}
		}
	}
}



CatStackBfd *cat_stack_tracer_get_or_create_bfd(CatStackTracer *tracer, const char *of_file_name) {
	CatStackBfd *result = NULL;
	CatStackBfd *find = tracer->first_stack_bfd;
	while(find) {
		if (strcmp(find->of_file_name, of_file_name)==0) {
			result = find;
			break;
		}
		find = find->next;
	}

	if (result==NULL) {
		result = malloc(sizeof(CatStackBfd));
		result->of_file_name = strdup(of_file_name);
		result->next = tracer->first_stack_bfd;
		tracer->first_stack_bfd = result;
		result->symbol_table = NULL;
		result->symbol_count = 0;
		result->abfd = bfd_openr(of_file_name, NULL);
	}

	if (result->abfd && result->symbol_table == NULL) {
		l_read_minisymbols(result);
	}
	return result;
}



static int l_find_matching_file(struct dl_phdr_info *info, size_t size, void *user_data) {
	CatStackTraceEntry *trace_entry = (CatStackTraceEntry *) user_data;
	const ElfW(Phdr) *phdr_entry;
	phdr_entry = info->dlpi_phdr;
	ElfW(Addr) load_base = info->dlpi_addr;
	int idx;
	void *entry_trace_adres = trace_entry->trace_address;
	for(idx=info->dlpi_phnum-1; idx>=0; idx--, phdr_entry++) {
		if (phdr_entry->p_type == PT_LOAD) {
			ElfW(Addr) vaddr = phdr_entry->p_vaddr + load_base;
			if (entry_trace_adres >= (void *) vaddr && entry_trace_adres < (void *) (vaddr + phdr_entry->p_memsz)) {
				/* we found a match */
				if (info->dlpi_name==NULL) {
					trace_entry->of_file_name = NULL;
				} else if (strlen(info->dlpi_name)>0) {
					trace_entry->of_file_name = strdup(info->dlpi_name);
				} else {
					trace_entry->of_file_name = NULL;
				}
				trace_entry->of_base_address = (void *) info->dlpi_addr;
				return -1;
			}
		}
	}
	return 0;
}




static void l_find_address_in_section(bfd *l_bfd, asection *section, void *user_data) {
	StackBfdAndEntry *bfd_and_entry = (StackBfdAndEntry *) user_data;
	CatStackTraceEntry *backtrace_entry = bfd_and_entry->trace_entry;
	if (backtrace_entry->src_file_name!=NULL || backtrace_entry->src_function!=NULL) {
		return;
	}

	if ((bfd_get_section_flags(l_bfd, section) & SEC_ALLOC) == 0) {
		return;
	}

	bfd_vma entry_vma = (bfd_vma) (backtrace_entry->trace_address-backtrace_entry->of_base_address);

	bfd_vma vma = bfd_get_section_vma(l_bfd, section);
	if (entry_vma < vma) {
		return;
	}

	bfd_size_type size = bfd_section_size(l_bfd, section);
	if (entry_vma >= vma + size) {
		return;
	}

	static const char *filename;
	static const char *functionname;
	static unsigned int line;
	if (bfd_find_nearest_line(l_bfd, section, bfd_and_entry->stack_bfd->symbol_table, entry_vma - vma, &filename, &functionname, &line)) {
		backtrace_entry->src_file_name = filename==NULL ? NULL : strdup(filename);
		backtrace_entry->src_function = functionname==NULL ? NULL : strdup(functionname);
		backtrace_entry->src_line_nr = line;
	}
}



CatStackTrace *cat_stack_tracer_create_trace(CatStackTracer *tracer, int start, int length) {
	if (tracer==NULL) {
		tracer = cat_stack_tracer_get_or_create();
	}
	size_t size;
	void **trace = (void **) malloc(80*sizeof(void *));
	size = backtrace(trace, 80);

	if (start==-1) {
		start=1;
	}

	if (length==-1) {
		length = size;
	} else {
		length += start;
		if (length>size) {
			length = size;
		}
	}
	length -= start;


	CatStackTrace *result = malloc(sizeof(CatStackTrace));
	result->entry_array = malloc(sizeof(CatStackTraceEntry)*length);
	result->entry_count = length;

	int idx=0;
	for(idx=0; idx<length; idx++) {
		CatStackTraceEntry *entry = result->entry_array+idx;
		entry->trace_address = trace[idx+start];
		entry->of_file_name = NULL;
		entry->of_base_address = 0;

		entry->src_file_name = NULL;
		entry->src_function = NULL;
		entry->src_line_nr = 0;



		dl_iterate_phdr(l_find_matching_file, entry);

		const char *of_file_name = entry->of_file_name;
		if (of_file_name==NULL) {
			of_file_name = "/proc/self/exe";
		}

		CatStackBfd *stack_bfd = cat_stack_tracer_get_or_create_bfd(tracer, of_file_name);
		if (stack_bfd->symbol_table!=NULL && stack_bfd->symbol_count>0) {
			StackBfdAndEntry bfd_and_entry;
			bfd_and_entry.stack_bfd = stack_bfd;
			bfd_and_entry.trace_entry = entry;
			bfd_map_over_sections(stack_bfd->abfd, l_find_address_in_section, (void *) &bfd_and_entry);
		}
	}

	return result;
}



void cat_stack_trace_free(CatStackTrace *trace) {
	if (trace==NULL) {
		return;
	}
	int idx;
	for(idx=0; idx<trace->entry_count; idx++) {
		CatStackTraceEntry *entry = trace->entry_array+idx;
		if (entry->of_file_name) {
			free(entry->of_file_name);
		}
		if (entry->src_file_name) {
			free(entry->src_file_name);
		}
		if (entry->src_function) {
			free(entry->src_function);
		}
	}
	free(trace->entry_array);
	free(trace);
}


void cat_stack_trace_dump(CatStackTrace *trace) {
	int idx;
	for(idx=0; idx<trace->entry_count; idx++) {
		CatStackTraceEntry *entry = trace->entry_array+idx;

		const char *filename = "null";
		if (entry->src_file_name) {
			filename = entry->src_file_name;
			while(TRUE) {
				const char *n = strchr(filename, '/');
				if (n==NULL) {
					break;
				}
				filename = n+1;
			}
		}

		printf("# %d [0x%016lx] %s(%s:%lu)\n", idx, (long) (entry->trace_address-entry->of_base_address), entry->src_function==NULL ? "null" : entry->src_function, filename, entry->src_line_nr);
	}
}


#endif



#else


#include <windows.h>


typedef USHORT NTAPI RtlCaptureStackBackTrace_Function(
    IN ULONG frames_to_skip,
    IN ULONG frames_to_capture,
    OUT PVOID *backtrace,
    OUT PULONG backtrace_hash);

// Load the function we need at static init time, where we don't have
// to worry about someone else holding the loader's lock.
static RtlCaptureStackBackTrace_Function* RtlCaptureStackBackTrace_fn = NULL;

int GetStackTrace(void** result, int max_depth, int skip_count) {
  if (!RtlCaptureStackBackTrace_fn) {
	  RtlCaptureStackBackTrace_fn =
	     (RtlCaptureStackBackTrace_Function*)
	     GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlCaptureStackBackTrace");
	  if (RtlCaptureStackBackTrace_fn==NULL) {
		  printf("no RtlCaptureStackBackTrace could be loaded");
		  return 0;
	  }
  }
  return (int)RtlCaptureStackBackTrace_fn(skip_count + 2, max_depth,
                                          result, 0);
}

void printStack( void )
{
}



#endif
