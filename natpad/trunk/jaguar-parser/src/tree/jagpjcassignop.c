/*
   File:    jagpjcassignop.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 4, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "jagpjcassignop.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPJCAssignOp"
#include <logging/catlog.h>

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPJCAssignOp, jagp_jcassignop, JAGP_TYPE_JCEXPRESSION,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_tag(JagPJCTree *tree) { return ((JagPJCAssignOp *) tree)->opcode; }

static void jagp_jcassignop_class_init(JagPJCAssignOpClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_tag;
}

static void jagp_jcassignop_init(JagPJCAssignOp *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCAssignOp *instance = JAGP_JCASSIGNOP(object);
	cat_unref_ptr(instance->lhs);
	cat_unref_ptr(instance->rhs);
	G_OBJECT_CLASS(jagp_jcassignop_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcassignop_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCAssignOp *jagp_jcassignop_new(JagPTag opcode, JagPJCExpression *lhs, JagPJCExpression *rhs) {
	JagPJCAssignOp *result = g_object_new(JAGP_TYPE_JCASSIGNOP, NULL);
	cat_ref_anounce(result);
//	JAGP_JCEXPRESSION_construct((JagPJCExpression *) result);
	result->opcode = opcode;
	result->lhs = cat_ref_ptr(lhs);
	result->rhs = cat_ref_ptr(rhs);
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
