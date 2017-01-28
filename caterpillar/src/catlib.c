/*
   Project: Natpad
   Author:  Douwe Vos
   Date:    17-7-2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2008 Douwe Vos.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */



// Test which c files lack 'cat_ref_anounce'
//   for I in `find astarte/ | grep \\\\.c$` ; do  if [ -z "`egrep  cat_ref_anounce $I`" ] ; then echo $I ; fi ;  done
//
// Enlist all TO DOs in c and h files
//   for I in `find . | grep \\\\.[ch]$ | grep -v build` ; do grep -nH TODO $I ; done

#include "catlib.h"
#include <stdio.h>
#include <stdlib.h>
//#include <glib-2.0/glib.h>
#include <glib-object.h>

#include "natdate.h"
#include "woo/catstringwo.h"
#include "profiling/catstacktracer.h"
#include "profiling/catimonitored.h"

#ifdef G_OS_UNIX
#include <execinfo.h>

#endif

gint64 cat_log_start_time = 0;

__thread gboolean cat_log_block = FALSE;

gint64 cat_time_running_in_ms() {
	return cat_date_current_time()-cat_log_start_time;
}

#ifndef CAT_REF_MONITORING

void cat_ref_report() {}
void cat_ref_summary() {}
void cat_ref_cleanup() {}



void *cat_ref_intern(void *obj) {
	if (obj) {
		g_object_ref(obj);
	}
	return obj;
}

void *cat_fake_ref_intern(void *obj) {
	return obj;
}


void cat_unref_intern(void *obj) {
	if (obj) {
		g_object_unref(obj);
	}
}

void *cat_ref_sink_intern(void *obj) {
	if (obj) {
		if (G_IS_INITIALLY_UNOWNED(obj)) {
			g_object_ref_sink(obj);
		} else if (G_IS_OBJECT(obj)) {
			g_object_ref(obj);
		}
	}
	return obj;
}


void cat_ref_sink_swap_intern(void **field, void *obj) {
	if (*field!=obj) {
		gpointer old = *field;
		if (obj) {
			g_object_ref_sink(obj);
		}
		*field = obj;
		if (old) {
			g_object_unref(old);
		}
	}
}

void cat_ref_swap_intern(void **field, void *obj) {
	if (*field!=obj) {
		gpointer old = *field;
		if (obj) {
			g_object_ref(obj);
		}
		*field = obj;
		if (old) {
			g_object_unref(old);
		}
	}
}

void cat_ref_anounce_intern(void *obj) {
}
void cat_ref_denounce_intern(void *obj) {}
void cat_ref_ignore_intern(void *obj) {}


#else

/* CAT_REF_MONITORING */

#include <string.h>


typedef struct _CatRefEntry CatRefEntry;

struct _CatRefEntry {
	GType type;
	int create_count;
	int destroy_count;
	int ignore_count;
	unsigned int size_of_instance;
};

CatRefEntry *cat_ref_entry_array = NULL;
int cat_ref_entry_count = 0;
static GMutex * volatile cat_ref_entry_mutex = NULL;
static GCond * volatile cat_ref_entry_condition;

enum _CatCounterType {
	CAT_CT_CREATE,
	CAT_CT_DESTROY,
	CAT_CT_IGNORE
};

static CatRefEntry *l_get_or_create_ref_entry(GType type, unsigned int size_of_instance, enum _CatCounterType cct) {


	if (cat_ref_entry_mutex==NULL) {
		cat_ref_entry_mutex = g_malloc(sizeof(GMutex));
		g_mutex_init(cat_ref_entry_mutex);

		cat_ref_entry_condition = g_malloc(sizeof(GCond));
		g_cond_init(cat_ref_entry_condition);
	}
	g_mutex_lock(cat_ref_entry_mutex);


	CatRefEntry *result = NULL;
	if (cat_ref_entry_array==NULL) {
		cat_ref_entry_array = g_malloc(sizeof(CatRefEntry)*1);
		cat_ref_entry_count = 1;
		result = cat_ref_entry_array;
		result->type = type;
		result->create_count = 0;
		result->destroy_count = 0;
		result->ignore_count = 0;
		result->size_of_instance = size_of_instance;
	} else {
		int idx;
		for(idx=0; idx<cat_ref_entry_count; idx++) {
			CatRefEntry *test_entry = cat_ref_entry_array+idx;
			if (test_entry->type == type) {
				result = test_entry;
				break;
			}
		}

		if (result==NULL) {
			cat_ref_entry_count++;
			cat_ref_entry_array = g_realloc(cat_ref_entry_array, sizeof(CatRefEntry)*cat_ref_entry_count);
			result = cat_ref_entry_array+cat_ref_entry_count-1;
			result->type = type;
			result->create_count = 0;
			result->destroy_count = 0;
			result->ignore_count = 0;
			result->size_of_instance = size_of_instance;
		}
	}

	switch(cct) {
		case CAT_CT_CREATE : { result->create_count++; } break;
		case CAT_CT_DESTROY : { result->destroy_count++; } break;
		case CAT_CT_IGNORE : { result->ignore_count++; } break;
	}
	g_mutex_unlock(cat_ref_entry_mutex);
	return result;
}


static int cat_ref_entry_sort_cmp(const void *ptr_a, const void *ptr_b) {
	const CatRefEntry *entry_a = (const CatRefEntry *) ptr_a;
	const CatRefEntry *entry_b = (const CatRefEntry *) ptr_b;
	const char *name_a = g_type_name((entry_a)->type);
	const char *name_b = g_type_name((entry_b)->type);
	return strcmp(name_a, name_b);
}

void cat_ref_entry_report() {
	printf("entry-report\n");
	int idx;

	qsort(cat_ref_entry_array, cat_ref_entry_count, sizeof(CatRefEntry), cat_ref_entry_sort_cmp);

	unsigned long long total_lost = 0;
	unsigned long long total_all = 0;

	for(idx=0; idx<cat_ref_entry_count; idx++) {
		CatRefEntry *test_entry = cat_ref_entry_array+idx;

		if (test_entry->create_count != test_entry->destroy_count+test_entry->ignore_count) {
			unsigned long long lost = test_entry->create_count - (test_entry->destroy_count + test_entry->ignore_count);
			lost = lost*test_entry->size_of_instance;
			printf("\033[1;31m%d/%d/%d :: %s [%lld]\033[0m\n", test_entry->create_count, test_entry->destroy_count, test_entry->ignore_count, g_type_name(test_entry->type), lost);
			if (strcmp(g_type_name(test_entry->type), "CatFixedStringStatic")!=0) {
				total_lost += lost;
			}
		} else {
			printf("%d/%d/%d :: %s\n", test_entry->create_count, test_entry->destroy_count, test_entry->ignore_count, g_type_name(test_entry->type));
		}
		total_all += test_entry->create_count;
	}
		printf("----------------------------------------------------\n");
		int per = ((total_lost*10000)/total_all);
		if (total_lost==0) {
			printf("\033[1;32mTotal created: %lld\033[0m\n", total_all);
		} else {
			printf("\033[1;31mTotal lost: %lld (%d.%02d) - created: %lld\033[0m\n", total_lost, per/100, per%100, total_all);
		}
}


#ifdef MONITOR_CLAZZ


int _ref_cnt(void *obj) {
	if (obj) {
		if (G_IS_OBJECT(obj)) {
			return G_OBJECT(obj)->ref_count;
		} else {
			return -1000;
		}
	}
	return -1;
}


static GList *monitor_list = NULL;

static GMutex * volatile mutex = NULL;
static GCond * volatile condition;

static GType mon_type = 0;
static GType mon_type2 = 0;

static __thread gboolean skip = FALSE;

static gboolean skip_type_monitor = FALSE;



struct _MonitorInfo {
	void *obj;
	const char *clazz_or_filename;	/* of occurrence */
	const char *type_name;
	int line_nr;
	const char *description;
	int ref_count;
	int delta_ref;
//    char **trace;
//    int trace_cnt;
#ifdef CAT_ENABLE_STACK_TRACING
    CatStackTrace *trace;
#endif
    gboolean sink;
    int64_t time_of_occurrence;
    void *thread_pointer;
};

typedef struct _MonitorInfo MonitorInfo;




static int cat_ref_summary_calc_delta(GPtrArray *collected) {
	int idx = 0;
	int anounce_cnt = 0;
	int denounce_cnt = 0;
	gpointer obj = NULL;
	const char *occ = NULL;
	const char *typname = NULL;
	while(idx<collected->len) {
		MonitorInfo *main_info = (MonitorInfo *) collected->pdata[idx++];

		if (obj==NULL) {
			obj = main_info->obj;
			occ = main_info->clazz_or_filename;
			typname = main_info->type_name;
		}

		if (strcmp(main_info->description, "anounc")==0) {
			anounce_cnt++;
		}

		if (strcmp(main_info->description, "denoun")==0) {
			denounce_cnt++;
		}
	}
	printf("obj:%p anounce_cnt:%d denounce_cnt:%d  %s %s\n", obj, anounce_cnt, denounce_cnt, occ, typname);
	return anounce_cnt-denounce_cnt;
}

void cat_ref_summary() {
	GList *startsCollected = NULL;
	GList *work_list = g_list_copy(monitor_list);
	GPtrArray *collected = g_ptr_array_new();
	GList *iter;
	gboolean keep_running = TRUE;
	while(keep_running) {
		iter = work_list;
		if (iter) {
			g_ptr_array_set_size(collected, 0);
			MonitorInfo *main_info = (MonitorInfo *) iter->data;
			g_ptr_array_add(collected, main_info);
			work_list = g_list_remove_link(work_list, iter);
			gboolean ignore_info = FALSE;
			if (main_info->ref_count!=0) {
				iter = work_list;
				while(iter) {
					MonitorInfo *iter_info = (MonitorInfo *) iter->data;
					if (iter_info->obj == main_info->obj) {
						if (strcmp(iter_info->description, "anounc")==0) {
							iter = NULL;
						} else {
							GList *iter_2_del = iter;
							iter = iter->next;
							work_list = g_list_remove_link(work_list, iter_2_del);
							g_ptr_array_add(collected, iter_info);
							if (strcmp(iter_info->description, "denoun")==0) {
								iter = NULL;
							} else if (strcmp(iter_info->description, "ignore")==0) {
								ignore_info = TRUE;
							}
						}
					} else {
						iter = iter->next;
					}
				}
			}

			main_info->delta_ref = cat_ref_summary_calc_delta(collected);
			if (main_info->delta_ref!=0 && !ignore_info) {
//				printf("ref-summary: %s dref:%d\n", main_info->clazz_or_filename, main_info->delta_ref);
				startsCollected = g_list_append(startsCollected, main_info);
			}


		} else {
			keep_running = FALSE;
		}
	}


	keep_running = TRUE;
	while(keep_running) {
		iter = startsCollected;
		if (iter) {
			MonitorInfo *main_info = (MonitorInfo *) iter->data;
			if (main_info==NULL) {
				startsCollected = g_list_remove_link(startsCollected, iter);
				continue;
			}
			startsCollected = g_list_remove_link(startsCollected, iter);
			iter = startsCollected;
			int sum = 1;
			while(iter) {
				MonitorInfo *iter_info = (MonitorInfo *) iter->data;
				if (iter_info==NULL) {
					iter = iter->next;
					continue;
				}

				if (iter_info->clazz_or_filename == main_info->clazz_or_filename) {
					GList *iter_2_del = iter;
					iter = iter->next;
					startsCollected = g_list_remove_link(startsCollected, iter_2_del);
					sum++;
				} else {
					iter = iter->next;
				}
			}

			if (main_info && main_info->clazz_or_filename) {
				printf("ref-summary: %s nr-of-unbalanced:%d\n", main_info->clazz_or_filename, sum);
			} else {
				printf("ref-summary: ??? nr-of-unbalanced:%d\n", sum);
			}
		} else {
			keep_running = FALSE;
		}
	}
}



static void cat_ref_report_entry(GPtrArray *collected, gboolean do_print) {
	int idx = 0;
	int pseudo_ref_count = 0;

	while(idx<collected->len) {
		MonitorInfo *main_info = (MonitorInfo *) collected->pdata[idx];
		if (idx==0) {
			if (do_print) {
				printf("monitor-report for %p \n", main_info->obj);
				CatStringWo *s = cat_string_wo_new();
				cat_string_wo_format(s, "%o", main_info->obj);
				printf("%s\n", cat_string_wo_getchars(s));
				cat_unref_ptr(s);
			}
			pseudo_ref_count = 1;
		} else {
			pseudo_ref_count += main_info->delta_ref;
		}
		if (do_print) {
			printf("[%d] :: tou=%ld ref-count=%d psrefcnt=%d  %s >> %s - %s[%d] %p\n", idx, main_info->time_of_occurrence-cat_log_start_time, main_info->ref_count, pseudo_ref_count, main_info->sink ? "sink" : "plain", main_info->description, main_info->clazz_or_filename, main_info->line_nr, main_info->thread_pointer);
#ifdef CAT_ENABLE_STACK_TRACING
			cat_stack_trace_dump(main_info->trace);
#endif
			printf("\n");
		}

#ifdef CAT_ENABLE_STACK_TRACING
		if (main_info->trace) {
			cat_stack_trace_free(main_info->trace);
			main_info->trace = NULL;
		}
#endif
		g_free(main_info);
		idx++;
	}
	if (idx>0 && do_print) {
		printf("----------------------------------------------------\n");
	}

}

void cat_ref_report_impl(void *obj2report) {
	GPtrArray *collected = g_ptr_array_new();
	GList *iter;
	gboolean keep_running = TRUE;
	gboolean clean_obj;
	gboolean ignore_obj;
	while(keep_running) {
		iter = monitor_list;
		clean_obj = FALSE;
		ignore_obj = FALSE;
		if (iter) {
			g_ptr_array_set_size(collected, 0);
			MonitorInfo *main_info = (MonitorInfo *) iter->data;

			gboolean with_announcement = FALSE;
			if (strcmp(main_info->description, "anounc")==0) {
				with_announcement = TRUE;
			}



			g_ptr_array_add(collected, main_info);
			monitor_list = g_list_remove_link(monitor_list, iter);
			if (main_info->ref_count!=0) {
				iter = monitor_list;
				while(iter) {
					MonitorInfo *iter_info = (MonitorInfo *) iter->data;
					if (iter_info->obj == main_info->obj) {
						GList *iter_2_del = iter;
						iter = iter->next;
						if (with_announcement) {
							if (strcmp(iter_info->description, "anounc")==0) {
								break;
							}
						}
						if (strcmp(iter_info->description, "ignore")==0) {
							ignore_obj = TRUE;
						}

						monitor_list = g_list_remove_link(monitor_list, iter_2_del);
						g_ptr_array_add(collected, iter_info);
						if (with_announcement) {
							if (strcmp(iter_info->description, "finlzd")==0) {
								clean_obj = TRUE;
								iter = NULL;
							}
						} else {
							if (iter_info->ref_count==0) {
								clean_obj = TRUE;
								iter = NULL;
							}
						}
					} else {
						iter = iter->next;
					}
				}
			} else {
				clean_obj = TRUE;
			}


			if (!ignore_obj) {
				if (obj2report) {
					if (obj2report==main_info->obj) {
						cat_ref_report_entry(collected, TRUE);
					}
				} else {
//			clean_obj = FALSE;
#ifdef MONITOR_CLAZZ_INVERT
					clean_obj = !clean_obj;
#endif
					cat_ref_report_entry(collected, !clean_obj);
				}
			}

		} else {
			keep_running = FALSE;
		}
	}
}


void cat_ref_report() {
	skip = TRUE;
	skip_type_monitor = TRUE;
	printf("monitor-report\n");
	cat_ref_report_impl(NULL);
	skip = FALSE;
}


void cat_ref_cleanup() {
	GList *iter = monitor_list;
	while(iter) {
		MonitorInfo *iter_info = (MonitorInfo *) iter->data;
#ifdef CAT_ENABLE_STACK_TRACING
		if (iter_info->trace) {
			cat_stack_trace_free(iter_info->trace);
			iter_info->trace = NULL;
		}
#endif

		free(iter_info);
		monitor_list = g_list_remove_link(monitor_list, iter);
		iter = monitor_list;
	}
}



void _monitor(const char *stxt, void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz, int ref_count_delta, gboolean with_sink) {
#ifdef MONITOR_CLAZZ
	if (obj==NULL) {
		return;
	}

	if (g_object_get_data(obj, "cat-ignore")!=NULL) {
		return;
	}

//	printf("in monitor\n");


	GTypeInstance *objinst = (GTypeInstance *) obj;
	GType type = objinst->g_class->g_type;

	if (mon_type==0) {
		mon_type = g_type_from_name(MONITOR_CLAZZ);
		skip = FALSE;
	}

//	if (mon_type2==0) {
//		mon_type2 = g_type_from_name("MooContentMap");
//		skip = FALSE;
//	}


	const char *type_name = g_type_name_from_instance((GTypeInstance *) obj);
	if (skip) {
//		printf("%s:%d\n", clazz_or_filename, line_nr);
		return;
	}

	skip = TRUE;




//	if (CAT_IS_IMONITORED(obj)) {
//	if (mon_type!=0 && g_type_is_a(type, mon_type)) {
	if ((mon_type!=0 && type==mon_type) || (mon_type2!=0 && type==mon_type2)) {

		if (mutex==NULL) {
			mutex = g_malloc(sizeof(GMutex));
			g_mutex_init(mutex);
			condition = g_malloc(sizeof(GCond));
			g_cond_init(condition);
		}
		g_mutex_lock(mutex);

//			printf("check i :: %d %d\n ", (int) mon_type , g_type_check_instance_is_a((GTypeInstance *) obj, mon_type));
//			printf("%s >> %s[%d] %s(%p, ref-count=%d)\n", stxt, clazz_or_filename, line_nr, MONITOR_CLAZZ, obj, _ref_cnt(obj));
		MonitorInfo *mon_info = g_new(MonitorInfo, 1);
		mon_info->thread_pointer = g_thread_self();
		mon_info->time_of_occurrence = cat_date_current_time();
		mon_info->obj = obj;
		mon_info->clazz_or_filename = clazz_or_filename==NULL ? type_name : clazz_or_filename;
		mon_info->type_name = type_name;
		mon_info->line_nr = line_nr;
		mon_info->description =stxt ;
		GObject *object = G_OBJECT(obj);
		int ref_count = object->ref_count + ref_count_delta;
		mon_info->ref_count = ref_count;
		mon_info->delta_ref = ref_count_delta;
		mon_info->sink = with_sink;

		if (with_sink && G_IS_INITIALLY_UNOWNED(object)) {
			if (g_object_is_floating(object)) {
				mon_info->ref_count--;
			}
		}

		monitor_list = g_list_append(monitor_list, mon_info);

#ifdef CAT_ENABLE_STACK_TRACING
		mon_info->trace = cat_stack_tracer_create_trace(NULL, -1, -1);
#endif
		g_mutex_unlock(mutex);
	}

	int rc = _ref_cnt(obj);
	if ((rc%5000)==1000) {
		printf("%s >> %s[%d] %s(%p, ref-count=%d) TO HIGH!\n", stxt, clazz_or_filename, line_nr, type_name, obj, _ref_cnt(obj));

	}
	skip = FALSE;

//	printf("not in monitor\n");
#endif
}

#endif


static void l_week_notify(gpointer data, GObject *ptr_of_obj) {
	GType tp = (GType) data;
	if (!skip_type_monitor) {
		l_get_or_create_ref_entry(tp, sizeof(ptr_of_obj), CAT_CT_DESTROY);
	}


	if (ptr_of_obj) {
		_monitor("finlzd", ptr_of_obj, "<SYSTEM>", -1, TRUE, -1000, FALSE);
	}

}

void cat_ref_anounce_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz) {
	if (obj) {
		_monitor("anounc", obj, clazz_or_filename, line_nr, is_clazz, 0, FALSE);
		GObject *gobj = G_OBJECT(obj);

		if (!skip_type_monitor) {
			l_get_or_create_ref_entry(gobj->g_type_instance.g_class->g_type, sizeof(*obj), CAT_CT_CREATE);
			g_object_weak_ref(gobj, (GWeakNotify) l_week_notify, (void *) (gobj->g_type_instance.g_class->g_type));
		}
	}
}

void cat_ref_denounce_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz) {
	if (obj) {
		_monitor("denoun", obj, clazz_or_filename, line_nr, is_clazz, 0, FALSE);
	}
}

void cat_ref_ignore_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz) {
	if (obj) {

		_monitor("ignore", obj, clazz_or_filename, line_nr, is_clazz, 0, FALSE);

		g_object_set_data(obj, "cat-ignore", "cat-ignore");
		GObject *gobj = G_OBJECT(obj);
		l_get_or_create_ref_entry(gobj->g_type_instance.g_class->g_type, sizeof(*obj), CAT_CT_IGNORE);
		g_object_weak_unref(gobj, (GWeakNotify) l_week_notify, (void *) (gobj->g_type_instance.g_class->g_type));
	}
}


void *cat_ref_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz) {
	if (obj) {
		if (G_IS_OBJECT(obj)) {
			_monitor("incref", obj, clazz_or_filename, line_nr, is_clazz, 1, FALSE);
			g_object_ref(obj);
		} else {
			printf("can not ref, not an object obj=%p\n", obj);
			cat_stacktrace_print();
		}
	}
	return obj;
}

void *cat_fake_ref_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz) {
	if (obj) {
		if (G_IS_OBJECT(obj)) {
			_monitor("fakeref", obj, clazz_or_filename, line_nr, is_clazz, 1, FALSE);
		}
	}
	return obj;
}


void *cat_ref_sink_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz) {
	if (obj) {
		if (G_IS_INITIALLY_UNOWNED(obj)) {
			_monitor("snkref", obj, clazz_or_filename, line_nr, is_clazz, 1, TRUE);
			g_object_ref_sink(obj);
		} else if (G_IS_OBJECT(obj)) {
			_monitor("snkref", obj, clazz_or_filename, line_nr, is_clazz, 1, TRUE);
			g_object_ref(obj);
//			if object is not of type unowned then ref_sink will just increase the reference (it is not considered as an error)
//			printf("WARN : %s[%d] Object can not float object %p : %s\n", clazz_or_filename, line_nr, obj, g_type_name_from_instance((GTypeInstance *) obj));
//			cat_stacktrace_print();
		} else {
			printf("can not ref, not an object obj=%p\n", obj);
			cat_stacktrace_print();
		}
	}
	return obj;
}


void cat_ref_sink_swap_intern(void **field, void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz) {
	if (*field!=obj) {

		gpointer old = *field;
		_monitor("swsold", old, clazz_or_filename, line_nr, is_clazz, -1, FALSE);
		_monitor("swsnew", obj, clazz_or_filename, line_nr, is_clazz, 1, TRUE);
		if (obj) {
			g_object_ref_sink(obj);
		}
		*field = obj;
		if (old) {
			g_object_unref(old);
		}
	} else if (obj) {
		if (g_object_is_floating(obj)) {
			printf("WARN: %s[%d] object:%p object is already stored in field but is still floating!\n", clazz_or_filename, line_nr, obj);
			cat_stacktrace_print();
		}
	}
}

void cat_ref_swap_intern(void **field, void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz) {
	if (*field!=obj) {

		gpointer old = *field;
		_monitor("swpold", old, clazz_or_filename, line_nr, is_clazz, -1, FALSE);
		_monitor("swpnew", obj, clazz_or_filename, line_nr, is_clazz, 1, FALSE);
		if (obj) {
			g_object_ref(obj);
		}
		*field = obj;
		if (old) {
			g_object_unref(old);
		}
	}
}


void cat_unref_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz) {
	void *in_obj = obj;
	if (obj) {
//		printf("in unref_intern obj =%p\n", obj);
		if (G_IS_OBJECT(in_obj)) {
			_monitor("unref ", obj, clazz_or_filename, line_nr, is_clazz, -1, FALSE);
//			printf("Should be an GOBJECT %p %d\n", in_obj, G_OBJECT(in_obj)->ref_count);
			g_object_unref(in_obj);
//			printf("did unref GOBJECT %p\n", in_obj);
		} else {
			printf("WARN: %s[%d] object:%p can not unref, not an object\n", clazz_or_filename, line_nr, obj);
			cat_ref_report_impl(obj);
			cat_stacktrace_print();

			printf("WARN: instance-name=%s", g_type_name_from_instance(obj));

			exit(0);
		}
//		printf("out unref_intern\n");
	}
}


#endif



void cat_stacktrace_print() {
#ifdef G_OS_UNIX
#ifdef CAT_ENABLE_STACK_TRACING
	CatStackTrace *trace = cat_stack_tracer_create_trace(NULL, 2, -1);
	cat_stack_trace_dump(trace);
	cat_stack_trace_free(trace);
#endif
#endif
}




void cat_dump_hex(const char *data, int length) {
	int bytes_per_row = 32;
	int offset;
	char as_text[200];
	as_text[bytes_per_row] = 0;
	for(offset=0; offset<length;) {
		printf("%x:", offset);
		int col;
		for(col=0; col<bytes_per_row; col++) {
			if (offset<length) {
				char ch = data[offset];
				printf("%02x ", (unsigned char) ch);
				as_text[col] = '.';
				if (ch>=32 && ch<128) {
					as_text[col] = ch;
				}
			} else {
				printf("   ");
				as_text[col] = ' ';
			}
			offset++;
		}

		printf("  '%s'\n", as_text);
	}

}

