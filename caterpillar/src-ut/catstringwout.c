/*
 * catstringut.c
 *
 *  Created on: Dec 11, 2010
 *      Author: Douwe
 */


#include "catstringwout.h"
#include "catstringstatic.h"
#include "../src/catlib.h"

#include "../src/catistringable.h"
#include "../src/woo/catstringwo.h"

#include "../src/woo/cathashmapwo.h"


#include "../src/logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#include "../src/logging/catlog.h"


static void _test_simple(CatTestContext *context) {
	CatStringWo *a_test_static = CAT_S(test_a_string);
	CatStringWo *a_test_local = cat_string_wo_new_with("Test me");
	cat_test_context_test_for_true(context, cat_string_wo_equal(a_test_local, a_test_static), "equal-test");
	cat_unref_ptr(a_test_local);
	a_test_local = cat_string_wo_new_with("Wrong");
	cat_test_context_test_for_true(context, !cat_string_wo_equal(a_test_local, a_test_static), "equal-test-inv");
	cat_unref_ptr(a_test_local);

	const CatStringWo *a_test_const = cat_string_wo_new_data("Test me");
	cat_test_context_test_for_true(context, cat_string_wo_equal(a_test_const, a_test_static), "equal-test");
	cat_unref_ptr(a_test_const);

}

static void _test_format(CatTestContext *context) {
	CatStringWo *a_test_local = cat_string_wo_new_with("Test me");
	CatHashMapWo *dummy_map = cat_hash_map_wo_new(NULL, NULL);
	CatStringWo *e_formated = cat_string_wo_new();
	cat_string_wo_format(e_formated, "test it, context=%o, map=%o, say-1000=%d, pointer=%p", a_test_local, dummy_map, 1000, dummy_map);


	CatStringWo *e_manual = cat_string_wo_new_with("test it, context=");
	cat_istringable_print(CAT_ISTRINGABLE(a_test_local), e_manual);
	cat_string_wo_append_chars(e_manual, ", map=");
	cat_istringable_print(CAT_ISTRINGABLE(dummy_map), e_manual);
	cat_string_wo_append_chars(e_manual, ", say-1000=");
	cat_string_wo_append_decimal(e_manual, 1000);
	cat_string_wo_append_chars(e_manual, ", pointer=");
	cat_string_wo_append_chars_len(e_manual, "0x", 3);
	cat_string_wo_append_hexadecimal(e_manual, (unsigned long long int) dummy_map, -1);

	cat_test_context_test_for_true(context, cat_string_wo_equal(e_manual, e_formated), "formatting");

	cat_unref_ptr(e_manual);
	cat_unref_ptr(e_formated);
	cat_unref_ptr(a_test_local);
	cat_unref_ptr(dummy_map);
}


void test_string(CatTestContext *context) {
	cat_test_context_start_sub(context, __FILE__);
	_test_simple(context);
	_test_format(context);
	cat_test_context_stop_sub(context);
}
