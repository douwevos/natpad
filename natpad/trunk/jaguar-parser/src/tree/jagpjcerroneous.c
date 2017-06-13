/*
   File:    jagpjcerroneous.c
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

#include "jagpjcerroneous.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPJCErroneous"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagPJCErroneous, jagp_jcerroneous, JAGP_TYPE_JCEXPRESSION);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_tag(JagPJCTree *tree) { return JAGP_TAG_ERRONEOUS; }

static void jagp_jcerroneous_class_init(JagPJCErroneousClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_tag;
}

static void jagp_jcerroneous_init(JagPJCErroneous *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCErroneous *instance = JAGP_JCERRONEOUS(object);
	cat_unref_ptr(instance->errs);
	G_OBJECT_CLASS(jagp_jcerroneous_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcerroneous_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagPJCErroneous *jagp_jcerroneous_new(CatArrayWo *errs) {
	JagPJCErroneous *result = g_object_new(JAGP_TYPE_JCERRONEOUS, NULL);
	cat_ref_anounce(result);
	result->errs = cat_ref_ptr(errs);
	return result;
}

