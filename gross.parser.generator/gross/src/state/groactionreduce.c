
/*
   File:    groreduceaction.c
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

#include "groactionreduce.h"
#include "groiparseaction.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroActionReduce"
#include <logging/catlog.h>

struct _GroActionReducePrivate {
	GroMProduction *production;
};

static void l_parse_action_iface_init(GroIParseActionInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroActionReduce, gro_action_reduce, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroActionReduce)
		G_IMPLEMENT_INTERFACE(GRO_TYPE_IPARSE_ACTION, l_parse_action_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_action_reduce_class_init(GroActionReduceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_action_reduce_init(GroActionReduce *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroActionReduce *instance = GRO_ACTION_REDUCE(object);
//	GroActionReducePrivate *priv = gro_action_reduce_get_instance_private(instance);
	G_OBJECT_CLASS(gro_action_reduce_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_action_reduce_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroActionReduce *gro_action_reduce_new(GroMProduction *production) {
	GroActionReduce *result = g_object_new(GRO_TYPE_ACTION_REDUCE, NULL);
	cat_ref_anounce(result);
	GroActionReducePrivate *priv = gro_action_reduce_get_instance_private(result);
	priv->production = cat_ref_ptr(production);
	return result;
}

GroMProduction *gro_action_reduce_get_production(GroActionReduce *action_reduce) {
	GroActionReducePrivate *priv = gro_action_reduce_get_instance_private(action_reduce);
	return priv->production;
}


/********************* start GroIParseAction implementation *********************/

static GroActionKind l_parse_action_kind(GroIParseAction *action) {
	return GRO_ACTION_KIND_REDUCE;
}

static void l_parse_action_iface_init(GroIParseActionInterface *iface) {
	iface->kind = l_parse_action_kind;
}

/********************* end GroIParseAction implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroActionReduce *instance = GRO_ACTION_REDUCE(self);
	GroActionReducePrivate *priv = gro_action_reduce_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, production=%O]", iname, self, priv->production);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
