/*
   File:    grorunstackentry.c
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 11, 2016
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

#include "grorunstackentry.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunStackEntry"
#include <logging/catlog.h>

struct _GroRunStackEntryPrivate {
	GroRunStackEntry *parent;
	GroRunState *state;
	GroRunIToken *lookahead;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunStackEntry, grorun_stack_entry, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunStackEntry)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_stack_entry_class_init(GroRunStackEntryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_stack_entry_init(GroRunStackEntry *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroRunStackEntry *instance = GRORUN_STACK_ENTRY(object);
	GroRunStackEntryPrivate *priv = grorun_stack_entry_get_instance_private(instance);
	cat_unref_ptr(priv->parent);
	cat_unref_ptr(priv->state);
	cat_unref_ptr(priv->lookahead);
	G_OBJECT_CLASS(grorun_stack_entry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_stack_entry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

GroRunStackEntry *grorun_stack_entry_new(GroRunStackEntry *parent, GroRunState *state, GroRunIToken *lookahead) {
	GroRunStackEntry *result = g_object_new(GRORUN_TYPE_STACK_ENTRY, NULL);
	cat_ref_anounce(result);
	GroRunStackEntryPrivate *priv = grorun_stack_entry_get_instance_private(result);
	priv->parent = cat_ref_ptr(parent);
	priv->state = cat_ref_ptr(state);
	priv->lookahead = cat_ref_ptr(lookahead);
	return result;
}

GroRunTransitionsForSymbol *grorun_stack_entry_get_transitions_for_symbol(GroRunStackEntry *entry) {
	GroRunStackEntryPrivate *priv = grorun_stack_entry_get_instance_private(entry);
	return grorun_state_get_transitions_for_symbol(priv->state, grorun_itoken_get_symbol(priv->lookahead));
}

GroRunTransitionsForSymbol *grorun_stack_entry_get_transitions_for_symbols(GroRunStackEntry *entry, GroRunSymbol *symbol) {
	GroRunStackEntryPrivate *priv = grorun_stack_entry_get_instance_private(entry);
	return grorun_state_get_transitions_for_symbol(priv->state, symbol);
}


GroRunStackEntry *grorun_stack_entry_get_parent(GroRunStackEntry *entry) {
	GroRunStackEntryPrivate *priv = grorun_stack_entry_get_instance_private(entry);
	return priv->parent;
}


GroRunIToken *grorun_stack_entry_get_lookahead(GroRunStackEntry *entry) {
	GroRunStackEntryPrivate *priv = grorun_stack_entry_get_instance_private(entry);
	return priv->lookahead;
}

GroRunState *grorun_stack_entry_get_state(GroRunStackEntry *entry) {
	GroRunStackEntryPrivate *priv = grorun_stack_entry_get_instance_private(entry);
	return priv->state;
}

void *grorun_stack_entry_get_value(GroRunStackEntry *entry) {
	if (entry==NULL) {
		return NULL;
	}
	GroRunStackEntryPrivate *priv = grorun_stack_entry_get_instance_private(entry);
	if (priv->lookahead==NULL) {
		return NULL;
	}

	return grorun_itoken_get_value(priv->lookahead);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunStackEntry *instance = GRORUN_STACK_ENTRY(self);
	GroRunStackEntryPrivate *priv = grorun_stack_entry_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p %O %O]", iname, self, priv->state, priv->lookahead);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
