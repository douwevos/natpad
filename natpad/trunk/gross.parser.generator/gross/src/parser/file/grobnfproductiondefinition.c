/*
   File:    grobnfproductiondefinition.c
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

#include "grobnfproductiondefinition.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfProductionDefinition"
#include <logging/catlog.h>

struct _GroBnfProductionDefinitionPrivate {
	GroBnfString *lhs_id;
	GroBnfRhsRuleList *rhs_rule_list;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfProductionDefinition, gro_bnf_production_definition, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfProductionDefinition)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_production_definition_class_init(GroBnfProductionDefinitionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_production_definition_init(GroBnfProductionDefinition *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroBnfProductionDefinition *instance = GRO_BNF_PRODUCTION_DEFINITION(object);
	GroBnfProductionDefinitionPrivate *priv = gro_bnf_production_definition_get_instance_private(instance);
	cat_ref_ptr(priv->lhs_id);
	cat_ref_ptr(priv->rhs_rule_list);
	G_OBJECT_CLASS(gro_bnf_production_definition_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_production_definition_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroBnfProductionDefinition *gro_bnf_production_definition_new(GroBnfString *lhs_id, GroBnfRhsRuleList *rhs_rule_list) {
	GroBnfProductionDefinition *result = g_object_new(GRO_TYPE_BNF_PRODUCTION_DEFINITION, NULL);
	cat_ref_anounce(result);
	GroBnfProductionDefinitionPrivate *priv = gro_bnf_production_definition_get_instance_private(result);
	priv->lhs_id = cat_ref_ptr(lhs_id);
	priv->rhs_rule_list = cat_ref_ptr(rhs_rule_list);
	return result;
}

GroBnfString *gro_bnf_production_definition_get_lhs_id(GroBnfProductionDefinition *definition) {
	GroBnfProductionDefinitionPrivate *priv = gro_bnf_production_definition_get_instance_private(definition);
	return priv->lhs_id;
}

GroBnfRhsRuleList *gro_bnf_production_definition_get_rhs_rule_list(GroBnfProductionDefinition *definition) {
	GroBnfProductionDefinitionPrivate *priv = gro_bnf_production_definition_get_instance_private(definition);
	return priv->rhs_rule_list;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroBnfProductionDefinition *instance = GRO_BNF_PRODUCTION_DEFINITION(self);
	GroBnfProductionDefinitionPrivate *priv = gro_bnf_production_definition_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
