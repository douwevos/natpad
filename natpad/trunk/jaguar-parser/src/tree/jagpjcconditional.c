/*
   File:    jagpjcconditional.c
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

#include "jagpjcconditional.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPJCConditional"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagPJCConditional, jagp_jcconditional, JAGP_TYPE_JCPOLY_EXPRESSION);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_tag(JagPJCTree *tree) { return JAGP_TAG_CONDEXPR; }

static void jagp_jcconditional_class_init(JagPJCConditionalClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_tag;
}

static void jagp_jcconditional_init(JagPJCConditional *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCConditional *instance = JAGP_JCCONDITIONAL(object);
	cat_unref_ptr(instance->cond);
	cat_unref_ptr(instance->truepart);
	cat_unref_ptr(instance->falsepart);
	G_OBJECT_CLASS(jagp_jcconditional_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcconditional_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCConditional *jagp_jcconditional_new(JagPJCExpression *cond, JagPJCExpression *truepart, JagPJCExpression *falsepart) {
	JagPJCConditional *result = g_object_new(JAGP_TYPE_JCCONDITIONAL, NULL);
	cat_ref_anounce(result);
//	JAGP_JCPOLY_EXPRESSION_construct((JagPJCPolyExpression *) result);
	result->cond = cat_ref_ptr(cond);
	result->truepart = cat_ref_ptr(truepart);
	result->falsepart = cat_ref_ptr(falsepart);
	return result;
}


