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

//#ifndef CATLIB_H_
//#define CATLIB_H_


#include <glib-object.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef G_OS_WIN32
#include <windows.h>
#endif


extern __thread gboolean cat_log_block;

extern gint64 cat_log_start_time;

#ifdef __GNUC__
#define VARIABLE_IS_NOT_USED __attribute__ ((unused))
#else
#define VARIABLE_IS_NOT_USED
#endif



gint64 cat_time_running_in_ms();

#ifndef CAT_REF_MONITORING


void cat_ref_report();
void cat_ref_summary();
void cat_ref_cleanup();

void *cat_ref_intern(void *obj);
void *cat_fake_ref_intern(void *obj);
void cat_unref_intern(void *obj);
void *cat_ref_sink_intern(void *obj);
void cat_ref_sink_swap_intern(void **field, void *obj);
void cat_ref_swap_intern(void **field, void *obj);

void cat_ref_anounce_intern(void *obj);
void cat_ref_denounce_intern(void *obj);
void cat_ref_ignore_intern(void *obj);


#define cat_free_ptr(obj) \
	if (obj) { \
		g_free((gpointer) obj); \
		obj = NULL; \
	}


#undef cat_unref
#undef cat_unref_ptr
#undef cat_fake_ref_ptr
#undef cat_ref
#undef cat_ref_ptr
#undef cat_ref_sink_swap
#undef cat_ref_swap
#undef cat_ref_anounce
#undef cat_ref_denounce
#undef cat_ref_sink_ptr

#define cat_unref(obj) cat_unref_intern((void *) obj)
#define cat_unref_ptr(obj) { const void *l_obj = obj; obj=NULL; cat_unref_intern((void *) l_obj); }

#define cat_fake_ref_ptr(obj) cat_fake_ref_intern(obj)
#define cat_ref(obj) cat_ref_intern((void *) obj)
#define cat_ref_ptr(obj) cat_ref_intern((void *) obj)
#define cat_ref_sink_swap(field, obj) cat_ref_sink_swap_intern((void **) &(field), (void *) obj)
#define cat_ref_swap(field, obj) cat_ref_swap_intern((void **) &(field), (void *) obj)
#define cat_ref_anounce(obj) cat_ref_anounce_intern(obj)
#define cat_ref_denounce(obj) cat_ref_denounce_intern(obj)
#define cat_ref_ignore(obj) cat_ref_ignore_intern(obj)
#define cat_ref_sink_ptr(obj) cat_ref_sink_intern((void *) obj)

#else /* CAT_REF_MONITORING */

#include <stdio.h>

void cat_ref_entry_report();


void cat_ref_report();
void cat_ref_summary();
void cat_ref_cleanup();

void _monitor(const char *stxt, void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz, int ref_count_delta, gboolean with_sink);
int _ref_cnt(void *obj);


void *cat_ref_sink_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz);
void *cat_ref_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz);
void *cat_fake_ref_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz);
void cat_unref_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz);
void cat_ref_sink_swap_intern(void **field, void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz);
void cat_ref_swap_intern(void **field, void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz);

void cat_ref_anounce_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz);
void cat_ref_denounce_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz);
void cat_ref_ignore_intern(void *obj, const char *clazz_or_filename, int line_nr, gboolean is_clazz);

#define cat_free_ptr(obj) \
	if (obj) { \
		g_free(obj); \
		obj = NULL; \
	}





#undef cat_unref
#undef cat_unref_ptr
#undef cat_fake_ref_ptr
#undef cat_ref
#undef cat_ref_ptr
#undef cat_ref_sink_swap
#undef cat_ref_swap
#undef cat_ref_anounce
#undef cat_ref_denounce
#undef cat_ref_ignore
#undef cat_ref_sink_ptr



#ifdef CAT_LOG_CLAZZ

	#define cat_unref(obj) cat_unref_intern((void *) obj, CAT_LOG_CLAZZ, __LINE__, TRUE)
	#define cat_unref_ptr(obj) { const void *l_obj = obj; obj=NULL; cat_unref_intern((void *) l_obj, CAT_LOG_CLAZZ, __LINE__, TRUE); }
	#define cat_fake_ref_ptr(obj) cat_fake_ref_intern(obj, CAT_LOG_CLAZZ, __LINE__, TRUE)
	#define cat_ref(obj) cat_ref_intern((void *) obj, CAT_LOG_CLAZZ, __LINE__, TRUE)
	#define cat_ref_ptr(obj) cat_ref_intern((void *) obj, CAT_LOG_CLAZZ, __LINE__, TRUE)
	#define cat_ref_sink_swap(field, obj) cat_ref_sink_swap_intern((void **) &(field), (void *) obj, CAT_LOG_CLAZZ, __LINE__, TRUE)
	#define cat_ref_swap(field, obj) cat_ref_swap_intern((void **) &(field), (void *) obj, CAT_LOG_CLAZZ, __LINE__, TRUE)
	#define cat_ref_anounce(obj) cat_ref_anounce_intern(obj, CAT_LOG_CLAZZ, __LINE__, TRUE)
	#define cat_ref_denounce(obj) cat_ref_denounce_intern(obj, CAT_LOG_CLAZZ, __LINE__, TRUE)
	#define cat_ref_ignore(obj) cat_ref_ignore_intern(obj, CAT_LOG_CLAZZ, __LINE__, TRUE)
	#define cat_ref_sink_ptr(obj) cat_ref_sink_intern(obj, CAT_LOG_CLAZZ, __LINE__, FALSE)

#else

	#define cat_unref(obj) cat_unref_intern((void *) obj, __BASE_FILE__, __LINE__, FALSE)
	#define cat_unref_ptr(obj) cat_unref_intern((void *) obj, __BASE_FILE__, __LINE__, FALSE); obj=NULL;
	#define cat_fake_ref_ptr(obj) cat_fake_ref_intern(obj, __BASE_FILE__, __LINE__, FALSE)
	#define cat_ref(obj) cat_ref_intern((void *) obj, __BASE_FILE__, __LINE__, FALSE)
	#define cat_ref_ptr(obj) cat_ref_intern((void *) obj, __BASE_FILE__, __LINE__, FALSE)
	#define cat_ref_sink_swap(field, obj) cat_ref_sink_swap_intern((void **) &(field), (void *) obj, __BASE_FILE__, __LINE__, FALSE)
	#define cat_ref_swap(field, obj) cat_ref_swap_intern((void **) &(field), (void *) obj, __BASE_FILE__, __LINE__, FALSE)
	#define cat_ref_anounce(obj) cat_ref_anounce_intern(obj, __BASE_FILE__, __LINE__, FALSE)
	#define cat_ref_denounce(obj) cat_ref_denounce_intern(obj, __BASE_FILE__, __LINE__, FALSE)
	#define cat_ref_ignore(obj) cat_ref_ignore_intern(obj, __BASE_FILE__, __LINE__, FALSE)
	#define cat_ref_sink_ptr(obj) cat_ref_sink_intern(obj, __BASE_FILE__, __LINE__, FALSE)

#endif


#endif /* CAT_REF_MONITORING */



void cat_stacktrace_print();


void cat_dump_hex(const char *data, int length);
//#endif /* CATLIB_H_ */
