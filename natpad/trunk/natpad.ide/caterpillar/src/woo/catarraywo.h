/*
   File:    catarraywo.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Oct 6, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef CATARRAYWO_H_
#define CATARRAYWO_H_

#include "catwo.h"
#include "../catiiterator.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_ARRAY_WO              (cat_array_wo_get_type())
#define CAT_ARRAY_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_array_wo_get_type(), CatArrayWo))
#define CAT_ARRAY_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_ARRAY_WO, CatArrayWoClass))
#define CAT_IS_ARRAY_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_ARRAY_WO))
#define CAT_IS_ARRAY_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_ARRAY_WO))
#define CAT_ARRAY_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_ARRAY_WO, CatArrayWoClass))


typedef struct _CatArrayWo               CatArrayWo;
typedef struct _CatArrayWoPrivate        CatArrayWoPrivate;
typedef struct _CatArrayWoClass          CatArrayWoClass;


struct _CatArrayWo {
	CatWo parent;
};

struct _CatArrayWoClass {
	CatWoClass parent_class;
};


GType cat_array_wo_get_type();

CatArrayWo *cat_array_wo_new();
CatArrayWo *cat_array_wo_new_size(int initial_size);
CatArrayWo *cat_array_wo_new_sub(const CatArrayWo *source, int offset, int length);

CatArrayWo *cat_array_wo_create_editable(CatArrayWo *instance) __attribute_warn_unused_result__;
CatArrayWo *cat_array_wo_anchor(CatArrayWo *instance, int version) __attribute_warn_unused_result__;
CatArrayWo *cat_array_wo_get_original(CatArrayWo *instance);
gboolean cat_array_wo_is_anchored(const CatArrayWo *instance);

gboolean cat_array_wo_equal(const CatArrayWo *instance_a, const CatArrayWo *instance_b, GEqualFunc equal_func);
CatArrayWo *cat_array_wo_clone(const CatArrayWo *source, CatCloneDepth clone_depth) __attribute_warn_unused_result__;

/* read operations */

int cat_array_wo_size(CatArrayWo *array);

GObject *cat_array_wo_get(CatArrayWo *array, int index);
GObject *cat_array_wo_get_first(CatArrayWo *array);
GObject *cat_array_wo_get_last(CatArrayWo *array);


CatIIterator *cat_array_wo_iterator(CatArrayWo *array);

int cat_array_wo_find_index(CatArrayWo *array, const void *search_item, int hint_index);
int cat_array_wo_find_index_ext(CatArrayWo *array, const void *search_item, int hint_index, GEqualFunc eq_func);


/* modify operations */

void cat_array_wo_clear(CatArrayWo *e_array);

int cat_array_wo_append(CatArrayWo *e_array, GObject *to_add);
void cat_array_wo_append_all(CatArrayWo *e_array, CatArrayWo *list_to_append);

void cat_array_wo_insert(CatArrayWo *e_array, GObject *to_insert, int index);


gboolean cat_array_wo_remove(CatArrayWo *e_array, int index, GObject **removed);
gboolean cat_array_wo_remove_last(CatArrayWo *e_array, GObject **removed);
gboolean cat_array_wo_remove_first(CatArrayWo *e_array, GObject **removed);
void cat_array_wo_remove_range(CatArrayWo *e_array, int offset, int length);

gboolean cat_array_wo_set(CatArrayWo *e_array, GObject *to_insert, int index, GObject **old_value);

void cat_array_wo_limit(CatArrayWo *e_array, int offset, int length);
void cat_array_wo_pad(CatArrayWo *e_array, int length, GObject *fill_with);

/* util opperations */

void cat_array_wo_sort(CatArrayWo *e_array, GCompareFunc cmp_func);
void cat_array_wo_reverse(CatArrayWo *e_array);

G_END_DECLS

#endif /* CATARRAYWO_H_ */
