/*
   File:    jagpqualident.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Aug 4, 2017
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

#include "jagpqualident.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPQualident"
#include <logging/catlog.h>

struct _JagPQualidentPrivate {
	CatArrayWo *children;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPQualident, jagp_qualident, JAGP_TYPE_JCEXPRESSION,
		G_ADD_PRIVATE(JagPQualident)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagPTag l_tree_get_class(JagPJCTree *tree) { return JAGP_TAG_QUALIDENT; }

static void jagp_qualident_class_init(JagPQualidentClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagPJCTreeClass *tree_class = JAGP_JCTREE_CLASS(clazz);
	tree_class->getTag = l_tree_get_class;
}

static void jagp_qualident_init(JagPQualident *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPQualident *instance = JAGP_QUALIDENT(object);
	JagPQualidentPrivate *priv = jagp_qualident_get_instance_private(instance);
	cat_unref_ptr(priv->children);
	G_OBJECT_CLASS(jagp_qualident_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_qualident_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPQualident *jagp_qualident_new(JagPToken *ident) {
	JagPQualident *result = g_object_new(JAGP_TYPE_QUALIDENT, NULL);
	cat_ref_anounce(result);
	JagPQualidentPrivate *priv = jagp_qualident_get_instance_private(result);
	jagp_jcexpression_construct((JagPJCExpression *) result);
	priv->children = cat_array_wo_new();
	cat_array_wo_append(priv->children, (GObject *) ident);
	return result;
}

void jagp_qualident_add_dot_ident(JagPQualident *qualident, JagPToken *dot, CatArrayWo *tyannos, JagPToken *ident) {
	JagPQualidentPrivate *priv = jagp_qualident_get_instance_private(qualident);
	cat_array_wo_append(priv->children, (GObject *) dot);
	if (tyannos) {
		cat_array_wo_append(priv->children, (GObject *) tyannos);
	}
	cat_array_wo_append(priv->children, (GObject *) ident);
}

gboolean jagp_qualident_is_plain_but_not_this(JagPQualident *qualident, JagPName *this_name) {
	JagPQualidentPrivate *priv = jagp_qualident_get_instance_private(qualident);
	if (cat_array_wo_size(priv->children)==1) {
		JagPToken *first = (JagPToken *) cat_array_wo_get_first(priv->children);
		if (first->kind==JAGP_KIND_IDENTIFIER) {
			JagPName *name = (JagPName *) first->value;
			return this_name!=name;
		}
	}
	return FALSE;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPQualident *instance = JAGP_QUALIDENT(self);
	JagPQualidentPrivate *priv = jagp_qualident_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, %O]", iname, self, priv->children);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
