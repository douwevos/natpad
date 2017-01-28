/*
   File:    grobnfrhsunarypart.c
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

#include "grobnfrhsunarypart.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfRhsUnaryPart"
#include <logging/catlog.h>

struct _GroBnfRhsUnaryPartPrivate {
	GroIBnfRhsRulePart *rr_part;
	GroBnfRhsUnaryOp op;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfRhsUnaryPart, gro_bnf_rhs_unary_part, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfRhsUnaryPart)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_rhs_unary_part_class_init(GroBnfRhsUnaryPartClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_rhs_unary_part_init(GroBnfRhsUnaryPart *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroBnfRhsUnaryPart *instance = GRO_BNF_RHS_UNARY_PART(object);
	GroBnfRhsUnaryPartPrivate *priv = gro_bnf_rhs_unary_part_get_instance_private(instance);
	cat_unref_ptr(priv->rr_part)
	G_OBJECT_CLASS(gro_bnf_rhs_unary_part_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_rhs_unary_part_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

GroBnfRhsUnaryPart *gro_bnf_rhs_unary_part_new(GroIBnfRhsRulePart *rr_part, GroBnfRhsUnaryOp op) {
	GroBnfRhsUnaryPart *result = g_object_new(GRO_TYPE_BNF_RHS_UNARY_PART, NULL);
	cat_ref_anounce(result);
	GroBnfRhsUnaryPartPrivate *priv = gro_bnf_rhs_unary_part_get_instance_private(result);
	priv->rr_part = cat_ref_ptr(rr_part);
	priv->op = op;
	return result;
}


GroIBnfRhsRulePart *gro_bnf_rhs_unary_part_get_part(GroBnfRhsUnaryPart *un_part) {
	GroBnfRhsUnaryPartPrivate *priv = gro_bnf_rhs_unary_part_get_instance_private(un_part);
	return priv->rr_part;
}

GroBnfRhsUnaryOp gro_bnf_rhs_unary_part_get_operator(GroBnfRhsUnaryPart *un_part) {
	GroBnfRhsUnaryPartPrivate *priv = gro_bnf_rhs_unary_part_get_instance_private(un_part);
	return priv->op;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroBnfRhsUnaryPart *instance = GRO_BNF_RHS_UNARY_PART(self);
	GroBnfRhsUnaryPartPrivate *priv = gro_bnf_rhs_unary_part_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

