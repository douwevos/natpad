/*
   File:    jagpjctypeunion.c
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

#include "jagpjctypeunion.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPJCTypeUnion"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagPJCTypeUnion, jagp_jctype_union, JAGP_TYPE_JCEXPRESSION);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_tag(JagPJCTree *tree) { return JAGP_TAG_TYPEUNION; }

static void jagp_jctype_union_class_init(JagPJCTypeUnionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_tag;
}

static void jagp_jctype_union_init(JagPJCTypeUnion *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCTypeUnion *instance = JAGP_JCTYPE_UNION(object);
	cat_unref_ptr(instance->alternatives);
	G_OBJECT_CLASS(jagp_jctype_union_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jctype_union_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCTypeUnion *jagp_jctype_union_new(CatArrayWo /*<JagPJCExpression>*/ *alternatives) {
	JagPJCTypeUnion *result = g_object_new(JAGP_TYPE_JCTYPE_UNION, NULL);
	cat_ref_anounce(result);
	result->alternatives = cat_ref_ptr(alternatives);
	return result;
}

