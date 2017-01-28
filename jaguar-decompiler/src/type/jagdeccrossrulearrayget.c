/*
   File:    jagdeccrossrulearrayget.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 14, 2012
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

#include "jagdeccrossrulearrayget.h"
#include "jagdecicrossrule.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecCrossRuleArrayGet"
#include <logging/catlog.h>

struct _JagDecCrossRuleArrayGetPrivate {
	JagDecValueTypeDefinitionList *array_type_set;
	JagDecValueTypeDefinitionList *result_type_set;
};

static void l_cross_rule_iface_init(JagDecICrossRuleInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecCrossRuleArrayGet, jag_dec_cross_rule_array_get, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ICROSS_RULE, l_cross_rule_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_array_weak_notify(gpointer data, GObject *where_the_object_was);
static void l_result_weak_notify(gpointer data, GObject *where_the_object_was);

static void jag_dec_cross_rule_array_get_class_init(JagDecCrossRuleArrayGetClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecCrossRuleArrayGetPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_cross_rule_array_get_init(JagDecCrossRuleArrayGet *instance) {
	JagDecCrossRuleArrayGetPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_CROSS_RULE_ARRAY_GET, JagDecCrossRuleArrayGetPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecCrossRuleArrayGet *instance = JAG_DEC_CROSS_RULE_ARRAY_GET(object);
	JagDecCrossRuleArrayGetPrivate *priv = instance->priv;
	if (priv->array_type_set) {
		g_object_weak_unref((GObject *) priv->array_type_set, (GWeakNotify) l_array_weak_notify, instance);
		priv->array_type_set = NULL;
	}
	if (priv->result_type_set) {
		g_object_weak_unref((GObject *) priv->result_type_set, (GWeakNotify) l_result_weak_notify, instance);
		priv->result_type_set = NULL;
	}
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_array_weak_notify(gpointer data, GObject *where_the_object_was) {
	JagDecCrossRuleArrayGetPrivate *priv = JAG_DEC_CROSS_RULE_ARRAY_GET_GET_PRIVATE(data);
	priv->array_type_set = NULL;
}

static void l_result_weak_notify(gpointer data, GObject *where_the_object_was) {
	JagDecCrossRuleArrayGetPrivate *priv = JAG_DEC_CROSS_RULE_ARRAY_GET_GET_PRIVATE(data);
	priv->result_type_set = NULL;
}


JagDecCrossRuleArrayGet *jag_dec_cross_rule_array_get_new(JagDecValueTypeDefinitionList *array_type_set, JagDecValueTypeDefinitionList *result_type_set) {
	JagDecCrossRuleArrayGet *result = g_object_new(JAG_DEC_TYPE_CROSS_RULE_ARRAY_GET, NULL);
	cat_ref_anounce(result);
	JagDecCrossRuleArrayGetPrivate *priv = result->priv;
	g_object_weak_ref((GObject *) array_type_set, (GWeakNotify) l_array_weak_notify, result);
	priv->array_type_set = array_type_set;
	g_object_weak_ref((GObject *) result_type_set, (GWeakNotify) l_result_weak_notify, result);
	priv->result_type_set = result_type_set;
	return result;
}








/********************* start JagDecICrossRule implementation *********************/

static gboolean l_cross_rule_apply(JagDecICrossRule *self) {
	JagDecCrossRuleArrayGetPrivate *priv = JAG_DEC_CROSS_RULE_ARRAY_GET_GET_PRIVATE(self);
	JagDecValueTypeDefinitionList *array_type_set = cat_ref_ptr(priv->array_type_set);
	JagDecValueTypeDefinitionList *result_type_set = cat_ref_ptr(priv->result_type_set);
	gboolean result = TRUE;
	if (array_type_set!=NULL && result_type_set!=NULL) {
		CatIIterator *iter = jag_dec_value_type_definition_list_iterator(array_type_set);
		while(cat_iiterator_has_next(iter)) {
			JagDecValueTypeDefinition *valTypeDef = (JagDecValueTypeDefinition *) cat_iiterator_next(iter);
			JagAstDeclarationType *arrayTypeDef = jag_dec_value_type_definition_get_declaration_type(valTypeDef);
			arrayTypeDef = jag_ast_declaration_type_set_dim_count(arrayTypeDef, jag_ast_declaration_type_get_dim_count(arrayTypeDef)-1);
			JagDecValueTypeDefinition *mainValTypeDef = NULL;

			JagDecValueTypeDefinitionKind val_type_def_kind = jag_dec_value_type_definition_get_kind(valTypeDef);
			CatIIterator *miter = jag_dec_value_type_definition_list_iterator(result_type_set);
			while(cat_iiterator_has_next(miter)) {
				JagDecValueTypeDefinition *testTypeDef = (JagDecValueTypeDefinition *) cat_iiterator_next(miter);
				JagAstDeclarationType *test_decl_type = jag_dec_value_type_definition_get_declaration_type(testTypeDef);
				JagDecValueTypeDefinitionKind test_type_def_kind = jag_dec_value_type_definition_get_kind(valTypeDef);
				if (jag_ast_declaration_type_equal(test_decl_type, arrayTypeDef) && test_type_def_kind == val_type_def_kind) {
					mainValTypeDef = testTypeDef;
					break;
				}
			}
			cat_unref_ptr(miter);

			if (jag_dec_value_type_definition_is_included(valTypeDef)) {
				if (mainValTypeDef==NULL) {
					mainValTypeDef = jag_dec_value_type_definition_new(arrayTypeDef, val_type_def_kind);
					jag_dec_value_type_definition_list_add_definition(result_type_set, mainValTypeDef);
					cat_unref(mainValTypeDef);
				}
			} else {
				if (mainValTypeDef!=NULL) {
					jag_dec_value_type_definition_set_included(mainValTypeDef, FALSE);
				}
			}
			cat_unref(arrayTypeDef);
		}
		cat_unref_ptr(iter);
		result = jag_dec_value_type_definition_list_all_cross_rules_applied(array_type_set);
	}
	cat_unref_ptr(array_type_set);
	cat_unref_ptr(result_type_set);
	return result;
}

static void l_cross_rule_iface_init(JagDecICrossRuleInterface *iface) {
	iface->apply = l_cross_rule_apply;
}

/********************* end JagDecICrossRule implementation *********************/
