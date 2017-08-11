/*
   File:    jagpjcident.c
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

#include "jagpjcident.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPJCIdent"
#include <logging/catlog.h>

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPJCIdent, jagp_jcident, JAGP_TYPE_JCEXPRESSION,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_tag(JagPJCTree *tree) { return JAGP_TAG_IDENT; }
static void l_tree_dump(JagPJCTree *tree, CatStringWo *indent);


static void jagp_jcident_class_init(JagPJCIdentClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_tag;
	tree_class->dump = l_tree_dump;
}

static void jagp_jcident_init(JagPJCIdent *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCIdent *instance = JAGP_JCIDENT(object);
	cat_unref_ptr(instance->name);
	G_OBJECT_CLASS(jagp_jcident_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcident_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCIdent *jagp_jcident_new(JagPName *name) {
	JagPJCIdent *result = g_object_new(JAGP_TYPE_JCIDENT, NULL);
	cat_ref_anounce(result);
	result->name = cat_ref_ptr(name);
	return result;
}

static void l_tree_dump(JagPJCTree *tree, CatStringWo *indent) {
	JagPJCIdent *fldacc = (JagPJCIdent *) tree;
	cat_log_print("DUMP", "%OIdent: left=%O - right=%O", indent, tree->cursor, tree->cursor_end);
	CatStringWo *cindent = cat_string_wo_new();
	cat_string_wo_format(cindent, "%O  ", indent);
	if (fldacc->name) {
		cat_log_print("DUMP", "%O  name=%O", cindent, fldacc->name);
	}

	cat_unref_ptr(cindent);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPJCIdent *instance = JAGP_JCIDENT(self);
	cat_string_wo_format(append_to, "Ident[%O]", instance->name);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
