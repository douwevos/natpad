/*
   File:    jagdecvaluetypedefinition.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 6, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "jagdecvaluetypedefinition.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecValueTypeDefinition"
#include <logging/catlog.h>

struct _JagDecValueTypeDefinitionPrivate {
	JagAstDeclarationType *typeDefinition;
	JagDecValueTypeDefinitionKind kind;
	gboolean is_include;
};

G_DEFINE_TYPE (JagDecValueTypeDefinition, jag_dec_value_type_definition, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_value_type_definition_class_init(JagDecValueTypeDefinitionClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecValueTypeDefinitionPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_value_type_definition_init(JagDecValueTypeDefinition *instance) {
	JagDecValueTypeDefinitionPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_VALUE_TYPE_DEFINITION, JagDecValueTypeDefinitionPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecValueTypeDefinition *instance = JAG_DEC_VALUE_TYPE_DEFINITION(object);
	JagDecValueTypeDefinitionPrivate *priv = instance->priv;
	cat_unref_ptr(priv->typeDefinition);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecValueTypeDefinition *jag_dec_value_type_definition_new(JagAstDeclarationType *typeDefinition, JagDecValueTypeDefinitionKind kind) {
	JagDecValueTypeDefinition *result = g_object_new(JAG_DEC_TYPE_VALUE_TYPE_DEFINITION, NULL);
	cat_ref_anounce(result);
	JagDecValueTypeDefinitionPrivate *priv = result->priv;
	priv->typeDefinition = cat_ref_ptr(typeDefinition);
	priv->kind = kind;
	priv->is_include = TRUE;
	return result;
}

JagDecValueTypeDefinition *jag_dec_value_type_definition_clone(JagDecValueTypeDefinition *source) {
	JagDecValueTypeDefinition *result = g_object_new(JAG_DEC_TYPE_VALUE_TYPE_DEFINITION, NULL);
	JagDecValueTypeDefinitionPrivate *source_priv = JAG_DEC_VALUE_TYPE_DEFINITION_GET_PRIVATE(source);
	cat_ref_anounce(result);
	JagDecValueTypeDefinitionPrivate *priv = result->priv;
	priv->typeDefinition = cat_ref_ptr(source_priv->typeDefinition);
	priv->kind = source_priv->kind;
	priv->is_include = source_priv->is_include;
	return result;
}


JagDecValueTypeDefinitionKind jag_dec_value_type_definition_get_kind(JagDecValueTypeDefinition *definition) {
	return JAG_DEC_VALUE_TYPE_DEFINITION_GET_PRIVATE(definition)->kind;
}

gboolean jag_dec_value_type_definition_is_included(JagDecValueTypeDefinition *definition) {
	return JAG_DEC_VALUE_TYPE_DEFINITION_GET_PRIVATE(definition)->is_include;
}

void jag_dec_value_type_definition_set_included(JagDecValueTypeDefinition *definition, gboolean included_val) {
	JAG_DEC_VALUE_TYPE_DEFINITION_GET_PRIVATE(definition)->is_include = included_val;
}


JagAstDeclarationType *jag_dec_value_type_definition_get_declaration_type(JagDecValueTypeDefinition *definition) {
	return JAG_DEC_VALUE_TYPE_DEFINITION_GET_PRIVATE(definition)->typeDefinition;
}


int jag_dec_value_type_definition_hash(JagDecValueTypeDefinition *definition) {
	JagDecValueTypeDefinitionPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_GET_PRIVATE(definition);
	return jag_ast_declaration_type_hash_code(priv->typeDefinition)+7*priv->kind;
}



gboolean jag_dec_value_type_definition_equal(JagDecValueTypeDefinition *definition_a, JagDecValueTypeDefinition *definition_b) {
	if (definition_a==definition_b) {
		return TRUE;
	}
	if (definition_a==NULL || definition_b==NULL) {
		return FALSE;
	}
	JagDecValueTypeDefinitionPrivate *priv_a = JAG_DEC_VALUE_TYPE_DEFINITION_GET_PRIVATE(definition_a);
	JagDecValueTypeDefinitionPrivate *priv_b = JAG_DEC_VALUE_TYPE_DEFINITION_GET_PRIVATE(definition_b);
	return (jag_ast_declaration_type_equal(priv_a->typeDefinition, priv_b->typeDefinition)) && priv_a->kind==priv_b->kind;
}

int jag_dec_value_type_definition_kind_get_prio(JagDecValueTypeDefinitionKind kind) {
	switch(kind) {
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CATCHED_EXCEPTION : return 100;
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_ARGUMENT : return 100;
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_LOCALVALUETABLE : return 100;
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONDITION : return 90;
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_NULL : return 85;
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT : return 80;
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_NEW : return 70;
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_SUPER : return 60;
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_THIS : return 65;
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_RETURN : return 50;
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CAST : return 30;
		case JAG_DEC_VALUE_TYPE_DEFINITION_KIND_INSTANCEOF : return 30;
	}
	return 0;
}

