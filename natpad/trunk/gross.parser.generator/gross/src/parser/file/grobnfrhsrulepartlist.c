/*
   File:    grobnfrhsrulepartlist.c
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

#include "grobnfrhsrulepartlist.h"
#include "groibnfrhsenlistable.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfRhsRulePartList"
#include <logging/catlog.h>

struct _GroBnfRhsRulePartListPrivate {
	GroBnfRhsRulePartList *linked;
	GroIBnfRhsRulePart *rhs_rule_part;
};

static void l_enlistable_iface_init(GroIBnfRhsEnlistableInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfRhsRulePartList, gro_bnf_rhs_rule_part_list, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfRhsRulePartList)
		G_IMPLEMENT_INTERFACE(GRO_TYPE_IBNF_RHS_ENLISTABLE, l_enlistable_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_rhs_rule_part_list_class_init(GroBnfRhsRulePartListClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_rhs_rule_part_list_init(GroBnfRhsRulePartList *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroBnfRhsRulePartList *instance = GRO_BNF_RHS_RULE_PART_LIST(object);
	GroBnfRhsRulePartListPrivate *priv = gro_bnf_rhs_rule_part_list_get_instance_private(instance);
	cat_unref_ptr(priv->linked);
	cat_unref_ptr(priv->rhs_rule_part);
	G_OBJECT_CLASS(gro_bnf_rhs_rule_part_list_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_rhs_rule_part_list_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

GroBnfRhsRulePartList *gro_bnf_rhs_rule_part_list_new(GroBnfRhsRulePartList *linked, GroIBnfRhsRulePart *rhs_rule_part) {
	GroBnfRhsRulePartList *result = g_object_new(GRO_TYPE_BNF_RHS_RULE_PART_LIST, NULL);
	cat_ref_anounce(result);
	GroBnfRhsRulePartListPrivate *priv = gro_bnf_rhs_rule_part_list_get_instance_private(result);
	priv->linked = cat_ref_ptr(linked);
	priv->rhs_rule_part = cat_ref_ptr(rhs_rule_part);
	return result;
}

/********************* start GroIBnfRhsEnlistable implementation *********************/

static CatArrayWo *l_enlist(GroIBnfRhsEnlistable *self) {
	GroBnfRhsRulePartList *tdl = GRO_BNF_RHS_RULE_PART_LIST(self);
	CatArrayWo *result = cat_array_wo_new();
	while(tdl!=NULL) {
		GroBnfRhsRulePartListPrivate *priv = gro_bnf_rhs_rule_part_list_get_instance_private(tdl);
		cat_array_wo_append(result, (GObject *) priv->rhs_rule_part);
		tdl = priv->linked;
	}
	cat_array_wo_reverse(result);
	return result;
}


static void l_enlistable_iface_init(GroIBnfRhsEnlistableInterface *iface) {
	iface->enlist = l_enlist;
}

/********************* end GroIBnfRhsEnlistable implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroBnfRhsRulePartList *instance = GRO_BNF_RHS_RULE_PART_LIST(self);
	GroBnfRhsRulePartListPrivate *priv = gro_bnf_rhs_rule_part_list_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
