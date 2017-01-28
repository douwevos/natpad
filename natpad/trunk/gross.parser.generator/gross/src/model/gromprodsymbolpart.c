
/*
   File:    gromprodsymbolpart.c
   Project: gross
   Author:  Douwe Vos
   Date:    May 26, 2016
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

#include "gromprodsymbolpart.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroMProdSymbolPart"
#include <logging/catlog.h>

struct _GroMProdSymbolPartPrivate {
	GroMSymbol *symbol;
	GroBnfString *label;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroMProdSymbolPart, gro_m_prod_symbol_part, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroMProdSymbolPart)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_m_prod_symbol_part_class_init(GroMProdSymbolPartClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_m_prod_symbol_part_init(GroMProdSymbolPart *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroMProdSymbolPart *instance = GRO_M_PROD_SYMBOL_PART(object);
//	GroMProdSymbolPartPrivate *priv = gro_m_prod_symbol_part_get_instance_private(instance);
	G_OBJECT_CLASS(gro_m_prod_symbol_part_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_m_prod_symbol_part_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroMProdSymbolPart *gro_m_prod_symbol_part_new(GroMSymbol *symbol, GroBnfString *label) {
	GroMProdSymbolPart *result = g_object_new(GRO_TYPE_M_PROD_SYMBOL_PART, NULL);
	cat_ref_anounce(result);
	GroMProdSymbolPartPrivate *priv = gro_m_prod_symbol_part_get_instance_private(result);
	priv->symbol = cat_ref_ptr(symbol);
	priv->label = cat_ref_ptr(label);
	return result;
}

GroMSymbol *gro_m_prod_symbol_part_get_symbol(GroMProdSymbolPart *part) {
	GroMProdSymbolPartPrivate *priv = gro_m_prod_symbol_part_get_instance_private(part);
	return priv->symbol;
}

GroBnfString *gro_m_prod_symbol_part_get_label(GroMProdSymbolPart *part) {
	GroMProdSymbolPartPrivate *priv = gro_m_prod_symbol_part_get_instance_private(part);
	return priv->label;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroMProdSymbolPart *instance = GRO_M_PROD_SYMBOL_PART(self);
	GroMProdSymbolPartPrivate *priv = gro_m_prod_symbol_part_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%O]", iname, priv->symbol);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
