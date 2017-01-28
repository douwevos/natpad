/*
   File:    grorunaccepttransition.c
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

#include "grorunaccepttransition.h"
#include "groruntransition.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunAcceptTransition"
#include <logging/catlog.h>

struct _GroRunAcceptTransitionPrivate {
	void *dummy;
};

static void l_transition_iface_init(GroRunTransitionInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunAcceptTransition, grorun_accept_transition, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunAcceptTransition)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_TRANSITION, l_transition_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_accept_transition_class_init(GroRunAcceptTransitionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_accept_transition_init(GroRunAcceptTransition *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroRunAcceptTransition *instance = GRORUN_ACCEPT_TRANSITION(object);
//	GroRunAcceptTransitionPrivate *priv = grorun_accept_transition_get_instance_private(instance);
	G_OBJECT_CLASS(grorun_accept_transition_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_accept_transition_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunAcceptTransition *grorun_accept_transition_new() {
	GroRunAcceptTransition *result = g_object_new(GRORUN_TYPE_ACCEPT_TRANSITION, NULL);
	cat_ref_anounce(result);
//	GroRunAcceptTransitionPrivate *priv = grorun_accept_transition_get_instance_private(result);
//	G_OBJECT_construct((GObject *) result);
	return result;
}



/********************* start GroRunTransition implementation *********************/

static void l_transition_iface_init(GroRunTransitionInterface *iface) {
}

/********************* endGroRunTransition implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroRunAcceptTransition *instance = GRORUN_ACCEPT_TRANSITION(self);
//	GroRunAcceptTransitionPrivate *priv = grorun_accept_transition_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
