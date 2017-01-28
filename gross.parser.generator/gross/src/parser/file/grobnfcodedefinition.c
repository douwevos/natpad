/*
   File:    grobnfcodedefinition.c
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

#include "grobnfcodedefinition.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfCodeDefinition"
#include <logging/catlog.h>

struct _GroBnfCodeDefinitionPrivate {
	int id;
	GroBnfProductionRuleReference *prod_rule_ref;
	GroBnfString *code_text;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfCodeDefinition, gro_bnf_code_definition, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfCodeDefinition)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_code_definition_class_init(GroBnfCodeDefinitionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_code_definition_init(GroBnfCodeDefinition *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroBnfCodeDefinition *instance = GRO_BNF_CODE_DEFINITION(object);
	GroBnfCodeDefinitionPrivate *priv = gro_bnf_code_definition_get_instance_private(instance);
	cat_unref_ptr(priv->prod_rule_ref);
	cat_unref_ptr(priv->code_text);
	G_OBJECT_CLASS(gro_bnf_code_definition_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_code_definition_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

GroBnfCodeDefinition *gro_bnf_code_definition_new(int id, GroBnfProductionRuleReference *prod_rule_ref, GroBnfString *code_text) {
	GroBnfCodeDefinition *result = g_object_new(GRO_TYPE_BNF_CODE_DEFINITION, NULL);
	cat_ref_anounce(result);
	GroBnfCodeDefinitionPrivate *priv = gro_bnf_code_definition_get_instance_private(result);
	priv->id = id;
	priv->prod_rule_ref = cat_ref_ptr(prod_rule_ref);
	priv->code_text = cat_ref_ptr(code_text);
	return result;
}

GroBnfProductionRuleReference *gro_bnf_code_definition_get_rule_ref(GroBnfCodeDefinition *code_definition) {
	GroBnfCodeDefinitionPrivate *priv = gro_bnf_code_definition_get_instance_private(code_definition);
	return priv->prod_rule_ref;
}

GroBnfString *gro_bnf_code_definition_get_code_text(GroBnfCodeDefinition *code_definition) {
	GroBnfCodeDefinitionPrivate *priv = gro_bnf_code_definition_get_instance_private(code_definition);
	return priv->code_text;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroBnfCodeDefinition *instance = GRO_BNF_CODE_DEFINITION(self);
	GroBnfCodeDefinitionPrivate *priv = gro_bnf_code_definition_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
