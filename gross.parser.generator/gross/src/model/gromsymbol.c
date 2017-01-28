
/*
   File:    gromsymbol.c
   Project: gross
   Author:  Douwe Vos
   Date:    May 25, 2016
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

#include "gromsymbol.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroMSymbol"
#include <logging/catlog.h>

struct _GroMSymbolPrivate {
	int index;
	GroBnfString *name;
	CatStringWo *stack_type;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroMSymbol, gro_m_symbol, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroMSymbol)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_m_symbol_class_init(GroMSymbolClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_m_symbol_init(GroMSymbol *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroMSymbol *instance = GRO_M_SYMBOL(object);
	GroMSymbolPrivate *priv = gro_m_symbol_get_instance_private(instance);
	cat_unref_ptr(priv->name);
	cat_unref_ptr(priv->stack_type);
	G_OBJECT_CLASS(gro_m_symbol_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_m_symbol_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void gro_m_symbol_construct(GroMSymbol *symbol, int index, GroBnfString *name) {
	GroMSymbolPrivate *priv = gro_m_symbol_get_instance_private(symbol);
	priv->index = index;
	priv->name = cat_ref_ptr(name);
	priv->stack_type = NULL;
}

CatStringWo *gro_m_symbol_get_name(GroMSymbol *symbol) {
	GroMSymbolPrivate *priv = gro_m_symbol_get_instance_private(symbol);
	return gro_bnf_string_get_text(priv->name);
}

GroBnfString *gro_m_symbol_get_string(GroMSymbol *symbol) {
	GroMSymbolPrivate *priv = gro_m_symbol_get_instance_private(symbol);
	return priv->name;
}

int gro_m_symbol_get_index(GroMSymbol *symbol) {
	GroMSymbolPrivate *priv = gro_m_symbol_get_instance_private(symbol);
	return priv->index;
}

void gro_m_symbol_set_stack_type(GroMSymbol *symbol, CatStringWo *stack_type) {
	GroMSymbolPrivate *priv = gro_m_symbol_get_instance_private(symbol);
	priv->stack_type = cat_string_wo_clone(stack_type, CAT_CLONE_DEPTH_AS_ANCHORED);
}

CatStringWo *gro_m_symbol_get_stack_type(GroMSymbol *symbol) {
	GroMSymbolPrivate *priv = gro_m_symbol_get_instance_private(symbol);
	return priv->stack_type;
}


gboolean gro_m_symbol_equal(const GroMSymbol *symbol_a, const GroMSymbol *symbol_b) {
	if (symbol_a==symbol_b) {
		return TRUE;
	}
	if (symbol_a==NULL || symbol_b==NULL) {
		return FALSE;
	}

	if (GRO_M_SYMBOL_GET_CLASS(symbol_a)!=GRO_M_SYMBOL_GET_CLASS(symbol_b)) {
		return FALSE;
	}

	CatStringWo *str_a = gro_m_symbol_get_name((GroMSymbol *) symbol_a);
	CatStringWo *str_b = gro_m_symbol_get_name((GroMSymbol *) symbol_b);
	return cat_string_wo_equal(str_a, str_b);
}

int gro_m_symbol_hash(GroMSymbol *symbol) {
	GroMSymbolPrivate *priv = gro_m_symbol_get_instance_private(symbol);
	return gro_bnf_string_hash(priv->name);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroMSymbol *instance = GRO_M_SYMBOL(self);
	GroMSymbolPrivate *priv = gro_m_symbol_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, idx=%d, '%O']", iname, self, priv->index, gro_bnf_string_get_text(priv->name));
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
