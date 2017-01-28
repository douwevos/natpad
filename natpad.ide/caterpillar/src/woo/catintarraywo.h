/*
   File:    catintarraywo.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jan 10, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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
#ifndef SRC_WOO_CATINTARRAYWO_H_
#define SRC_WOO_CATINTARRAYWO_H_

#include "catwo.h"
#include "../catiiterator.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_INT_ARRAY_WO              (cat_int_array_wo_get_type())
#define CAT_INT_ARRAY_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_int_array_wo_get_type(), CatIntArrayWo))
#define CAT_INT_ARRAY_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_INT_ARRAY_WO, CatIntArrayWoClass))
#define CAT_IS_INT_ARRAY_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_INT_ARRAY_WO))
#define CAT_IS_INT_ARRAY_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_INT_ARRAY_WO))
#define CAT_INT_ARRAY_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_INT_ARRAY_WO, CatIntArrayWoClass))


typedef struct _CatIntArrayWo               CatIntArrayWo;
typedef struct _CatIntArrayWoPrivate        CatIntArrayWoPrivate;
typedef struct _CatIntArrayWoClass          CatIntArrayWoClass;


struct _CatIntArrayWo {
	CatWo parent;
};

struct _CatIntArrayWoClass {
	CatWoClass parent_class;
};


GType cat_int_array_wo_get_type();

CatIntArrayWo *cat_int_array_wo_new();
CatIntArrayWo *cat_int_array_wo_new_size(int initial_size);
CatIntArrayWo *cat_int_array_wo_new_sub(const CatIntArrayWo *source, int offset, int length);

int cat_int_array_wo_size(CatIntArrayWo *array);

int cat_int_array_wo_get(CatIntArrayWo *array, int index);
int cat_int_array_wo_get_first(CatIntArrayWo *array);
int cat_int_array_wo_get_last(CatIntArrayWo *array);

int cat_int_array_wo_find_index(CatIntArrayWo *array, int search_item, int hint_index);

void cat_int_array_wo_clear(CatIntArrayWo *e_array);
int cat_int_array_wo_append(CatIntArrayWo *e_array, int to_add);
void cat_int_array_wo_append_all(CatIntArrayWo *e_array, CatIntArrayWo *list_to_append);
void cat_int_array_wo_insert(CatIntArrayWo *e_array, int to_insert, int index);

gboolean cat_int_array_wo_remove(CatIntArrayWo *e_array, int index, int *removed);
void cat_int_array_wo_remove_range(CatIntArrayWo *e_array, int offset, int length);

gboolean cat_int_array_wo_remove_last(CatIntArrayWo *e_array, int *removed);
gboolean cat_int_array_wo_remove_first(CatIntArrayWo *e_array,  int *removed);

gboolean cat_int_array_wo_set(CatIntArrayWo *e_array, int to_set, int index, int *old_value);

void cat_int_array_wo_limit(CatIntArrayWo *e_array, int offset, int length);
void cat_int_array_wo_pad(CatIntArrayWo *e_array, int length, int fill_with);

void cat_int_array_wo_reverse(CatIntArrayWo *e_array);

CAT_WO_BASE_H(CatIntArrayWo,cat_int_array_wo);

G_END_DECLS

#endif /* SRC_WOO_CATINTARRAYWO_H_ */
