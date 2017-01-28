
/*
   File:    groterminalset.c
   Project: gross
   Author:  Douwe Vos
   Date:    May 31, 2016
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

#include "groterminalset.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroTerminalSet"
#include <logging/catlog.h>

struct _GroTerminalSetPrivate {
	CatHashSet *all;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroTerminalSet, gro_terminal_set, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroTerminalSet)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_terminal_set_class_init(GroTerminalSetClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_terminal_set_init(GroTerminalSet *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroTerminalSet *instance = GRO_TERMINAL_SET(object);
//	GroTerminalSetPrivate *priv = gro_terminal_set_get_instance_private(instance);
	G_OBJECT_CLASS(gro_terminal_set_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_terminal_set_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroTerminalSet *gro_terminal_set_new() {
	GroTerminalSet *result = g_object_new(GRO_TYPE_TERMINAL_SET, NULL);
	cat_ref_anounce(result);
	GroTerminalSetPrivate *priv = gro_terminal_set_get_instance_private(result);
	priv->all = cat_hash_set_new((GHashFunc) gro_m_symbol_hash, (GEqualFunc) gro_m_symbol_equal);
	return result;
}

GroTerminalSet *gro_terminal_set_clone(const GroTerminalSet *src) {
	GroTerminalSet *result = g_object_new(GRO_TYPE_TERMINAL_SET, NULL);
	cat_ref_anounce(result);
	GroTerminalSetPrivate *priv = gro_terminal_set_get_instance_private(result);
	priv->all = cat_hash_set_new((GHashFunc) gro_m_symbol_hash, (GEqualFunc) gro_m_symbol_equal);
	const GroTerminalSetPrivate *priv_src = gro_terminal_set_get_instance_private((GroTerminalSet *) src);
	cat_hash_set_add_all(priv->all, priv_src->all);
	return result;
}


gboolean gro_terminal_set_add(GroTerminalSet *terminal_set, GroMTerminal *terminal) {
	GroTerminalSetPrivate *priv = gro_terminal_set_get_instance_private(terminal_set);
	GroMTerminal *other = (GroMTerminal *) cat_hash_set_add(priv->all, (GObject *) terminal);
	return other==NULL;
}

gboolean gro_terminal_set_add_all(GroTerminalSet *terminal_set, GroTerminalSet *to_add) {
	GroTerminalSetPrivate *priv = gro_terminal_set_get_instance_private(terminal_set);
	GroTerminalSetPrivate *priv_a = gro_terminal_set_get_instance_private(to_add);
	return cat_hash_set_add_all(priv->all, priv_a->all);
}

gboolean gro_terminal_set_is_empty(GroTerminalSet *terminal_set) {
	GroTerminalSetPrivate *priv = gro_terminal_set_get_instance_private(terminal_set);
	return cat_hash_set_size(priv->all)==0;
}

gboolean gro_terminal_set_contains(const GroTerminalSet *terminal_set, GroMTerminal *terminal) {
	GroTerminalSetPrivate *priv = gro_terminal_set_get_instance_private((GroTerminalSet *) terminal_set);
	return cat_hash_set_get(priv->all, (GObject *) terminal)!=NULL;
}

GroMTerminalIterator *gro_terminal_set_iterator(GroTerminalSet *terminal_set) {
	GroTerminalSetPrivate *priv = gro_terminal_set_get_instance_private(terminal_set);
	return (GroMTerminalIterator *) cat_hash_set_iterator(priv->all);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroTerminalSet *instance = GRO_TERMINAL_SET(self);
	GroTerminalSetPrivate *priv = gro_terminal_set_get_instance_private(instance);

//	cat_string_wo_format(append_to, "%s[%p", iname, self);
	cat_string_wo_format(append_to, "{");
	gboolean is_first = TRUE;
	CatIIterator *iter = cat_hash_set_iterator(priv->all);
	while(cat_iiterator_has_next(iter)) {
		GroMTerminal *term = (GroMTerminal *) cat_iiterator_next(iter);
		if (is_first) {
			is_first = FALSE;
		} else {
			cat_string_wo_append_chars(append_to, " ");
		}
		cat_string_wo_append(append_to, gro_m_symbol_get_name((GroMSymbol *) term));
	}
	cat_unref_ptr(iter);
	cat_string_wo_append_chars(append_to, "}");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
