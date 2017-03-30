/*
   File:    catlog.h
   Project: Catterpillar
   Author:  Douwe Vos
   Date:    May 13, 2009
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "catlogdefs.h"
#include "../catlib.h"
#include "../woo/catstringwo.h"
#include "../natdate.h"

#include <stdlib.h>
#include <stdio.h>

//#undef CAT_LOG_LEVEL


int cat_log_indent_level;


//#undef CAT_LOG_LEVEL
//
//#ifdef CAT_LOG_LEVEL2
//#define CAT_LOG_LEVEL   CAT_LOG_LEVEL2
//#endif

#ifndef CAT_LOG_LEVEL
#define CAT_LOG_LEVEL   CAT_LOG_ALL
#endif

#ifdef G_OS_WIN32
#define PRINT_INT64		"%I64d"
#else
#define PRINT_INT64		"%ld"
#endif


#ifdef CAT_LOG_CLAZZ
#define cat_log_print(level, format, args...) \
{ \
	if (!cat_log_block) { \
		cat_log_block = TRUE; \
		if (cat_log_start_time==0) { \
			cat_log_start_time = cat_date_current_time(); \
		} \
		gint64 diftime = cat_date_current_time()-cat_log_start_time; \
		CatStringWo *e_logline = cat_string_wo_new(); \
		cat_string_wo_format(e_logline, "%ld " level ":" CAT_LOG_CLAZZ "(%d)" , diftime, __LINE__); \
		if (cat_string_wo_length(e_logline)<45) { \
			while(cat_string_wo_length(e_logline)<45) { \
				cat_string_wo_append_chars_len(e_logline, " ", 1); \
			} \
		} else { \
			cat_string_wo_limit(e_logline, 0, 45); \
		} \
		int klmn; \
		for(klmn=-1; klmn<cat_log_indent_level; klmn++) { \
			cat_string_wo_append_chars_len(e_logline, "  ", 2); \
		} \
	/*	puts(level ":" CAT_LOG_CLAZZ "(%d) " format "\n"); */ \
		cat_string_wo_format(e_logline, format "\n", ##args); \
		printf("%s", cat_string_wo_getchars(e_logline)); \
		cat_unref_ptr(e_logline); \
		cat_log_block = FALSE; \
	}\
}\


#else
#define cat_log_print(level, format, args...) \
{ \
	if (cat_log_start_time==0) { \
		cat_log_start_time = cat_date_current_time(); \
	} \
	gint64 diftime = cat_date_current_time()-cat_log_start_time; \
	printf(PRINT_INT64 " " level ":%s(%d) " , diftime, __BASE_FILE__, __LINE__); \
	int klmn; \
	for(klmn=0; klmn<cat_log_indent_level; klmn++) { \
		printf("  "); \
	} \
	printf(format "\n", ##args); \
}\

#endif

#if CAT_LOG_LEVEL<=CAT_LOG_FATAL
#define cat_log_fatal(format, args...) \
	cat_log_print("FATL", format, ##args); \
	cat_stacktrace_print();
#else
#define cat_log_fatal(format, args...)
#endif

#if CAT_LOG_LEVEL<=CAT_LOG_ERROR
#define cat_log_error(format, args...) \
	cat_log_print("ERRO", format, ##args); \
//	cat_stacktrace_print();
#else
#define cat_log_error(format, args...)
#endif


#if CAT_LOG_LEVEL<=CAT_LOG_WARN
#define cat_log_warn(format, args...) \
	cat_log_print("WARN", format, ##args)
#define cat_log_on_warn(code) code
#define cat_log_on_warn_else(code, elsecode) code
#else
#define cat_log_warn(format, args...)
#define cat_log_on_warn(code)
#define cat_log_on_warn_else(code, elsecode) elsecode
#endif

#if CAT_LOG_LEVEL<=CAT_LOG_INFO
#define cat_log_info(format, args...) \
	cat_log_print("INFO", format, ##args)
#define cat_log_on_info(code) code
#else
#define cat_log_info(format, args...)
#define cat_log_on_info(code)
#endif


#if CAT_LOG_LEVEL<=CAT_LOG_DEBUG
#define cat_log_debug(format, args...) \
	cat_log_print("DEBG", format, ##args)
#define cat_log_on_debug(code) code
#else
#define cat_log_debug(format, args...)
#define cat_log_on_debug(code)
#endif

#if CAT_LOG_LEVEL<=CAT_LOG_DETAIL
#define cat_log_detail(format, args...) \
	cat_log_print("DETL", format, ##args)
#define cat_log_on_detail(code) code
#else
#define cat_log_detail(format, args...)
#define cat_log_on_detail(code)
#endif

#if CAT_LOG_LEVEL<=CAT_LOG_VERBOSE
#define cat_log_verbose(format, args...) \
	cat_log_print("TRAC", format, ##args)
#define cat_log_on_verbose(code) code
#else
#define cat_log_verbose(format, args...)
#define cat_log_on_verbose(code)
#endif


#if CAT_LOG_LEVEL<=CAT_LOG_TRACE
#define cat_log_trace(format, args...) \
	cat_log_print("TRAC", format, ##args)
#define cat_log_on_trace(code) code
#else
#define cat_log_trace(format, args...)
#define cat_log_on_trace(code)
#endif


//#endif
