/*
   File:    jagpjcclassdecl.c
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

#include "jagpjcclassdecl.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPJCClassDecl"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagPJCClassDecl, jagp_jcclass_decl, JAGP_TYPE_JCSTATEMENT);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_class(JagPJCTree *tree) { return JAGP_TAG_CLASSDEF; }
static void l_tree_dump(JagPJCTree *tree, CatStringWo *indent);

static void jagp_jcclass_decl_class_init(JagPJCClassDeclClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_class;
	tree_class->dump = l_tree_dump;
}

static void jagp_jcclass_decl_init(JagPJCClassDecl *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCClassDecl *instance = JAGP_JCCLASS_DECL(object);
	cat_unref_ptr(instance->modifiers);
	cat_unref_ptr(instance->name);
	cat_unref_ptr(instance->typarams);
	cat_unref_ptr(instance->extending);
	cat_unref_ptr(instance->implementing);
	cat_unref_ptr(instance->defs);
	G_OBJECT_CLASS(jagp_jcclass_decl_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcclass_decl_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCClassDecl *jagp_jcclass_decl_new(JagPJCModifiers *modifiers, JagPName *name, CatArrayWo *typarams, JagPJCExpression *extending, CatArrayWo *implementing, CatArrayWo *defs /*, ClassSymbol*/) {
	JagPJCClassDecl *result = g_object_new(JAGP_TYPE_JCCLASS_DECL, NULL);
	cat_ref_anounce(result);
	result->modifiers = cat_ref_ptr(modifiers);
	result->name = cat_ref_ptr(name);
	result->typarams = cat_ref_ptr(typarams);
	result->extending = cat_ref_ptr(extending);
	result->implementing = cat_ref_ptr(implementing);
	result->defs = cat_ref_ptr(defs);
	return result;
}


static void l_tree_dump(JagPJCTree *tree, CatStringWo *indent) {
	JagPJCClassDecl *unit = (JagPJCClassDecl *) tree;
	cat_log_print("DUMP", "%OClassDecl: %O, left=%O - right=%O", indent, unit->name, tree->cursor, tree->cursor_end);
	cat_log_print("DUMP", "%O           modifiers:   %O", indent, unit->modifiers);
	if (unit->typarams) {
		cat_log_print("DUMP", "%O           type-params: %O", indent, unit->typarams);
	}
	if (unit->extending) {
		cat_log_print("DUMP", "%O           extends:     %O", indent, unit->extending);
	}
	if (unit->implementing) {
		cat_log_print("DUMP", "%O           implements:  %O", indent, unit->implementing);
	}
	CatStringWo *cindent = cat_string_wo_new();
	cat_string_wo_format(cindent, "%O  ", indent);
	CatIIterator *iter = cat_array_wo_iterator(unit->defs);
	while(cat_iiterator_has_next(iter)) {
		JagPJCTree *child = (JagPJCTree *) cat_iiterator_next(iter);
		jagp_jctree_dump(child, cindent);
	}
	cat_unref_ptr(iter);
	cat_unref_ptr(cindent);
}
