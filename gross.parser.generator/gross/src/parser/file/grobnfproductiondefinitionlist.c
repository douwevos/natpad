/*
   File:    grobnfproductiondefinitionlist.c
   Project: gross
   Author:  Douwe Vos
   Date:    Nov 17, 2016
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

#include "grobnfproductiondefinitionlist.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfProductionDefinitionList"
#include <logging/catlog.h>

struct _GroBnfProductionDefinitionListPrivate {
	GroBnfProductionDefinitionList *linked;
	GroBnfProductionDefinition *prod_definition;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfProductionDefinitionList, gro_bnf_production_definition_list, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfProductionDefinitionList)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_production_definition_list_class_init(GroBnfProductionDefinitionListClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_production_definition_list_init(GroBnfProductionDefinitionList *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroBnfProductionDefinitionList *instance = GRO_BNF_PRODUCTION_DEFINITION_LIST(object);
	GroBnfProductionDefinitionListPrivate *priv = gro_bnf_production_definition_list_get_instance_private(instance);
	cat_unref_ptr(priv->linked);
	cat_unref_ptr(priv->prod_definition);
	G_OBJECT_CLASS(gro_bnf_production_definition_list_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_production_definition_list_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

GroBnfProductionDefinitionList *gro_bnf_production_definition_list_new(GroBnfProductionDefinitionList *plist, GroBnfProductionDefinition *prod_definition) {
	GroBnfProductionDefinitionList *result = g_object_new(GRO_TYPE_BNF_PRODUCTION_DEFINITION_LIST, NULL);
	cat_ref_anounce(result);
	GroBnfProductionDefinitionListPrivate *priv = gro_bnf_production_definition_list_get_instance_private(result);
	priv->linked = cat_ref_ptr(plist);
	priv->prod_definition = cat_ref_ptr(prod_definition);
	return result;
}

CatArrayWo *gro_bnf_production_definition_list_enlist(GroBnfProductionDefinitionList *list) {
	CatArrayWo *result = cat_array_wo_new();
	GroBnfProductionDefinitionList *tdl = list;
	while(tdl!=NULL) {
		GroBnfProductionDefinitionListPrivate *priv = gro_bnf_production_definition_list_get_instance_private(tdl);
		cat_array_wo_append(result, (GObject *) priv->prod_definition);
		tdl = priv->linked;
	}
	cat_array_wo_reverse(result);
	return result;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroBnfProductionDefinitionList *instance = GRO_BNF_PRODUCTION_DEFINITION_LIST(self);
//	GroBnfProductionDefinitionListPrivate *priv = gro_bnf_production_definition_list_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
