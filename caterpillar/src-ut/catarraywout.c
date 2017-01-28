/*
   File:    catfixedarrayut.c
   Project: Natpad
   Author:  Douwe Vos
   Date:    Aug 19, 2010
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

#include "catarraywout.h"

#include "../src/catlib.h"
#include "../src/woo/catarraywo.h"
#include "../src/woo/catstringwo.h"

#include "../src/logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#include "../src/logging/catlog.h"
//
//
//static void _test_boolean(CatTestContext *context, const char *test, gboolean val_a, gboolean val_b) {
//	if (val_a!=val_b) {
//		context->sub_fail_count++;
//		printf("fail !!\n");
////		cat_log_warn("NO MATCH ! %s", test);
//	}
//	context->sub_total_count++;
//}
//
static CatStringWo **_get_10_strings() {
	CatStringWo **strings = (CatStringWo **) g_malloc(sizeof(CatStringWo *)*10);
	strings[0] = cat_string_wo_anchor(cat_string_wo_new("zero"), 0);
	strings[1] = cat_string_wo_anchor(cat_string_wo_new("one"), 0);
	strings[2] = cat_string_wo_anchor(cat_string_wo_new("two"), 0);
	strings[3] = cat_string_wo_anchor(cat_string_wo_new("three"), 0);
	strings[4] = cat_string_wo_anchor(cat_string_wo_new("four"), 0);
	strings[5] = cat_string_wo_anchor(cat_string_wo_new("five"), 0);
	strings[6] = cat_string_wo_anchor(cat_string_wo_new("six"), 0);
	strings[7] = cat_string_wo_anchor(cat_string_wo_new("seven"), 0);
	strings[8] = cat_string_wo_anchor(cat_string_wo_new("eight"), 0);
	strings[9] = cat_string_wo_anchor(cat_string_wo_new("nine"), 0);
	return strings;
}

static void _unget_10_strings(CatStringWo **strings) {
	cat_unref_ptr(strings[0]);
	cat_unref_ptr(strings[1]);
	cat_unref_ptr(strings[2]);
	cat_unref_ptr(strings[3]);
	cat_unref_ptr(strings[4]);
	cat_unref_ptr(strings[5]);
	cat_unref_ptr(strings[6]);
	cat_unref_ptr(strings[7]);
	cat_unref_ptr(strings[8]);
	cat_unref_ptr(strings[9]);
	g_free(strings);
}
//
//
//static gboolean _ref_count_strings(CatFixedString **strings, int rc[10]) {
//	int idx;
//	for(idx=0; idx<9; idx++) {
//		if (G_OBJECT(strings[idx])->ref_count!=rc[idx]) {
////			cat_log_warn("G_OBJECT(strings[%d])->ref_count=%d  != rc[%d]=%d", idx, G_OBJECT(strings[idx])->ref_count, idx, rc[idx]);
//			return FALSE;
//		}
//	}
//	return TRUE;
//}
//
static void _test_append(CatTestContext *context) {
	CatStringWo **strings = _get_10_strings();
	CatArrayWo *e_array = cat_array_wo_new();
	cat_array_wo_append(e_array, G_OBJECT(strings[2]));
	cat_array_wo_append(e_array, G_OBJECT(strings[2]));
	cat_array_wo_append(e_array, G_OBJECT(strings[4]));
	cat_array_wo_append(e_array, G_OBJECT(strings[6]));
	cat_array_wo_append(e_array, G_OBJECT(strings[2]));
//	cat_ref_sink_ptr(fixed_array);
//	int refs[] = {1,1,4,1,2,1,2,1,1,1};
//	_test_boolean(context, "append-ref-count", _ref_count_strings(strings, refs), TRUE);
//	cat_unref_ptr(fixed_array);
//	int unrefs[] = {1,1,1,1,1,1,1,1,1,1};
//	_test_boolean(context, "append-unref-count", _ref_count_strings(strings, unrefs), TRUE);

	cat_array_wo_remove_range(e_array, 2, 1);

	CatArrayWo *a_array = cat_array_wo_anchor(e_array, 1);

	e_array = cat_array_wo_create_editable(a_array);
	cat_unref_ptr(a_array);
	cat_array_wo_remove_range(e_array, 2, 1);
//	cat_array_wo_insert(e_array, G_OBJECT(strings[3]), 3);
	cat_array_wo_append(e_array, G_OBJECT(strings[3]));
	a_array = cat_array_wo_anchor(e_array, 1);
	cat_unref_ptr(a_array);

	_unget_10_strings(strings);
}
//
//static void _test_append_2(CatTestContext *context) {
//	CatFixedString **strings = _get_10_strings();
//	CatFixedArray *fixed_array = cat_fixed_array_new();
//	fixed_array = cat_fixed_array_append(fixed_array, G_OBJECT(strings[2]));
//	fixed_array = cat_fixed_array_append(fixed_array, G_OBJECT(strings[2]));
//	CatFixedArray *snapshot = cat_ref_sink_ptr(fixed_array);
//	fixed_array = cat_fixed_array_append(fixed_array, G_OBJECT(strings[4]));
//	fixed_array = cat_fixed_array_append(fixed_array, G_OBJECT(strings[6]));
//	fixed_array = cat_fixed_array_append(fixed_array, G_OBJECT(strings[2]));
//	cat_ref_sink_ptr(fixed_array);
//	int refs[] = {1,1,6,1,2,1,2,1,1,1};
//	_test_boolean(context, "append-ref-count", _ref_count_strings(strings, refs), TRUE);
//	cat_unref_ptr(fixed_array);
//
//	int refsnap[] = {1,1,3,1,1,1,1,1,1,1};
//	_test_boolean(context, "append-ref-count-snap", _ref_count_strings(strings, refsnap), TRUE);
//
//	cat_unref_ptr(snapshot);
//
//	int unrefs[] = {1,1,1,1,1,1,1,1,1,1};
//	_test_boolean(context, "append-unref-count", _ref_count_strings(strings, unrefs), TRUE);
//	_unget_10_strings(strings);
//}
//
//
//
void test_array_wo(CatTestContext *context) {
	cat_test_context_start_sub(context, __FILE__);
	_test_append(context);
//	_test_append_2(context);
	cat_test_context_stop_sub(context);
}
