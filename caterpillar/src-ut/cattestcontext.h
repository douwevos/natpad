/*
   File:    cattestcontext.c
   Project: Caterpillar
   Author:  Douwe Vos
   Date:    Aug 19, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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


#ifndef CAT_TEST_CONTEXT_H_
#define CAT_TEST_CONTEXT_H_

#include <glib-object.h>

struct _CatTestContext {
	int sub_total_count;
	int sub_fail_count;
	char *sub_name;
	int total_count;
	int total_fail_count;
};

typedef struct _CatTestContext CatTestContext;


void cat_test_context_start_sub(CatTestContext *context, char *sub_name);
void cat_test_context_stop_sub(CatTestContext *context);


void cat_test_context_test_for_true(CatTestContext *context, gboolean val, const char *msg_if_not);

/* returns 'CatFixedString **'  */
void **cat_test_context_load_names(CatTestContext *context, int *count);

void cat_test_context_free_names(CatTestContext *context, void **names);


#endif
