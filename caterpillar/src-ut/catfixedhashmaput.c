///*
//   File:    catfixedhashmaput.c
//   Project: caterpillar
//   Author:  Douwe Vos
//   Date:    Mar 30, 2012
//   Web:     http://www.natpad.net/
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2012 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#include "cathashmaput.h"
//#include "../src/catfixedhashmap.h"
//#include "../src/catstring.h"
//#include "../src/catlib.h"
//#include <math.h>
//
//
///* next two lines were copied from "../src/catfixedhashmap.c" */
//#define LOAD_FACTOR           0.75
//#define LOAD_FACTOR_INCREASE  61
//
//
//static void _test_map_size(CatTestContext *context, CatFixedHashMap *map, int expected_size) {
//	if (cat_fixed_hash_map_count(map)!=expected_size) {
//		context->sub_fail_count++;
//		printf("fail !! map contains %d elements expected %d elements\n", cat_fixed_hash_map_count(map), expected_size);
//	}
//	context->sub_total_count++;
//}
//
//static void _test_map_bucket_count(CatTestContext *context, CatFixedHashMap *map, int expected_bucket_count) {
//	if (map->bucket_count!=expected_bucket_count) {
//		context->sub_fail_count++;
//		printf("fail !! map contains %d buckets expected %d buckets\n", map->bucket_count, expected_bucket_count);
//	}
//	context->sub_total_count++;
//}
//
//static void _test_map_value(CatTestContext *context, CatFixedHashMap *map, const char *func_name, CatFixedString *key, CatFixedString *resolved_value, GObject *resolved_as_obj, CatFixedString *expected_value) {
//	if (resolved_value!=expected_value) {
//		context->sub_fail_count++;
//		const char *key_as_text = key==NULL ? "NULL" : cat_string_wo_getchars(key);
//		const char *resolved_as_text = resolved_value==NULL ? "NULL" : cat_string_wo_getchars(resolved_value);
//		const char *expected_as_text = expected_value==NULL ? "NULL" : cat_string_wo_getchars(expected_value);
//		printf("fail !! %s resolved value '%s':%p does not match expected value '%s' for key '%s'\n", func_name, resolved_as_text, resolved_as_obj, expected_as_text, key_as_text);
//	}
//	context->sub_total_count++;
//}
//
//
//static void _test_simple_put(CatTestContext *context) {
//	CatFixedHashMap *hash_map = cat_fixed_hash_map_new((GHashFunc) cat_fixed_string_hash, (GEqualFunc) cat_fixed_string_equal);
//	int names_count;
//	CatFixedString **names = (CatFixedString **) cat_test_context_load_names(context, &names_count);
//	int add_count = names_count/2;
//	int add_idx;
//	int names_idx = 0;
//	for(add_idx=0; add_idx<add_count; add_idx++) {
//		CatFixedString *key = names[names_idx++];
//		CatFixedString *value = names[names_idx++];
//		hash_map = cat_fixed_hash_map_put(hash_map, (GObject *) key, (GObject *) value);
//	}
//	_test_map_size(context, hash_map, add_count);
//
//	float nice_bucket_count = (float) cat_fixed_hash_map_count(hash_map) * LOAD_FACTOR;
//	int expected_bckt_cnt = ceilf(nice_bucket_count);
//	expected_bckt_cnt = (expected_bckt_cnt-1+LOAD_FACTOR_INCREASE)/LOAD_FACTOR_INCREASE;
//	expected_bckt_cnt *= LOAD_FACTOR_INCREASE;
//	_test_map_bucket_count(context, hash_map, expected_bckt_cnt);
//
//	names_idx = 0;
//	for(add_idx=0; add_idx<add_count; add_idx++) {
//		GObject *key = (GObject *) names[names_idx++];
//		CatFixedString *value = CAT_FIXED_STRING(cat_fixed_hash_map_get(hash_map, key));
//		CatFixedString *expected_value = names[names_idx++];
//		_test_map_value(context, hash_map, "simple-put", CAT_FIXED_STRING(key), value, (GObject *) value, expected_value);
//	}
//	// TODO test if it is still floating
//	cat_ref_sink_ptr(hash_map);
//
//	cat_unref_ptr(hash_map);
//	cat_test_context_free_names(context, (void **) names);
//}
//
//
////
////static void _test_remove(CatTestContext *context) {
////	CatHashMap *hash_map = cat_hash_map_new(cat_fixed_string_hash, cat_fixed_string_equal);
////	int names_count;
////	CatFixedString **names = (CatFixedString **) cat_test_context_load_names(context, &names_count);
////	int add_count = names_count/2;
////	int add_idx;
////	int names_idx = 0;
////	for(add_idx=0; add_idx<add_count; add_idx++) {
////		cat_hash_map_put(hash_map, (GObject *) names[names_idx++], (GObject *) names[names_idx++]);
////	}
////	_test_map_size(context, hash_map, add_count);
////
////	for(add_idx=0; add_idx<add_count; add_idx++) {
////		GObject *key = (GObject *) names[names_idx++];
////		CatFixedString *expected_value = names[names_idx++];
////		GObject *removed_value = cat_hash_map_remove(hash_map, key);
////		_test_map_value(context, hash_map, "remove", CAT_FIXED_STRING(key), CAT_FIXED_STRING(removed_value), expected_value);
////	}
////
////	_test_map_size(context, hash_map, 0);
////
////	cat_unref_ptr(hash_map);
////	cat_test_context_free_names(context, names);
////}
//
//
//static void _test_remove(CatTestContext *context) {
//	CatFixedHashMap *hash_map = cat_fixed_hash_map_new((GHashFunc) cat_fixed_string_hash, (GEqualFunc) cat_fixed_string_equal);
//	int names_count;
//	CatFixedString **names = (CatFixedString **) cat_test_context_load_names(context, &names_count);
//	int add_count = names_count/2;
//	int add_idx;
//	int names_idx = 0;
//	for(add_idx=0; add_idx<add_count; add_idx++) {
//		hash_map = cat_fixed_hash_map_put(hash_map, (GObject *) names[names_idx], (GObject *) names[names_idx+1]);
//		names_idx += 2;
//	}
//	_test_map_size(context, hash_map, add_count);
//
//	/* simple get */
//
//	GObject *resolved = cat_fixed_hash_map_get(hash_map, names[0]);
//	_test_map_value(context, hash_map, "check-map", CAT_FIXED_STRING(names[0]), CAT_FIXED_STRING(resolved), resolved, CAT_FIXED_STRING(names[1]));
//
//
//	int sub_count = 0;
//	names_idx = 0;
//	// remove all odd values
//	for(add_idx=0; add_idx<add_count; add_idx++) {
//		GObject *key = (GObject *) names[names_idx];
//		CatFixedString *expected_value = names[names_idx+1];
//		if ((add_idx%2)==0) {
//			GObject *removed_value = NULL;
//			hash_map = cat_fixed_hash_map_remove(hash_map, key, &removed_value);
//			_test_map_value(context, hash_map, "remove", CAT_FIXED_STRING(key), CAT_FIXED_STRING(removed_value), removed_value, expected_value);
//			cat_unref(removed_value);
//			sub_count++;
//		}
//		names_idx += 2;
//	}
//
//	_test_map_size(context, hash_map, add_count-sub_count);
//
//	cat_unref_ptr(hash_map);
//	cat_test_context_free_names(context, (void **) names);
//}
//
//void test_fixed_hash_map(CatTestContext *context) {
//	cat_test_context_start_sub(context, __FILE__);
//	_test_simple_put(context);
//	_test_remove(context);
//	cat_test_context_stop_sub(context);
//}
