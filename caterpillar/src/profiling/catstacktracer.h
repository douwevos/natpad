/*
   File:    catstacktracer.h
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

#ifndef CATSTACKTRACER_H_
#define CATSTACKTRACER_H_

#if defined(unix) || defined(__unix__) || defined(__unix)
# define PREDEF_PLATFORM_UNIX
#endif


#ifdef PREDEF_PLATFORM_UNIX

#ifdef CAT_ENABLE_STACK_TRACING

#define _GNU_SOURCE


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef PREDEF_PLATFORM_UNIX
#include <execinfo.h>
#else
#include <windows.h>
#include <dbghelp.h>
#endif
#include <bfd.h>
//include <libiberty.h>
#include <dlfcn.h>
#include <link.h>


typedef struct l_CatStackTracer CatStackTracer;
typedef struct l_CatStackBfd CatStackBfd;


struct l_CatStackBfd {
	struct l_CatStackBfd *next;
	char *of_file_name;
	bfd *abfd;
	asymbol **symbol_table;
	long symbol_count;
};


struct l_CatStackTracer {
	CatStackBfd *first_stack_bfd;
};



typedef struct l_CatStackTraceEntry CatStackTraceEntry;
typedef struct l_CatStackTrace CatStackTrace;


struct l_CatStackTraceEntry {
	void *trace_address;

	char *of_file_name;
	void *of_base_address;

	char *src_file_name;
	char *src_function;
	unsigned long src_line_nr;
};

struct l_CatStackTrace {
	CatStackTraceEntry *entry_array;
	int entry_count;
};



typedef struct l_StackBfdAndEntry StackBfdAndEntry;

struct l_StackBfdAndEntry {
	CatStackBfd *stack_bfd;
	CatStackTraceEntry *trace_entry;
};



CatStackTracer *cat_stack_tracer_get_or_create();


CatStackTrace *cat_stack_tracer_create_trace(CatStackTracer *tracer, int start, int length);

void cat_stack_trace_free(CatStackTrace *trace);

void cat_stack_trace_dump(CatStackTrace *trace);


#endif

#else



void printStack( void );



#endif


#endif /* CATSTACKTRACER_H_ */
