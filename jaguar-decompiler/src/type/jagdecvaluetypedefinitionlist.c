/*
   File:    jagdecvaluetypedefinitionlist.c
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

#include "jagdecvaluetypedefinitionlist.h"
#include <jaguarbytecode.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecValueTypeDefinitionList"
#include <logging/catlog.h>

enum _JagDecCrossRuleState {
	JAG_DEC_CROSS_RULE_STATE_UNRESOLVED,
	JAG_DEC_CROSS_RULE_STATE_RUNNING,
	JAG_DEC_CROSS_RULE_STATE_RESOLVED
};

enum _JagDecSpecial {
	JAG_DEC_SPECIAL_NORMAL,
	JAG_DEC_SPECIAL_HIDDEN
};

typedef enum _JagDecCrossRuleState JagDecCrossRuleState;
typedef enum _JagDecSpecial JagDecSpecial;

struct _JagDecValueTypeDefinitionListPrivate {
	CatArrayWo *e_list;
	CatArrayWo *e_cross_rules;
	JagDecCrossRuleState crossRulesState;
	JagDecSpecial special;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecValueTypeDefinitionList, jag_dec_value_type_definition_list, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_value_type_definition_list_class_init(JagDecValueTypeDefinitionListClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecValueTypeDefinitionListPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_value_type_definition_list_init(JagDecValueTypeDefinitionList *instance) {
	JagDecValueTypeDefinitionListPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_VALUE_TYPE_DEFINITION_LIST, JagDecValueTypeDefinitionListPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecValueTypeDefinitionList *instance = JAG_DEC_VALUE_TYPE_DEFINITION_LIST(object);
	JagDecValueTypeDefinitionListPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_list);
	cat_unref_ptr(priv->e_cross_rules);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void jag_dec_value_type_definition_list_construct(JagDecValueTypeDefinitionList *value_type_definition_list) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(value_type_definition_list);
	priv->e_list = cat_array_wo_new();
	priv->e_cross_rules = cat_array_wo_new();
	priv->crossRulesState = JAG_DEC_CROSS_RULE_STATE_UNRESOLVED;
	priv->special = JAG_DEC_SPECIAL_NORMAL;
}


JagDecValueTypeDefinitionList *jag_dec_value_type_definition_list_new() {
	JagDecValueTypeDefinitionList *result = g_object_new(JAG_DEC_TYPE_VALUE_TYPE_DEFINITION_LIST, NULL);
	cat_ref_anounce(result);
	jag_dec_value_type_definition_list_construct(result);
	return result;
}

JagDecValueTypeDefinitionList *jag_dec_value_type_definition_list_new_hidden_exception() {
	JagDecValueTypeDefinitionList *result = g_object_new(JAG_DEC_TYPE_VALUE_TYPE_DEFINITION_LIST, NULL);
	cat_ref_anounce(result);
	jag_dec_value_type_definition_list_construct(result);
	result->priv->special = JAG_DEC_SPECIAL_HIDDEN;
	return result;
}


int jag_dec_value_type_definition_list_hash(JagDecValueTypeDefinitionList *list) {
	return (int) ((long) list);
}

gboolean jag_dec_value_type_definition_list_equal(JagDecValueTypeDefinitionList *list_a, JagDecValueTypeDefinitionList *list_b) {
	return list_a==list_b;
}


void jag_dec_value_type_definition_list_add_definition(JagDecValueTypeDefinitionList *list, JagDecValueTypeDefinition *definition) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);
	cat_array_wo_append(priv->e_list, (GObject *) definition);
}

CatIIterator *jag_dec_value_type_definition_list_iterator(JagDecValueTypeDefinitionList *list) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);
	return cat_array_wo_iterator(priv->e_list);
}


gboolean jag_dec_value_type_definition_list_is_empty(JagDecValueTypeDefinitionList *list) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);
	return cat_array_wo_size(priv->e_list)==0;
}


void jag_dec_value_type_definition_list_add_basic(JagDecValueTypeDefinitionList *list, JagBytType primitive_type, JagDecValueTypeDefinitionKind kind) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);
	JagAstDeclarationType *decl_type = jag_ast_declaration_type_new(primitive_type, 0);
	JagDecValueTypeDefinition *definition = jag_dec_value_type_definition_new(decl_type, kind);
	cat_unref(decl_type);
	cat_array_wo_append(priv->e_list, (GObject *) definition);
	cat_unref(definition);
}




static int primitiveTypeGetPrio(JagBytType type) {
	switch(type) {
		case JAG_BYT_TYPE_BOOLEAN : return 0;
		case JAG_BYT_TYPE_BYTE : return 10;
		case JAG_BYT_TYPE_SHORT : return 20;
		case JAG_BYT_TYPE_INT : return 30;
		case JAG_BYT_TYPE_CHAR : return 35;
		case JAG_BYT_TYPE_FLOAT : return 40;
		case JAG_BYT_TYPE_LONG : return 50;
		case JAG_BYT_TYPE_DOUBLE : return 60;
		case JAG_BYT_TYPE_VOID : return 1000;
		case JAG_BYT_TYPE_REFERENCE : return 200;
	}
	return 2000;
}

static gboolean primitiveTypeIsPrefered(JagBytType main, JagBytType other) {
	return primitiveTypeGetPrio(other)>primitiveTypeGetPrio(main);
}



static JagAstDeclarationType *l_get_local_best_type_def(JagDecValueTypeDefinitionList *list) {
	JagAstDeclarationType *result = NULL;
	JagDecValueTypeDefinitionKind bestKind;
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);

	CatIIterator *iter = cat_array_wo_iterator(priv->e_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecValueTypeDefinition *valTypeDef = (JagDecValueTypeDefinition *) cat_iiterator_next(iter);
		if (jag_dec_value_type_definition_is_included(valTypeDef)) {

			JagDecValueTypeDefinitionKind thisKind = jag_dec_value_type_definition_get_kind(valTypeDef);
			JagAstDeclarationType *thisType = jag_dec_value_type_definition_get_declaration_type(valTypeDef);

			if (result==NULL) {
				result = thisType;
				bestKind = thisKind;
			} else {
				if (jag_dec_value_type_definition_kind_get_prio(bestKind)<jag_dec_value_type_definition_kind_get_prio(thisKind)) {
					result = thisType;
					bestKind = thisKind;
				} else if (jag_dec_value_type_definition_kind_get_prio(bestKind)==jag_dec_value_type_definition_kind_get_prio(thisKind)) {

					if (jag_ast_declaration_type_is_primitive(result) && jag_ast_declaration_type_is_primitive(thisType)) {

						if (!primitiveTypeIsPrefered(jag_ast_declaration_type_get_primitive_type(result), jag_ast_declaration_type_get_primitive_type(thisType))) {
							result = thisType;
							bestKind = thisKind;
						}
					}
				}
			}
		}
	}
	cat_unref_ptr(iter);
	return result;
}

JagAstDeclarationType *jag_dec_value_type_definition_list_get_best_type_ref(JagDecValueTypeDefinitionList *list) {
	JagAstDeclarationType *result = l_get_local_best_type_def(list);
	if (result == NULL) {
		JagBytName *name_object = jag_byt_name_new(cat_string_wo_new_with("java.lang.Object"), FALSE);
		result = jag_ast_declaration_type_new_reference(name_object, 0);
		cat_unref_ptr(name_object);
	} else {
		cat_ref_ptr(result);
	}
	return result;
}


void jag_dec_value_type_definition_list_add_cross_rule(JagDecValueTypeDefinitionList *list, JagDecICrossRule *cross_rule) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);
	cat_array_wo_append(priv->e_cross_rules, (GObject *) cross_rule);
}

gboolean jag_dec_value_type_definition_list_apply_cross_rules(JagDecValueTypeDefinitionList *list) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);
	if (priv->crossRulesState == JAG_DEC_CROSS_RULE_STATE_RESOLVED) {
		return TRUE;
	}
	gboolean result = TRUE;

//	log.debug("cross-rules.size="+crossRules.size()+", this="+this);

	int cridx = 0;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_cross_rules);
	while(cat_iiterator_has_next(iter)) {
		JagDecICrossRule *crossRule = (JagDecICrossRule *) cat_iiterator_next(iter);
		gboolean didApply = jag_dec_icross_rule_apply(crossRule);
//		log.detail("["+cridx+"] cross-rule="+crossRule+", didApply="+didApply);
		cridx++;
		if (!didApply) {
			result = FALSE;
		}
	}
	cat_unref_ptr(iter);
	if (result) {
		priv->crossRulesState = JAG_DEC_CROSS_RULE_STATE_RESOLVED;
	} else {
		switch(priv->crossRulesState) {
			case JAG_DEC_CROSS_RULE_STATE_RESOLVED : {
				result = TRUE;
			} break;
			case JAG_DEC_CROSS_RULE_STATE_RUNNING : {
				result = TRUE;
				priv->crossRulesState = JAG_DEC_CROSS_RULE_STATE_RESOLVED;
			} break;
			case JAG_DEC_CROSS_RULE_STATE_UNRESOLVED : {
				priv->crossRulesState = JAG_DEC_CROSS_RULE_STATE_RUNNING;
			} break;
		}
	}

	return result;
}


gboolean jag_dec_value_type_definition_list_all_cross_rules_applied(JagDecValueTypeDefinitionList *list) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);
	return priv->crossRulesState==JAG_DEC_CROSS_RULE_STATE_RESOLVED;
}

gboolean jag_dec_value_type_definition_list_cross_rules_running(JagDecValueTypeDefinitionList *list) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);
	return priv->crossRulesState==JAG_DEC_CROSS_RULE_STATE_RUNNING;
}


void jag_dec_value_type_definition_list_copy_to(JagDecValueTypeDefinitionList *list, JagDecValueTypeDefinitionList *dest) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);
//	JagDecValueTypeDefinitionListPrivate *dest_priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(dest);
	CatIIterator *iter = cat_array_wo_iterator(priv->e_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecValueTypeDefinition *valTypeDef = (JagDecValueTypeDefinition *) cat_iiterator_next(iter);
		JagDecValueTypeDefinition *cloned = jag_dec_value_type_definition_clone(valTypeDef);
		jag_dec_value_type_definition_list_add_definition(dest, cloned);
		cat_unref_ptr(cloned);
	}
	cat_unref_ptr(iter);
}


CatHashSet *jag_dec_value_type_definition_list_enlist_included(JagDecValueTypeDefinitionList *list) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);
	CatHashSet *result = cat_hash_set_new((GHashFunc) jag_dec_value_type_definition_hash, (GEqualFunc) jag_dec_value_type_definition_equal);
	CatIIterator *iter = cat_array_wo_iterator(priv->e_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecValueTypeDefinition *valTypeDef = (JagDecValueTypeDefinition *) cat_iiterator_next(iter);
		if (jag_dec_value_type_definition_is_included(valTypeDef)) {
			cat_hash_set_add(result, (GObject *) valTypeDef);
		}
	}
	cat_unref_ptr(iter);
	return result;
}

void jag_dec_value_type_definition_list_exclude_set(JagDecValueTypeDefinitionList *list, CatHashSet *set_to_exclude, gboolean opposite) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(list);
	CatIIterator *iter = cat_array_wo_iterator(priv->e_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecValueTypeDefinition *valTypeDef = (JagDecValueTypeDefinition *) cat_iiterator_next(iter);
		if (jag_dec_value_type_definition_is_included(valTypeDef)) {
			if (cat_hash_set_get(set_to_exclude, (GObject *) valTypeDef)!=NULL) {
				if (!opposite) {
					jag_dec_value_type_definition_set_included(valTypeDef, TRUE);
				}
			} else {
				if (opposite) {
					jag_dec_value_type_definition_set_included(valTypeDef, TRUE);
				}
			}
		}
	}
	cat_unref_ptr(iter);
}



/********************* start CatIStringableInterface implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagDecValueTypeDefinitionListPrivate *priv = JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(self);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(append_to, name+6);
	cat_string_wo_append_char(append_to, '[');
	cat_string_wo_append_hexadecimal(append_to, (long long int) self, -1);
	cat_string_wo_append_chars(append_to, ", types=");
	gboolean is_first = TRUE;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_list);
	while(cat_iiterator_has_next(iter)) {
		if (is_first) {
			is_first = FALSE;
		} else {
			cat_string_wo_append_chars(append_to, ", ");
		}
		JagDecValueTypeDefinition *valTypeDef = (JagDecValueTypeDefinition *) cat_iiterator_next(iter);
		JagAstDeclarationType *type_def = jag_dec_value_type_definition_get_declaration_type(valTypeDef);
		jag_ast_declaration_type_as_text(type_def, append_to);
		cat_string_wo_append_char(append_to, ':');
		JagDecValueTypeDefinitionKind kind = jag_dec_value_type_definition_get_kind(valTypeDef);
		cat_string_wo_append_decimal(append_to, kind);
	}
	cat_unref_ptr(iter);
	cat_string_wo_append_char(append_to, ']');


}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringableInterface implementation *********************/
