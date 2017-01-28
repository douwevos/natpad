
/*
   File:    groparsereducetable.c
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 5, 2016
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

#include "groparsereducetable.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroParseReduceTable"
#include <logging/catlog.h>

struct _GroParseReduceTablePrivate {
	GroModel *model;
	GroParseReduceRow **rows;
	int row_count;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroParseReduceTable, gro_parse_reduce_table, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroParseReduceTable)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_parse_reduce_table_class_init(GroParseReduceTableClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_parse_reduce_table_init(GroParseReduceTable *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroParseReduceTable *instance = GRO_PARSE_REDUCE_TABLE(object);
//	GroParseReduceTablePrivate *priv = gro_parse_reduce_table_get_instance_private(instance);
	G_OBJECT_CLASS(gro_parse_reduce_table_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_parse_reduce_table_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroParseReduceTable *gro_parse_reduce_table_new(GroModel *model, int state_count) {
	GroParseReduceTable *result = g_object_new(GRO_TYPE_PARSE_REDUCE_TABLE, NULL);
	cat_ref_anounce(result);
	GroParseReduceTablePrivate *priv = gro_parse_reduce_table_get_instance_private(result);
	priv->model = cat_ref_ptr(model);

	priv->row_count = state_count;
	priv->rows = g_malloc(sizeof(GroParseReduceRow *) * state_count);
	int row_idx;
	for(row_idx=0; row_idx<state_count; row_idx++) {
		priv->rows[row_idx] = gro_parse_reduce_row_new(model);
	}
	return result;
}

GroParseReduceRow *gro_parse_reduce_table_get_row(GroParseReduceTable *reduce_table, int index) {
	GroParseReduceTablePrivate *priv = gro_parse_reduce_table_get_instance_private(reduce_table);
	return priv->rows[index];
}

int gro_parse_reduce_table_row_count(GroParseReduceTable *reduce_table) {
	GroParseReduceTablePrivate *priv = gro_parse_reduce_table_get_instance_private(reduce_table);
	return priv->row_count;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroParseReduceTable *instance = GRO_PARSE_REDUCE_TABLE(self);
	GroParseReduceTablePrivate *priv = gro_parse_reduce_table_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, row_count=%d]", iname, self, priv->row_count);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
