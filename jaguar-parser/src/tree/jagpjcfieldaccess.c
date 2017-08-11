/*
   File:    jagpjcfieldaccess.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 3, 2017
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

#include "jagpjcfieldaccess.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPJCFieldAccess"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagPJCFieldAccess, jagp_jcfield_access, JAGP_TYPE_JCEXPRESSION);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_tag(JagPJCTree *tree) { return JAGP_TAG_SELECT; }
static void l_tree_dump(JagPJCTree *tree, CatStringWo *indent);

static void jagp_jcfield_access_class_init(JagPJCFieldAccessClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_tag;
	tree_class->dump = l_tree_dump;
}

static void jagp_jcfield_access_init(JagPJCFieldAccess *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCFieldAccess *instance = JAGP_JCFIELD_ACCESS(object);
	cat_unref_ptr(instance->name);
	cat_unref_ptr(instance->selected);
	G_OBJECT_CLASS(jagp_jcfield_access_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcfield_access_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagPJCFieldAccess *jagp_jcfield_access_new(JagPJCExpression *selected, JagPJCTree *name) {
	JagPJCFieldAccess *result = g_object_new(JAGP_TYPE_JCFIELD_ACCESS, NULL);
	cat_ref_anounce(result);
	result->selected = cat_ref_ptr(selected);
	result->name = cat_ref_ptr(name);
	return result;
}

static void l_tree_dump(JagPJCTree *tree, CatStringWo *indent) {
	JagPJCFieldAccess *fldacc = (JagPJCFieldAccess *) tree;
	cat_log_print("DUMP", "%OFieldAccess: left=%O - right=%O", indent, tree->cursor, tree->cursor_end);
	CatStringWo *cindent = cat_string_wo_new();
	cat_string_wo_format(cindent, "%O  ", indent);
	if (fldacc->selected) {
		jagp_jctree_dump((JagPJCTree *) fldacc->selected, cindent);
	}
	if (fldacc->name) {
		cat_log_print("DUMP", "%O  name=%O", cindent, fldacc->name);
//		jagp_jctree_dump(fldacc->name, cindent);
	}

	cat_unref_ptr(cindent);
}
