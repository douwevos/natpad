/*
   File:    grobnfrhssublist.c
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

#include "grobnfrhssublist.h"
#include "groibnfrhsenlistable.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfRhsSubList"
#include <logging/catlog.h>

struct _GroBnfRhsSubListPrivate {
	GroBnfRhsSubList *linked;
	GroBnfRhsSub *sub;
};

static void l_enlistable_iface_init(GroIBnfRhsEnlistableInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfRhsSubList, gro_bnf_rhs_sub_list, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfRhsSubList)
		G_IMPLEMENT_INTERFACE(GRO_TYPE_IBNF_RHS_ENLISTABLE, l_enlistable_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_rhs_sub_list_class_init(GroBnfRhsSubListClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_rhs_sub_list_init(GroBnfRhsSubList *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroBnfRhsSubList *instance = GRO_BNF_RHS_SUB_LIST(object);
	GroBnfRhsSubListPrivate *priv = gro_bnf_rhs_sub_list_get_instance_private(instance);
	cat_unref_ptr(priv->linked);
	cat_unref_ptr(priv->sub);
	G_OBJECT_CLASS(gro_bnf_rhs_sub_list_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_rhs_sub_list_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroBnfRhsSubList *gro_bnf_rhs_sub_list_new(GroBnfRhsSubList *linked, GroBnfRhsSub *sub) {
	GroBnfRhsSubList *result = g_object_new(GRO_TYPE_BNF_RHS_SUB_LIST, NULL);
	cat_ref_anounce(result);
	GroBnfRhsSubListPrivate *priv = gro_bnf_rhs_sub_list_get_instance_private(result);
	priv->linked = cat_ref_ptr(linked);
	priv->sub = cat_ref_ptr(sub);
	return result;
}



/********************* start GroIBnfRhsEnlistable implementation *********************/

static CatArrayWo *l_enlist(GroIBnfRhsEnlistable *self) {
	GroBnfRhsSubList *tdl = GRO_BNF_RHS_SUB_LIST(self);
	CatArrayWo *result = cat_array_wo_new();
	while(tdl!=NULL) {
		GroBnfRhsSubListPrivate *priv = gro_bnf_rhs_sub_list_get_instance_private(tdl);
		cat_array_wo_append(result, (GObject *) priv->sub);
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
	GroBnfRhsSubList *instance = GRO_BNF_RHS_SUB_LIST(self);
	GroBnfRhsSubListPrivate *priv = gro_bnf_rhs_sub_list_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
