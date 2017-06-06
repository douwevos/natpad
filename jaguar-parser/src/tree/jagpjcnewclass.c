/*
   File:    jagpjcnewclass.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 13, 2017
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

#include "jagpjcnewclass.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPJCNewClass"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagPJCNewClass, jagp_jcnew_class, JAGP_TYPE_JCPOLY_EXPRESSION);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_tag(JagPJCTree *tree) { return JAGP_TAG_NEWCLASS; }

static void jagp_jcnew_class_class_init(JagPJCNewClassClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_tag;
}

static void jagp_jcnew_class_init(JagPJCNewClass *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCNewClass *instance = JAGP_JCNEW_CLASS(object);
	cat_unref_ptr(instance->args);
	cat_unref_ptr(instance->clazz);
	cat_unref_ptr(instance->def);
	cat_unref_ptr(instance->encl);
	cat_unref_ptr(instance->typeargs);
	G_OBJECT_CLASS(jagp_jcnew_class_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcnew_class_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCNewClass *jagp_jcnew_class_new(JagPJCExpression *encl, CatArrayWo /*<JCExpression>*/ *typeargs, JagPJCExpression *clazz,
        CatArrayWo /*<JCExpression>*/ *args, JagPJCClassDecl *def) {
	JagPJCNewClass *result = g_object_new(JAGP_TYPE_JCNEW_CLASS, NULL);
	cat_ref_anounce(result);
//	JAGP_JCPOLY_EXPRESSION_construct((JagPJCPolyExpression *) result);
	result->encl = cat_ref_ptr(encl);
	result->typeargs = cat_ref_ptr(typeargs);
	result->clazz = cat_ref_ptr(clazz);
	result->args = cat_ref_ptr(args);
	result->def = cat_ref_ptr(def);
	return result;
}

