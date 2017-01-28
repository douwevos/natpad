
/*
   File:    grolalrstate.c
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 2, 2016
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

#include "grolalrstate.h"
#include "groparseactiontable.h"
#include "groparsereducetable.h"
#include "groiparseaction.h"
#include "groactionshift.h"
#include <logging/catlogdefs.h>

#include "groactionreduce.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroLalrState"
#include <logging/catlog.h>

struct _GroLalrStatePrivate {
	GroLalrItemSet *item_set;
	int index;
	GroLalrTransition *transitions;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroLalrState, gro_lalr_state, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroLalrState)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_lalr_state_class_init(GroLalrStateClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_lalr_state_init(GroLalrState *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroLalrState *instance = GRO_LALR_STATE(object);
//	GroLalrStatePrivate *priv = gro_lalr_state_get_instance_private(instance);
	G_OBJECT_CLASS(gro_lalr_state_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_lalr_state_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

GroLalrState *gro_lalr_state_new(GroLalrItemSet *item_set, int index) {
	GroLalrState *result = g_object_new(GRO_TYPE_LALR_STATE, NULL);
	cat_ref_anounce(result);
	GroLalrStatePrivate *priv = gro_lalr_state_get_instance_private(result);
	priv->item_set = cat_ref_ptr(item_set);
	priv->index = index;
	priv->transitions = NULL;
	return result;
}

GroLalrItemSet *gro_lalr_state_get_item_set(GroLalrState *state) {
	GroLalrStatePrivate *priv = gro_lalr_state_get_instance_private(state);
	return priv->item_set;
}

int gro_lalr_state_get_index(GroLalrState *state) {
	GroLalrStatePrivate *priv = gro_lalr_state_get_instance_private(state);
	return priv->index;
}


void gro_lalr_state_add_transition(GroLalrState *state, GroMSymbol *on_sym, GroLalrState *to_state) {
	GroLalrStatePrivate *priv = gro_lalr_state_get_instance_private(state);
	GroLalrTransition *ntr = gro_lalr_transition_new(on_sym, to_state, priv->transitions);
	cat_unref_ptr(priv->transitions);
	priv->transitions = ntr;
}

void gro_lalr_state_propagate_lookaheads(GroLalrState *state) {
	GroLalrStatePrivate *priv = gro_lalr_state_get_instance_private(state);
	GroLalrItemIterator *iter = gro_lalr_item_set_iterator(priv->item_set);
	while(gro_lalr_item_iterator_has_next(iter)) {
		GroLalrItem *item = gro_lalr_item_iterator_next(iter);
		gro_lalr_item_propagate_lookaheads(item, NULL);
	}
	cat_unref_ptr(iter);
}


GroLalrTransition *gro_lalr_state_get_transitions(GroLalrState *state) {
	GroLalrStatePrivate *priv = gro_lalr_state_get_instance_private(state);
	return priv->transitions;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroLalrState *instance = GRO_LALR_STATE(self);
	GroLalrStatePrivate *priv = gro_lalr_state_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[index=%d, %o, transitions:", iname, priv->index, priv->item_set);
	GroLalrTransition *trans = priv->transitions;
	while(trans) {
		GroLalrTransition *ltrans = trans;
		trans = gro_lalr_transition_next(trans);
		if (trans) {
			cat_string_wo_format(append_to, "%O > ", ltrans);
		} else {
			cat_string_wo_format(append_to, "%O", ltrans);
		}
	}
	cat_string_wo_append_char(append_to, ']');
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
