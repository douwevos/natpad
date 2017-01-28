
/*
   File:    groparsereducerow.c
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

#include "groparsereducerow.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroParseReduceRow"
#include <logging/catlog.h>

struct _GroParseReduceRowPrivate {
	GroModel *model;
	int reduce_count;
	GroLalrState **reduces;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroParseReduceRow, gro_parse_reduce_row, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroParseReduceRow)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_parse_reduce_row_class_init(GroParseReduceRowClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_parse_reduce_row_init(GroParseReduceRow *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroParseReduceRow *instance = GRO_PARSE_REDUCE_ROW(object);
//	GroParseReduceRowPrivate *priv = gro_parse_reduce_row_get_instance_private(instance);
	G_OBJECT_CLASS(gro_parse_reduce_row_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_parse_reduce_row_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroParseReduceRow *gro_parse_reduce_row_new(GroModel *model) {
	GroParseReduceRow *result = g_object_new(GRO_TYPE_PARSE_REDUCE_ROW, NULL);
	cat_ref_anounce(result);
	GroParseReduceRowPrivate *priv = gro_parse_reduce_row_get_instance_private(result);
	priv->model = cat_ref_ptr(model);
	priv->reduce_count = gro_model_non_terminal_count(model);
	priv->reduces = g_malloc0(sizeof(GroLalrState *) * priv->reduce_count);
	return result;
}

GroLalrState *gro_parse_reduce_row_get_to_state(GroParseReduceRow *reduce_row, int sym_index) {
	GroParseReduceRowPrivate *priv = gro_parse_reduce_row_get_instance_private(reduce_row);
	return priv->reduces[sym_index];
}

void gro_parse_reduce_row_set_to_state(GroParseReduceRow *reduce_row, int sym_index, GroLalrState *to_state) {
	GroParseReduceRowPrivate *priv = gro_parse_reduce_row_get_instance_private(reduce_row);
	cat_log_debug("### reduce_row=%p, sym_index=%d, to_state=%p", reduce_row, sym_index, to_state);
	cat_ref_swap(priv->reduces[sym_index], to_state);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroParseReduceRow *instance = GRO_PARSE_REDUCE_ROW(self);
	GroParseReduceRowPrivate *priv = gro_parse_reduce_row_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, reduce-cnt=%d]", iname, self, priv->reduce_count);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
