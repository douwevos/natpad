/*
   File:    jagpjcannotatedtype.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 7, 2017
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

#include "jagpjcannotatedtype.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPJCAnnotatedType"
#include <logging/catlog.h>


static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPJCAnnotatedType, jagp_jcannotated_type, JAGP_TYPE_JCEXPRESSION,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_tag(JagPJCTree *tree) { return JAGP_TAG_ANNOTATED_TYPE; }

static void jagp_jcannotated_type_class_init(JagPJCAnnotatedTypeClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_tag;
}

static void jagp_jcannotated_type_init(JagPJCAnnotatedType *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCAnnotatedType *instance = JAGP_JCANNOTATED_TYPE(object);
	cat_unref_ptr(instance->annotations);
	cat_unref_ptr(instance->underlying_type);
	G_OBJECT_CLASS(jagp_jcannotated_type_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcannotated_type_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagPJCAnnotatedType *jagp_jcannotated_type_new(CatArrayWo *annotations, JagPJCExpression *underlying_type) {
	JagPJCAnnotatedType *result = g_object_new(JAGP_TYPE_JCANNOTATED_TYPE, NULL);
	cat_ref_anounce(result);
	jagp_jcexpression_construct((JagPJCExpression *) result);
	result->annotations = cat_ref_ptr(annotations);
	if (annotations) {
		CatIIterator *iter = cat_array_wo_iterator(annotations);
		while(cat_iiterator_has_next(iter)) {
			JagPJCTree *annotree = JAGP_JCTREE(cat_iiterator_next(iter));
			annotree->owner = result;
		}
		cat_unref_ptr(iter);
	}
	result->underlying_type = cat_ref_ptr(underlying_type);
	if (underlying_type) {
		((JagPJCTree *) underlying_type)->owner = result;
	}
	return result;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
