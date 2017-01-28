/*
 * catregexput.c
 *
 *  Created on: Jun 24, 2010
 */

#include "../src/catlib.h"

#include "../src/regexp/catregexp.h"

#include "catregexput.h"

#include "../src/logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#include "../src/logging/catlog.h"

static void _run(CatTestContext *context, CatRegexp *reg_exp, CatStringWo *a_to_match, gboolean should_match) {
	gboolean mresult = cat_regexp_match(reg_exp, a_to_match);
	if (mresult!=should_match) {
		context->sub_fail_count++;
		cat_log_warn("NO MATCH ! pattern=%s  to_match=%s", cat_string_wo_getchars(reg_exp->a_pattern), cat_string_wo_getchars(a_to_match));
	}
	context->sub_total_count++;
	cat_unref_ptr(a_to_match);
}

static void _test_simple(CatTestContext *context) {
	CatStringWo *reg = cat_string_wo_new_with("*\\.txt");
	CatRegexp *reg_exp = cat_regexp_new(reg);
	cat_unref_ptr(reg);
	_run(context, reg_exp, cat_string_wo_new_with(""), FALSE);
	_run(context, reg_exp, cat_string_wo_new_with("txt"), FALSE);
	_run(context, reg_exp, cat_string_wo_new_with(".txt"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("stxt"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("*.txt"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("text.txt.txt"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("txt.text"), FALSE);
	cat_unref(reg_exp);
}

static void _test_repeater(CatTestContext *context) {
	CatStringWo *reg = cat_string_wo_new_with("[ab]*[ab]*");
	CatRegexp *reg_exp = cat_regexp_new(reg);
	cat_unref_ptr(reg);
	_run(context, reg_exp, cat_string_wo_new_with("abababaa"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("ababafbaa"), FALSE);
	_run(context, reg_exp, cat_string_wo_new_with("aaaaa"), TRUE);
	cat_unref(reg_exp);
}

static void _test_repeater2(CatTestContext *context) {
	CatStringWo *reg = cat_string_wo_new_with("[ab]*[cd]*");
	CatRegexp *reg_exp = cat_regexp_new(reg);
	cat_unref_ptr(reg);
	_run(context, reg_exp, cat_string_wo_new_with("abababaa"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("abababaacdddcddccc"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("cdddcaddccc"), FALSE);
	_run(context, reg_exp, cat_string_wo_new_with("aaaaa"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("cd"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("abcd"), TRUE);
	cat_unref(reg_exp);
}

static void _test_repeater_list(CatTestContext *context) {
	CatStringWo *reg = cat_string_wo_new_with("\\(ab\\)*[cd]*");
	CatRegexp *reg_exp = cat_regexp_new(reg);
	cat_unref_ptr(reg);
	_run(context, reg_exp, cat_string_wo_new_with("ababab"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("ababddcddccc"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("abba"), FALSE);
	_run(context, reg_exp, cat_string_wo_new_with("cccd"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("cd"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with(""), TRUE);
	cat_unref(reg_exp);
}

static void _test_repeater_even_length(CatTestContext *context) {
	CatStringWo *reg =cat_string_wo_new_with("\\(\\.\\.\\)*");
	CatRegexp *reg_exp = cat_regexp_new(reg);
	cat_unref_ptr(reg);
	_run(context, reg_exp, cat_string_wo_new_with("asfdje"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("ababddf"), FALSE);
	_run(context, reg_exp, cat_string_wo_new_with("abba"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with("ccc"), FALSE);
	_run(context, reg_exp, cat_string_wo_new_with("cd"), TRUE);
	_run(context, reg_exp, cat_string_wo_new_with(""), TRUE);
	cat_unref(reg_exp);
}


void test_regexp(CatTestContext *context) {
	cat_test_context_start_sub(context, __FILE__);
	_test_simple(context);
	_test_repeater(context);
	_test_repeater2(context);
	_test_repeater_list(context);
	_test_repeater_even_length(context);
	cat_test_context_stop_sub(context);
}
