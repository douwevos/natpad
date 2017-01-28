/*
   File:    draconnectorkey.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 26, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "draconnectorkey.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraConnectorKey"
#include <logging/catlog.h>

struct _DraConnectorKeyPrivate {
	ChaDocument *document;
	CatStringWo *a_slot_key;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraConnectorKey, dra_connector_key, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraConnectorKey)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_connector_key_class_init(DraConnectorKeyClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_connector_key_init(DraConnectorKey *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraConnectorKey *instance = DRA_CONNECTOR_KEY(object);
	DraConnectorKeyPrivate *priv = dra_connector_key_get_instance_private(instance);
	cat_unref_ptr(priv->a_slot_key);
	cat_unref_ptr(priv->document);
	G_OBJECT_CLASS(dra_connector_key_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_connector_key_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraConnectorKey *dra_connector_key_new(ChaDocument *document, CatStringWo *a_slot_key) {
	DraConnectorKey *result = g_object_new(DRA_TYPE_CONNECTOR_KEY, NULL);
	cat_ref_anounce(result);
	DraConnectorKeyPrivate *priv = dra_connector_key_get_instance_private(result);
	priv->document = cat_ref_ptr(document);
	priv->a_slot_key = cat_ref_ptr(a_slot_key);
	return result;
}


int dra_connector_key_hash(const DraConnectorKey *key) {
	DraConnectorKeyPrivate *priv = dra_connector_key_get_instance_private((DraConnectorKey *) key);
	return cha_document_hash_code(priv->document)*7 + cat_string_wo_hash(priv->a_slot_key);
}

gboolean dra_connector_key_equal(const DraConnectorKey *key_a, const DraConnectorKey *key_b) {
	if (key_a==key_b) {
		return TRUE;
	}
	if (key_a==NULL || key_b==NULL) {
		return FALSE;
	}
	DraConnectorKeyPrivate *priv_a = dra_connector_key_get_instance_private((DraConnectorKey *) key_a);
	DraConnectorKeyPrivate *priv_b = dra_connector_key_get_instance_private((DraConnectorKey *) key_b);
	return priv_a->document==priv_b->document && cat_string_wo_equal(priv_a->a_slot_key, priv_b->a_slot_key);
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
