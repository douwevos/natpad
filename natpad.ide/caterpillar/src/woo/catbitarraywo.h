/*
   File:    catbitarraywo.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    May 30, 2015
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

#ifndef CATBITARRAYWO_H_
#define CATBITARRAYWO_H_

#include "catwo.h"
#include "../catiiterator.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_BIT_ARRAY_WO              (cat_bit_array_wo_get_type())
#define CAT_BIT_ARRAY_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_bit_array_wo_get_type(), CatBitArrayWo))
#define CAT_BIT_ARRAY_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_BIT_ARRAY_WO, CatBitArrayWoClass))
#define CAT_IS_BIT_ARRAY_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_BIT_ARRAY_WO))
#define CAT_IS_BIT_ARRAY_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_BIT_ARRAY_WO))
#define CAT_BIT_ARRAY_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_BIT_ARRAY_WO, CatBitArrayWoClass))


typedef struct _CatBitArrayWo               CatBitArrayWo;
typedef struct _CatBitArrayWoPrivate        CatBitArrayWoPrivate;
typedef struct _CatBitArrayWoClass          CatBitArrayWoClass;


struct _CatBitArrayWo {
	CatWo parent;
};

struct _CatBitArrayWoClass {
	CatWoClass parent_class;
};


GType cat_bit_array_wo_get_type();

CatBitArrayWo *cat_bit_array_wo_new();

int cat_bit_array_wo_length(CatBitArrayWo *bit_array);

gboolean cat_bit_array_wo_get(CatBitArrayWo *bit_array, int index);
gboolean cat_bit_array_wo_set(CatBitArrayWo *e_bit_array, int index);
void cat_bit_array_wo_set_all(CatBitArrayWo *e_bit_array);
void cat_bit_array_wo_invert(CatBitArrayWo *e_bit_array);

gboolean cat_bit_array_wo_unset(CatBitArrayWo *e_bit_array, int index);
void cat_bit_array_wo_toggle(CatBitArrayWo *e_bit_array, int index);

void cat_bit_array_wo_remove_range(CatBitArrayWo *e_bit_array, int offset, int length);
void cat_bit_array_wo_insert(CatBitArrayWo *e_bit_array, int offset, int length);

gboolean cat_bit_array_wo_has_at_least_one_set(const CatBitArrayWo *bit_array);

CAT_WO_BASE_H(CatBitArrayWo,cat_bit_array_wo);

G_END_DECLS

#endif /* CATBITARRAYWO_H_ */
