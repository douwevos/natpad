/*
   File:    grorunshifttransition.c
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 10, 2016
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

#include "grorunshifttransition.h"
#include "groruntransition.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunShiftTransition"
#include <logging/catlog.h>

struct _GroRunShiftTransitionPrivate {
	GroRunState *shift_to_state;
};

static void l_transition_iface_init(GroRunTransitionInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunShiftTransition, grorun_shift_transition, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunShiftTransition)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_TRANSITION, l_transition_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_shift_transition_class_init(GroRunShiftTransitionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_shift_transition_init(GroRunShiftTransition *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroRunShiftTransition *instance = GRORUN_SHIFT_TRANSITION(object);
//	GroRunShiftTransitionPrivate *priv = grorun_shift_transition_get_instance_private(instance);
//	cat_unref_ptr(priv->shift_to_state);
	G_OBJECT_CLASS(grorun_shift_transition_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_shift_transition_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunShiftTransition *grorun_shift_transition_new(GroRunState *shift_to_state) {
	GroRunShiftTransition *result = g_object_new(GRORUN_TYPE_SHIFT_TRANSITION, NULL);
	cat_ref_anounce(result);
	GroRunShiftTransitionPrivate *priv = grorun_shift_transition_get_instance_private(result);
	priv->shift_to_state = shift_to_state;
	return result;
}

GroRunState *grorun_shift_transition_get_forward_state(GroRunShiftTransition *shift_trans) {
	GroRunShiftTransitionPrivate *priv = grorun_shift_transition_get_instance_private(shift_trans);
	return priv->shift_to_state;
}

/********************* start GroRunTransition implementation *********************/

static void l_transition_iface_init(GroRunTransitionInterface *iface) {
}

/********************* endGroRunTransition implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunShiftTransition *instance = GRORUN_SHIFT_TRANSITION(self);
	GroRunShiftTransitionPrivate *priv = grorun_shift_transition_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p to_state=%O]", iname, self, priv->shift_to_state);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

