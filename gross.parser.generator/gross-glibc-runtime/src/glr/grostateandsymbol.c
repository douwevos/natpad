
/*
   File:    grostateandsymbol.c
   Project: gross-glibc-runtime
   Author:  Douwe Vos
   Date:    Jun 24, 2016
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

#include "grostateandsymbol.h"


struct _GroStateAndSymbolPrivate {
	GroStateAndSymbol *linked;
	int state;
	int symbol;
	gboolean is_terminal;
};


G_DEFINE_TYPE_WITH_CODE(GroStateAndSymbol, gro_state_and_symbol, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroStateAndSymbol)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_state_and_symbol_class_init(GroStateAndSymbolClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_state_and_symbol_init(GroStateAndSymbol *instance) {
}

static void l_dispose(GObject *object) {
	gro_log("dispose:%p", object);
	GroStateAndSymbol *instance = GRO_STATE_AND_SYMBOL(object);
	GroStateAndSymbolPrivate *priv = gro_state_and_symbol_get_instance_private(instance);
	gro_unref_ptr(priv->linked);
	G_OBJECT_CLASS(gro_state_and_symbol_parent_class)->dispose(object);
	gro_log("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	gro_log("finalize:%p", object);
	G_OBJECT_CLASS(gro_state_and_symbol_parent_class)->finalize(object);
	gro_log("finalized:%p", object);
}


GroStateAndSymbol *gro_state_and_symbol_new(GroStateAndSymbol *linked, int state, int symbol, gboolean is_terminal) {
	GroStateAndSymbol *result = g_object_new(GRO_TYPE_STATE_AND_SYMBOL, NULL);
	GroStateAndSymbolPrivate *priv = gro_state_and_symbol_get_instance_private(result);
	priv->linked = gro_ref_ptr(linked);
	priv->state = state;
	priv->symbol = symbol;
	priv->is_terminal = is_terminal;
	return result;
}

GroStateAndSymbol *gro_state_and_symbol_get_linked(GroStateAndSymbol *state_and_symbol) {
	GroStateAndSymbolPrivate *priv = gro_state_and_symbol_get_instance_private(state_and_symbol);
	return priv->linked;
}

int gro_state_and_symbol_get_state(GroStateAndSymbol *state_and_symbol) {
	GroStateAndSymbolPrivate *priv = gro_state_and_symbol_get_instance_private(state_and_symbol);
	return priv->state;
}

int gro_state_and_symbol_get_symbol(GroStateAndSymbol *state_and_symbol) {
	GroStateAndSymbolPrivate *priv = gro_state_and_symbol_get_instance_private(state_and_symbol);
	return priv->symbol;
}

gboolean gro_state_and_symbol_mergable(GroStateAndSymbol *state_and_symbol_a, GroStateAndSymbol *state_and_symbol_b) {
	GroStateAndSymbolPrivate *priv_a = gro_state_and_symbol_get_instance_private(state_and_symbol_a);
	GroStateAndSymbolPrivate *priv_b = gro_state_and_symbol_get_instance_private(state_and_symbol_b);
	return (priv_a->linked==priv_b->linked)
			&& (priv_a->symbol==priv_b->symbol)
			&& (priv_a->state==priv_b->state)
			&& (priv_a->is_terminal==priv_b->is_terminal);
}
