/*
   File:    catfixedintarray.h
   Project: Natpad
   Author:  Douwe Vos
   Date:    Feb 9, 2009
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


#ifndef CATFIXEDINTARRAY_H_
#define CATFIXEDINTARRAY_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_FIXED_INT_ARRAY            (cat_fixed_int_array_get_type())
#define CAT_FIXED_INT_ARRAY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_fixed_int_array_get_type(), CatFixedIntArray))
#define CAT_FIXED_INT_ARRAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_FIXED_INT_ARRAY, CatFixedIntArrayClass))
#define CAT_IS_FIXED_INT_ARRAY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_FIXED_INT_ARRAY))
#define CAT_IS_FIXED_INT_ARRAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_FIXED_INT_ARRAY))
#define CAT_FIXED_INT_ARRAY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_FIXED_INT_ARRAY, CatFixedIntArrayClass))


typedef struct _CatFixedIntArray	      CatFixedIntArray;

typedef struct _CatFixedIntArrayClass	  CatFixedIntArrayClass;

struct _CatFixedIntArray {
	GInitiallyUnowned parent;
	int *data;
	int size;
};

struct _CatFixedIntArrayClass {
	GInitiallyUnownedClass parent_class;
};

CatFixedIntArray *cat_fixed_int_array_new();

int cat_fixed_int_array_size(CatFixedIntArray *fixed_int_array);

int cat_fixed_int_array_get(CatFixedIntArray *fixed_int_array, int index);

CatFixedIntArray *cat_fixed_int_array_append(CatFixedIntArray *fixed_int_array, int to_append);

CatFixedIntArray *cat_fixed_int_array_insert(CatFixedIntArray *fixed_int_array, int index, int to_insert);

CatFixedIntArray *cat_fixed_int_array_remove(CatFixedIntArray *fixed_int_array, int index, int *removed);

G_END_DECLS

#endif /* CATFIXEDINTARRAY_H_ */
