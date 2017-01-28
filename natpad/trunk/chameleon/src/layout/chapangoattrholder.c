/*
   File:    chapangoattrholder.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 22, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "chapangoattrholder.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaPangoAttrHolder"
#include <logging/catlog.h>

struct _ChaPangoAttrHolderPrivate {
	PangoAttribute *attr;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPangoAttrHolder, cha_pango_attr_holder, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaPangoAttrHolder)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_pango_attr_holder_class_init(ChaPangoAttrHolderClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_pango_attr_holder_init(ChaPangoAttrHolder *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaPangoAttrHolder *instance = CHA_PANGO_ATTR_HOLDER(object);
	ChaPangoAttrHolderPrivate *priv = cha_pango_attr_holder_get_instance_private(instance);
	if (priv->attr) {
		pango_attribute_destroy(priv->attr);
		priv->attr = NULL;
	}
	G_OBJECT_CLASS(cha_pango_attr_holder_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_pango_attr_holder_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaPangoAttrHolder *cha_pango_attr_holder_new(PangoAttribute *pango_attr) {
	ChaPangoAttrHolder *result = g_object_new(CHA_TYPE_PANGO_ATTR_HOLDER, NULL);
	cat_ref_anounce(result);
	ChaPangoAttrHolderPrivate *priv = cha_pango_attr_holder_get_instance_private(result);
	priv->attr = pango_attr;
	return result;
}



PangoAttribute *cha_pango_attr_holder_get(ChaPangoAttrHolder *holder) {
	ChaPangoAttrHolderPrivate *priv = cha_pango_attr_holder_get_instance_private(holder);
	return priv->attr;
}

gboolean cha_pango_attr_holder_equal(const ChaPangoAttrHolder *holder_a, const ChaPangoAttrHolder *holder_b) {
	if (holder_a==holder_b) {
		return TRUE;
	} else if ((holder_a==NULL) || (holder_b==NULL)) {
		return FALSE;
	}
	ChaPangoAttrHolderPrivate *priv_a = cha_pango_attr_holder_get_instance_private((ChaPangoAttrHolder *) holder_a);
	ChaPangoAttrHolderPrivate *priv_b = cha_pango_attr_holder_get_instance_private((ChaPangoAttrHolder *) holder_b);

	if ((priv_a->attr->end_index==priv_b->attr->end_index)
			&& (priv_a->attr->start_index==priv_b->attr->start_index)
			&& (priv_a->attr->klass==priv_b->attr->klass)) {
		return pango_attribute_equal(priv_a->attr, priv_b->attr);
	}
	return FALSE;

}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaPangoAttrHolder *instance = CHA_PANGO_ATTR_HOLDER(self);
	ChaPangoAttrHolderPrivate *priv = cha_pango_attr_holder_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p attr=%p]", iname, self, priv->attr);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
