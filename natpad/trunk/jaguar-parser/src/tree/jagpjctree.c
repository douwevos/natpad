/*
   File:    jagpjctree.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 1, 2017
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

#include "jagpjctree.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPJCTree"
#include <logging/catlog.h>

struct _JagPJCTreePrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPJCTree, jagp_jctree, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPJCTree)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_jctree_class_init(JagPJCTreeClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_jctree_init(JagPJCTree *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPJCTree *instance = JAGP_JCTREE(object);
//	JagPJCTreePrivate *priv = jagp_jctree_get_instance_private(instance);
	G_OBJECT_CLASS(jagp_jctree_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jctree_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCTree *jagp_jctree_new() {
	JagPJCTree *result = g_object_new(JAGP_TYPE_JCTREE, NULL);
	cat_ref_anounce(result);
	JagPJCTreePrivate *priv = jagp_jctree_get_instance_private(result);
//	G_OBJECT_construct((GObject *) result);
	return result;
}

JagPTag jagp_jctree_get_tag(JagPJCTree *tree) {
	JagPJCTreeClass *clazz = JAGP_JCTREE_GET_CLASS(tree);
	if (clazz->getTag == NULL) {
		cat_log_error("class does not implement jctree.getTag : %O", tree);
	}
	return clazz->getTag(tree);
}

gboolean jagp_jctree_has_tag(JagPJCTree *tree, JagPTag tag) {
	JagPJCTreeClass *clazz = JAGP_JCTREE_GET_CLASS(tree);
	if (clazz->getTag == NULL) {
		cat_log_error("class does not implement jctree.getTag : %O", tree);
	}
	return clazz->getTag(tree)==tag;
}


int jagp_jctree_get_start_position(JagPJCTree *tree) {
	return jagp_tree_info_get_end_pos(tree);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPJCTree *instance = JAGP_JCTREE(self);
	JagPJCTreePrivate *priv = jagp_jctree_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
