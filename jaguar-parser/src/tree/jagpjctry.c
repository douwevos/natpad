/*
   File:    jagpjctry.c
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

#include "jagpjctry.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPJCTry"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagPJCTry, jagp_jctry, JAGP_TYPE_JCSTATEMENT);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_tag(JagPJCTree *tree) { return JAGP_TAG_TRY; }

static void jagp_jctry_class_init(JagPJCTryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_tag;
}

static void jagp_jctry_init(JagPJCTry *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCTry *instance = JAGP_JCTRY(object);
	cat_unref_ptr(instance->resources);
	cat_unref_ptr(instance->body);
	cat_unref_ptr(instance->catchers);
	cat_unref_ptr(instance->finalizer);
	G_OBJECT_CLASS(jagp_jctry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jctry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCTry *jagp_jctry_new(CatArrayWo /*<JagPJCTree>*/ *resources, JagPJCBlock *body, CatArrayWo /*<JCCatch>*/ *catchers, JagPJCBlock *finalizer) {
	JagPJCTry *result = g_object_new(JAGP_TYPE_JCTRY, NULL);
	cat_ref_anounce(result);
	result->resources = cat_ref_ptr(resources);
	result->body = cat_ref_ptr(body);
	result->catchers = cat_ref_ptr(catchers);
	result->finalizer = cat_ref_ptr(finalizer);
	return result;
}

