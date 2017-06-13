/*
   File:    jagpjcannotation.c
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

#include "jagpjcannotation.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPJCAnnotation"
#include <logging/catlog.h>

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPJCAnnotation, jagp_jcannotation, JAGP_TYPE_JCEXPRESSION,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_class(JagPJCTree *tree);

static void jagp_jcannotation_class_init(JagPJCAnnotationClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_class;
}

static void jagp_jcannotation_init(JagPJCAnnotation *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPJCAnnotation *instance = JAGP_JCANNOTATION(object);
	cat_unref_ptr(instance->annotation_type);
	cat_unref_ptr(instance->args);
	G_OBJECT_CLASS(jagp_jcannotation_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcannotation_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagPJCAnnotation *jagp_jcannotation_new(gboolean is_type_annotation, JagPJCTree *annotation_type, CatArrayWo *args) {
	JagPJCAnnotation *result = g_object_new(JAGP_TYPE_JCANNOTATION, NULL);
	cat_ref_anounce(result);
	result->is_type_annotation = is_type_annotation;
	result->annotation_type = cat_ref_ptr(annotation_type);
	result->args = cat_ref_ptr(args);
	return result;
}

static JagPTag l_tree_get_class(JagPJCTree *tree) {
	JagPJCAnnotation *instance = JAGP_JCANNOTATION(tree);
	if (instance->is_type_annotation) {
		return JAGP_TAG_TYPE_ANNOTATION;
	}
	return JAGP_TAG_ANNOTATION;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPJCAnnotation *instance = JAGP_JCANNOTATION(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
