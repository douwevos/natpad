/*
   File:    jagdeccrossrulematchforcondition.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 28, 2012
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

#include "jagdeccrossrulematchforcondition.h"
#include "jagdecicrossrule.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecCrossRuleMatchForCondition"
#include <logging/catlog.h>

struct _JagDecCrossRuleMatchForConditionPrivate {
	JagDecValueTypeDefinitionList *type_set_a;
	JagDecValueTypeDefinitionList *type_set_b;
};

static void l_cross_rule_iface_init(JagDecICrossRuleInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecCrossRuleMatchForCondition, jag_dec_cross_rule_match_for_condition, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_ICROSS_RULE, l_cross_rule_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_seta_weak_notify(gpointer data, GObject *where_the_object_was);
static void l_setb_weak_notify(gpointer data, GObject *where_the_object_was);

static void jag_dec_cross_rule_match_for_condition_class_init(JagDecCrossRuleMatchForConditionClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecCrossRuleMatchForConditionPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_cross_rule_match_for_condition_init(JagDecCrossRuleMatchForCondition *instance) {
	JagDecCrossRuleMatchForConditionPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_CROSS_RULE_MATCH_FOR_CONDITION, JagDecCrossRuleMatchForConditionPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecCrossRuleMatchForCondition *instance = JAG_DEC_CROSS_RULE_MATCH_FOR_CONDITION(object);
	JagDecCrossRuleMatchForConditionPrivate *priv = instance->priv;
	if (priv->type_set_a) {
		g_object_weak_unref((GObject *) priv->type_set_a, (GWeakNotify) l_seta_weak_notify, instance);
		priv->type_set_a = NULL;
	}
	if (priv->type_set_b) {
		g_object_weak_unref((GObject *) priv->type_set_b, (GWeakNotify) l_setb_weak_notify, instance);
		priv->type_set_b = NULL;
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


JagDecCrossRuleMatchForCondition *jag_dec_cross_rule_match_for_condition_new(JagDecValueTypeDefinitionList *type_set_a, JagDecValueTypeDefinitionList *type_set_b) {
	JagDecCrossRuleMatchForCondition *result = g_object_new(JAG_DEC_TYPE_CROSS_RULE_MATCH_FOR_CONDITION, NULL);
	cat_ref_anounce(result);
	JagDecCrossRuleMatchForConditionPrivate *priv = result->priv;
	g_object_weak_ref((GObject *) type_set_a, (GWeakNotify) l_seta_weak_notify, result);
	priv->type_set_a = type_set_a;
	g_object_weak_ref((GObject *) type_set_b, (GWeakNotify) l_setb_weak_notify, result);
	priv->type_set_b = type_set_b;
	return result;
}







static void l_seta_weak_notify(gpointer data, GObject *where_the_object_was) {
	JagDecCrossRuleMatchForConditionPrivate *priv = JAG_DEC_CROSS_RULE_MATCH_FOR_CONDITION_GET_PRIVATE(data);
	priv->type_set_a = NULL;
}

static void l_setb_weak_notify(gpointer data, GObject *where_the_object_was) {
	JagDecCrossRuleMatchForConditionPrivate *priv = JAG_DEC_CROSS_RULE_MATCH_FOR_CONDITION_GET_PRIVATE(data);
	priv->type_set_b = NULL;
}





/********************* start JagDecICrossRule implementation *********************/

static void l_copy_set(JagDecValueTypeDefinitionList *source, JagDecValueTypeDefinitionList *destination) {
	CatIIterator *iter = jag_dec_value_type_definition_list_iterator(source);
	while(cat_iiterator_has_next(iter)) {
		JagDecValueTypeDefinition *valTypeDef = (JagDecValueTypeDefinition *) cat_iiterator_next(iter);
		if (jag_dec_value_type_definition_is_included(valTypeDef)) {
			JagDecValueTypeDefinition *cloned = jag_dec_value_type_definition_clone(valTypeDef);
			jag_dec_value_type_definition_list_add_definition(destination, cloned);
			cat_unref_ptr(cloned);
		}
	}
	cat_unref_ptr(iter);
}

static gboolean l_cross_rule_apply(JagDecICrossRule *self) {
	JagDecCrossRuleMatchForConditionPrivate *priv = JAG_DEC_CROSS_RULE_MATCH_FOR_CONDITION_GET_PRIVATE(self);
	JagDecValueTypeDefinitionList *seta = cat_ref_ptr(priv->type_set_a);
	JagDecValueTypeDefinitionList *setb = cat_ref_ptr(priv->type_set_b);
	gboolean result = TRUE;

	if (seta!=NULL && setb!=NULL) {
		result = (jag_dec_value_type_definition_list_cross_rules_running(seta) || jag_dec_value_type_definition_list_all_cross_rules_applied(seta)) &&
			(jag_dec_value_type_definition_list_cross_rules_running(setb) || jag_dec_value_type_definition_list_all_cross_rules_applied(setb));

		if (jag_dec_value_type_definition_list_is_empty(seta)) {
			if (!jag_dec_value_type_definition_list_is_empty(setb)) {
				l_copy_set(setb, seta);
			}
		} else if (jag_dec_value_type_definition_list_is_empty(setb)) {
			l_copy_set(seta, setb);
		} else {
			CatHashSet *iseta = jag_dec_value_type_definition_list_enlist_included(seta);
			CatHashSet *isetb = jag_dec_value_type_definition_list_enlist_included(setb);

			cat_hash_set_intersection(iseta, isetb);
			if (cat_hash_set_size(iseta)==0) {
				cat_log_warn("result type-set would be empty");
			} else {
				jag_dec_value_type_definition_list_exclude_set(seta, iseta, TRUE);
				jag_dec_value_type_definition_list_exclude_set(setb, iseta, TRUE);
			}
			cat_unref_ptr(iseta);
			cat_unref_ptr(isetb);
		}
	}
	cat_unref_ptr(seta);
	cat_unref_ptr(setb);
	return result;
}

static void l_cross_rule_iface_init(JagDecICrossRuleInterface *iface) {
	iface->apply = l_cross_rule_apply;
}

/********************* end JagDecICrossRule implementation *********************/
