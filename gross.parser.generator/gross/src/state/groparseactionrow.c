
/*
   File:    groparseactionrow.c
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

#include "groparseactionrow.h"
#include "groactionerror.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroParseActionRow"
#include <logging/catlog.h>

struct _GroParseActionRowPrivate {
	GroModel *model;
	int action_count;
	GroIParseAction **actions;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroParseActionRow, gro_parse_action_row, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroParseActionRow)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_parse_action_row_class_init(GroParseActionRowClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_parse_action_row_init(GroParseActionRow *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroParseActionRow *instance = GRO_PARSE_ACTION_ROW(object);
//	GroParseActionRowPrivate *priv = gro_parse_action_row_get_instance_private(instance);
	G_OBJECT_CLASS(gro_parse_action_row_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_parse_action_row_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroParseActionRow *gro_parse_action_row_new(GroModel *model) {
	GroParseActionRow *result = g_object_new(GRO_TYPE_PARSE_ACTION_ROW, NULL);
	cat_ref_anounce(result);
	GroParseActionRowPrivate *priv = gro_parse_action_row_get_instance_private(result);
	priv->model = cat_ref_ptr(model);
	priv->action_count = gro_model_terminal_count(model);
	priv->actions = g_malloc(sizeof(GroIParseAction *) * priv->action_count);

	/* set each element to an error action */
	int i;
	for(i = 0; i < priv->action_count; i++) {
		priv->actions[i] = (GroIParseAction *) gro_action_error_new();
	}

	return result;
}

GroIParseAction *gro_parse_action_row_get_action(GroParseActionRow *action_row, int index) {
	GroParseActionRowPrivate *priv = gro_parse_action_row_get_instance_private(action_row);
	return priv->actions[index];
}

void gro_parse_action_row_set_action(GroParseActionRow *action_row, int index, GroIParseAction *action) {
	GroParseActionRowPrivate *priv = gro_parse_action_row_get_instance_private(action_row);
	cat_ref_swap(priv->actions[index], action);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroParseActionRow *instance = GRO_PARSE_ACTION_ROW(self);
	GroParseActionRowPrivate *priv = gro_parse_action_row_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, action_count=%d]", iname, self, priv->action_count);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
