
/*
   File:    groactionshift.c
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 6, 2016
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

#include "groactionshift.h"
#include "groiparseaction.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroActionShift"
#include <logging/catlog.h>

struct _GroActionShiftPrivate {
	GroLalrState *shift_to;
};

static void l_parse_action_iface_init(GroIParseActionInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroActionShift, gro_action_shift, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroActionShift)
		G_IMPLEMENT_INTERFACE(GRO_TYPE_IPARSE_ACTION, l_parse_action_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_action_shift_class_init(GroActionShiftClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_action_shift_init(GroActionShift *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroActionShift *instance = GRO_ACTION_SHIFT(object);
//	GroActionShiftPrivate *priv = gro_action_shift_get_instance_private(instance);
	G_OBJECT_CLASS(gro_action_shift_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_action_shift_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroActionShift *gro_action_shift_new(GroLalrState *shift_to) {
	GroActionShift *result = g_object_new(GRO_TYPE_ACTION_SHIFT, NULL);
	cat_ref_anounce(result);
	GroActionShiftPrivate *priv = gro_action_shift_get_instance_private(result);
	priv->shift_to = cat_ref_ptr(shift_to);
	return result;
}

GroLalrState *gro_action_shift_get_shift_to(GroActionShift *action) {
	GroActionShiftPrivate *priv = gro_action_shift_get_instance_private(action);
	return priv->shift_to;
}

/********************* start GroIParseAction implementation *********************/

static GroActionKind l_parse_action_kind(GroIParseAction *action) {
	return GRO_ACTION_KIND_SHIFT;
}

static void l_parse_action_iface_init(GroIParseActionInterface *iface) {
	iface->kind = l_parse_action_kind;
}

/********************* end GroIParseAction implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroActionShift *instance = GRO_ACTION_SHIFT(self);
	GroActionShiftPrivate *priv = gro_action_shift_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, shift_to=%O]", iname, self, priv->shift_to);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
