/*
   File:    catbitarraywout.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jun 2, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "catbitarraywout.h"

#include "../src/catistringable.h"
#include "../src/woo/catbitarraywo.h"
#include "../src/woo/catstringwo.h"
#include "../src/catlib.h"

#include "../src/logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#include "../src/logging/catlog.h"


static void print_it(CatStringWo *e_out, CatBitArrayWo *ba) {
	int bit_count = cat_bit_array_wo_length(ba);
	int bit_idx;
	for(bit_idx=0; bit_idx<bit_count; bit_idx++) {
		gboolean bit_is_set = cat_bit_array_wo_get(ba, bit_idx);
		cat_string_wo_append_chars(e_out, bit_is_set ? "1" : "0");
		if ((bit_idx%8)==7) {
			cat_string_wo_append_chars(e_out, " ");
		}
	}
}

static void _test_simple(CatTestContext *context) {
	CatBitArrayWo *ba_a = cat_bit_array_wo_new();
	cat_bit_array_wo_insert(ba_a,0,10);
	cat_bit_array_wo_set(ba_a, 8);
	cat_bit_array_wo_set(ba_a, 4);
	CatStringWo *e_out = cat_string_wo_new();
	print_it(e_out, ba_a);
	cat_log_error("%s", cat_string_wo_getchars(e_out));

	int idx;
	for(idx=1; idx<4; idx++) {
		int bit_count = cat_bit_array_wo_length(ba_a);
		cat_bit_array_wo_insert(ba_a, bit_count, idx);
		CatStringWo *e_out = cat_string_wo_new();
		print_it(e_out, ba_a);
		cat_log_error("%s", cat_string_wo_getchars(e_out));
		cat_bit_array_wo_set(ba_a, bit_count);
		e_out = cat_string_wo_new();
		print_it(e_out, ba_a);
		cat_log_error("%s", cat_string_wo_getchars(e_out));
	}

}


void test_bit_array(CatTestContext *context) {
	cat_test_context_start_sub(context, __FILE__);
	_test_simple(context);
//	_test_format(context);
	cat_test_context_stop_sub(context);
}
