///*
//   File:    catfixedlist.c
//   Project: Natpad
//   Author:  Douwe Vos
//   Date:    Feb 7, 2009
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2008 Douwe Vos.
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
//
//#include "catfixedintarray.h"
//#include "catlib.h"
//#include <stdio.h>
//#include <string.h>
//
//#include "logging/catlogdefs.h"
//#define CAT_LOG_LEVEL CAT_LOG_FATAL
//#define CAT_LOG_CLAZZ "CatFixedIntArray"
//#include "logging/catlog.h"
//
//
//G_DEFINE_TYPE(CatFixedIntArray, cat_fixed_int_array, G_TYPE_INITIALLY_UNOWNED)
//
//static void _dispose(GObject *object);
//
//static void cat_fixed_int_array_class_init(CatFixedIntArrayClass *dcclass) {
//	GObjectClass *object_class = G_OBJECT_CLASS(dcclass);
//	object_class->dispose = _dispose;
//}
//
//static void cat_fixed_int_array_init(CatFixedIntArray *block_array) {
//}
//
//static void _dispose(GObject *object) {
//	CatFixedIntArray *int_array = CAT_FIXED_INT_ARRAY(object);
//	cat_free_ptr(int_array->data);
//	int_array->size = 0;
//}
//
//
//CatFixedIntArray *cat_fixed_int_array_new() {
//	CatFixedIntArray *result = g_object_new(CAT_TYPE_FIXED_INT_ARRAY, NULL);
//	cat_ref_anounce(result);
//	result->data = NULL;
//	result->size = 0;
//	return result;
//}
//
//int cat_fixed_int_array_size(CatFixedIntArray *fixed_int_array) {
//	return fixed_int_array->size;
//}
//
//int cat_fixed_int_array_get(CatFixedIntArray *fixed_int_array, int index) {
//	if (index>=0 && index<fixed_int_array->size) {
//		return fixed_int_array->data[index];
//	}
//	return 0;
//}
//
//
//
//CatFixedIntArray *cat_fixed_int_array_append(CatFixedIntArray *fixed_int_array, int to_append) {
//	CatFixedIntArray *result = cat_fixed_int_array_new();
//	result->data = g_malloc(sizeof(int) * (fixed_int_array->size+1));
//	memcpy(result->data, fixed_int_array->data, sizeof(int)*fixed_int_array->size);
//	result->size = fixed_int_array->size+1;
//	result->data[fixed_int_array->size] = to_append;
//	return result;
//}
//
//CatFixedIntArray *cat_fixed_int_array_insert(CatFixedIntArray *fixed_int_array, int index, int to_insert) {
//	if (index>=fixed_int_array->size) {
//		return cat_fixed_int_array_append(fixed_int_array, to_insert);
//	}
//	if (index<0) {
//		index = 0;
//	}
//	CatFixedIntArray *result = cat_fixed_int_array_new();
//	result->data = g_malloc(sizeof(int) * (fixed_int_array->size+1));
//	if (index>0) {
//		memcpy(result->data, fixed_int_array->data, index * sizeof(int));
//	}
//	if (index<fixed_int_array->size) {
//		memcpy(result->data+index+1, fixed_int_array->data+index, sizeof(int)*(fixed_int_array->size-index));
//	}
//	result->data[index] = to_insert;
//	result->size = fixed_int_array->size+1;
//	return result;
//}
//
//
//CatFixedIntArray *cat_fixed_int_array_remove(CatFixedIntArray *fixed_int_array, int index, int *removed) {
//	if (index<0 || index>=fixed_int_array->size) {
//		if (removed!=NULL) {
//			*removed = -1;
//		}
//		cat_ref(fixed_int_array);
//		return fixed_int_array;
//	}
//	if (removed!=NULL) {
//		*removed = fixed_int_array->data[index];
//	}
//	CatFixedIntArray *result = cat_fixed_int_array_new();
//	result->data = g_malloc(sizeof(int) * (fixed_int_array->size-1));
//	result->size = fixed_int_array->size-1;
//
//	if (index>0) {
//		memcpy(result->data, fixed_int_array->data, sizeof(int)*index);
//	}
//	if (index+1<fixed_int_array->size) {
//		memcpy(result->data+index, fixed_int_array->data+index+1, sizeof(int)*(fixed_int_array->size-index-1));
//	}
//	return result;
//}
