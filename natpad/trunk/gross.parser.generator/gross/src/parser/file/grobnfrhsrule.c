/*
   File:    grobnfrhsrule.c
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

#include "grobnfrhsrule.h"
#include "groibnfrhsenlistable.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfRhsRule"
#include <logging/catlog.h>

struct _GroBnfRhsRulePrivate {
	GroBnfString *label;
	GroBnfRhsRulePartList *rr_part_list;
};

static void l_enlistable_iface_init(GroIBnfRhsEnlistableInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfRhsRule, gro_bnf_rhs_rule, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfRhsRule)
		G_IMPLEMENT_INTERFACE(GRO_TYPE_IBNF_RHS_ENLISTABLE, l_enlistable_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_rhs_rule_class_init(GroBnfRhsRuleClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_rhs_rule_init(GroBnfRhsRule *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroBnfRhsRule *instance = GRO_BNF_RHS_RULE(object);
	GroBnfRhsRulePrivate *priv = gro_bnf_rhs_rule_get_instance_private(instance);
	cat_unref_ptr(priv->label);
	cat_unref_ptr(priv->rr_part_list);
	G_OBJECT_CLASS(gro_bnf_rhs_rule_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_rhs_rule_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

GroBnfRhsRule *gro_bnf_rhs_rule_new(GroBnfString *lab, GroBnfRhsRulePartList *rr_part_list) {
	GroBnfRhsRule *result = g_object_new(GRO_TYPE_BNF_RHS_RULE, NULL);
	cat_ref_anounce(result);
	GroBnfRhsRulePrivate *priv = gro_bnf_rhs_rule_get_instance_private(result);
	priv->label = cat_ref_ptr(lab);
	priv->rr_part_list = cat_ref_ptr(rr_part_list);
	return result;
}

/********************* start GroIBnfRhsEnlistable implementation *********************/

static CatArrayWo *l_enlist(GroIBnfRhsEnlistable *self) {
	GroBnfRhsRulePrivate *priv = gro_bnf_rhs_rule_get_instance_private(GRO_BNF_RHS_RULE(self));
	if (priv->rr_part_list==NULL) {
		return cat_array_wo_new();
	}
	return gro_bnf_rhs_rule_part_list_enlist(priv->rr_part_list);
}


static void l_enlistable_iface_init(GroIBnfRhsEnlistableInterface *iface) {
	iface->enlist = l_enlist;
}

/********************* end GroIBnfRhsEnlistable implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroBnfRhsRule *instance = GRO_BNF_RHS_RULE(self);
	GroBnfRhsRulePrivate *priv = gro_bnf_rhs_rule_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
