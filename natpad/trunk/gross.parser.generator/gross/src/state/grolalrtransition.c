
/*
   File:    grolalrtransition.c
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 3, 2016
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

#include "grolalrtransition.h"
#include "grolalrstate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroLalrTransition"
#include <logging/catlog.h>

struct _GroLalrTransitionPrivate {
	GroMSymbol *on_sym;
	GroLalrState *to_state;
	GroLalrTransition *next;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroLalrTransition, gro_lalr_transition, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroLalrTransition)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_lalr_transition_class_init(GroLalrTransitionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_lalr_transition_init(GroLalrTransition *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroLalrTransition *instance = GRO_LALR_TRANSITION(object);
//	GroLalrTransitionPrivate *priv = gro_lalr_transition_get_instance_private(instance);
	G_OBJECT_CLASS(gro_lalr_transition_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_lalr_transition_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

GroLalrTransition *gro_lalr_transition_new(GroMSymbol *on_sym, struct _GroLalrState *to_state, GroLalrTransition *next) {
	GroLalrTransition *result = g_object_new(GRO_TYPE_LALR_TRANSITION, NULL);
	cat_ref_anounce(result);
	GroLalrTransitionPrivate *priv = gro_lalr_transition_get_instance_private(result);
	priv->on_sym = cat_ref_ptr(on_sym);
	priv->to_state = cat_ref_ptr(to_state);
	priv->next = cat_ref_ptr(next);
	return result;
}

GroLalrTransition *gro_lalr_transition_next(GroLalrTransition *transition) {
	GroLalrTransitionPrivate *priv = gro_lalr_transition_get_instance_private(transition);
	return priv->next;
}

GroMSymbol *gro_lalr_transition_get_key_symbol(GroLalrTransition *transition) {
	GroLalrTransitionPrivate *priv = gro_lalr_transition_get_instance_private(transition);
	return priv->on_sym;
}

struct _GroLalrState *gro_lalr_transition_get_to_state(GroLalrTransition *transition) {
	GroLalrTransitionPrivate *priv = gro_lalr_transition_get_instance_private(transition);
	return priv->to_state;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroLalrTransition *instance = GRO_LALR_TRANSITION(self);
	GroLalrTransitionPrivate *priv = gro_lalr_transition_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, on-sym=%O, to_state=%d]", iname, self, priv->on_sym, gro_lalr_state_get_index(priv->to_state));
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
