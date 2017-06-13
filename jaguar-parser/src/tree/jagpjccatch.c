/*
   File:    jagpjccatch.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 14, 2017
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

#include "jagpjccatch.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPJCCatch"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagPJCCatch, jagp_jccatch, JAGP_TYPE_JCTREE);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_class(JagPJCTree *tree) { return JAGP_TAG_CATCH; }

static void jagp_jccatch_class_init(JagPJCCatchClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_class;
}

static void jagp_jccatch_init(JagPJCCatch *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCCatch *instance = JAGP_JCCATCH(object);
	cat_unref_ptr(instance->param);
	cat_unref_ptr(instance->body);
	G_OBJECT_CLASS(jagp_jccatch_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jccatch_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCCatch *jagp_jccatch_new(JagPJCVariableDecl *param, JagPJCBlock *body) {
	JagPJCCatch *result = g_object_new(JAGP_TYPE_JCCATCH, NULL);
	cat_ref_anounce(result);
	result->param = cat_ref_ptr(param);
	result->body = cat_ref_ptr(body);
	return result;
}
