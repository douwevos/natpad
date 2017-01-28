/*
   File:    grorunstate.c
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

#include "grorunstate.h"
#include "grorunshifttransition.h"
#include "grorunreducetransition.h"
#include "grorunaccepttransition.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunState"
#include <logging/catlog.h>

struct _GroRunStatePrivate {
	int id;
	CatHashMapWo *transition_map; /* <Symbol, TransitionsForSymbol> */

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunState, grorun_state, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunState)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_state_class_init(GroRunStateClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_state_init(GroRunState *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroRunState *instance = GRORUN_STATE(object);
	GroRunStatePrivate *priv = grorun_state_get_instance_private(instance);
	cat_unref_ptr(priv->transition_map);
	G_OBJECT_CLASS(grorun_state_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_state_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunState *grorun_state_new(int id) {
	GroRunState *result = g_object_new(GRORUN_TYPE_STATE, NULL);
	cat_ref_anounce(result);
	GroRunStatePrivate *priv = grorun_state_get_instance_private(result);
	priv->id = id;
	priv->transition_map = cat_hash_map_wo_new((GHashFunc) grorun_symbol_hash, (GEqualFunc) grorun_symbol_equal);
	return result;
}

void grorun_state_add_shift(GroRunState *state, GroRunSymbol *for_symbol, GroRunState *shift_to_state) {
	GroRunTransitionsForSymbol *trans_for_sym = grorun_state_get_or_create_transitions_for_symbol(state, for_symbol);
	GroRunShiftTransition *shift_trans = grorun_shift_transition_new(shift_to_state);
	grorun_transitions_for_symbol_add(trans_for_sym, (GroRunTransition *) shift_trans);
	cat_unref_ptr(shift_trans);
}

void grorun_state_add_reduce(GroRunState *state, GroRunSymbol *for_symbol, GroRunReduceLayout *reduce_layout, int action_id) {
	GroRunTransitionsForSymbol *trans_for_sym = grorun_state_get_or_create_transitions_for_symbol(state, for_symbol);
	GroRunReduceTransition *red_trans = grorun_reduce_transition_new(reduce_layout, action_id);
	grorun_transitions_for_symbol_add(trans_for_sym, (GroRunTransition *) red_trans);
	cat_unref_ptr(red_trans);
}

void grorun_state_add_accept(GroRunState *state, GroRunSymbol *for_symbol) {
	GroRunTransitionsForSymbol *trans_for_sym = grorun_state_get_or_create_transitions_for_symbol(state, for_symbol);
	GroRunAcceptTransition *acc_trans = grorun_accept_transition_new();
	grorun_transitions_for_symbol_add(trans_for_sym, (GroRunTransition *) acc_trans);
	cat_unref_ptr(acc_trans);
}


GroRunTransitionsForSymbol *grorun_state_get_or_create_transitions_for_symbol(GroRunState *state, GroRunSymbol *for_symbol) {
	GroRunStatePrivate *priv = grorun_state_get_instance_private(state);
	GroRunTransitionsForSymbol *result = (GroRunTransitionsForSymbol *) cat_hash_map_wo_get(priv->transition_map, for_symbol);
	if (result==NULL) {
		result = grorun_transitions_for_symbol_new(for_symbol);
		cat_hash_map_wo_put(priv->transition_map, (GObject*) for_symbol, (GObject*) result);
		cat_unref(result);
	}
	return result;
}

GroRunTransitionsForSymbol *grorun_state_get_transitions_for_symbol(GroRunState *state, GroRunSymbol *for_symbol) {
	GroRunStatePrivate *priv = grorun_state_get_instance_private(state);
	return (GroRunTransitionsForSymbol *) cat_hash_map_wo_get(priv->transition_map, for_symbol);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunState *instance = GRORUN_STATE(self);
	GroRunStatePrivate *priv = grorun_state_get_instance_private(instance);
	cat_string_wo_format(append_to, "state[%d]", priv->id);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
