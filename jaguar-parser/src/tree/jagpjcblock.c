/*
   File:    jagpjcblock.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 9, 2017
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

#include "jagpjcblock.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPJCBlock"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagPJCBlock, jagp_jcblock, JAGP_TYPE_JCSTATEMENT);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_tag(JagPJCTree *tree) { return JAGP_TAG_BLOCK; }
static void l_tree_dump(JagPJCTree *tree, CatStringWo *indent);

static void jagp_jcblock_class_init(JagPJCBlockClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_tag;
	tree_class->dump = l_tree_dump;
}

static void jagp_jcblock_init(JagPJCBlock *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCBlock *instance = JAGP_JCBLOCK(object);
	cat_unref_ptr(instance->stats);
	G_OBJECT_CLASS(jagp_jcblock_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcblock_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCBlock *jagp_jcblock_new(long long flags, CatArrayWo *stats) {
	JagPJCBlock *result = g_object_new(JAGP_TYPE_JCBLOCK, NULL);
	cat_ref_anounce(result);
	result->flags = flags;
	result->stats = cat_ref_ptr(stats);
	result->endpos = NULL;
	return result;
}



static void l_tree_dump(JagPJCTree *tree, CatStringWo *indent) {
	JagPJCBlock *unit = (JagPJCBlock *) tree;
	cat_log_print("DUMP", "%OBlock: flags=%ld, left=%O - right=%O", indent, unit->flags, tree->cursor, tree->cursor_end);
	CatStringWo *cindent = cat_string_wo_new();
	cat_string_wo_format(cindent, "%O  ", indent);
	CatIIterator *iter = cat_array_wo_iterator(unit->stats);
	while(cat_iiterator_has_next(iter)) {
		JagPJCTree *child = (JagPJCTree *) cat_iiterator_next(iter);
		jagp_jctree_dump(child, cindent);
	}
	cat_unref_ptr(iter);
	cat_unref_ptr(cindent);
}
