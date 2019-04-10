/*
   File:    draspellrequestfactory.c
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

#include "draspellrequestfactory.h"
#include "draspellrequest.h"
#include "../document/draiconnectorrequestfactory.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraSpellRequestFactory"
#include <logging/catlog.h>

struct _DraSpellRequestFactoryPrivate {
	DraSpellHelper *spell_helper;
	CatStringWo *a_slot_key;
};

static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraSpellRequestFactory, dra_spell_request_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraSpellRequestFactory)
		G_IMPLEMENT_INTERFACE(DRA_TYPE_ICONNECTOR_REQUEST_FACTORY, l_connector_request_factory_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_spell_request_factory_class_init(DraSpellRequestFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_spell_request_factory_init(DraSpellRequestFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraSpellRequestFactory *instance = DRA_SPELL_REQUEST_FACTORY(object);
	DraSpellRequestFactoryPrivate *priv = dra_spell_request_factory_get_instance_private(instance);
	cat_unref_ptr(priv->a_slot_key);
	cat_unref_ptr(priv->spell_helper);
	G_OBJECT_CLASS(dra_spell_request_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_spell_request_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraSpellRequestFactory *dra_spell_request_factory_new(DraSpellHelper *spell_helper, CatStringWo *a_slot_key) {
	DraSpellRequestFactory *result = g_object_new(DRA_TYPE_SPELL_REQUEST_FACTORY, NULL);
	cat_ref_anounce(result);
	DraSpellRequestFactoryPrivate *priv = dra_spell_request_factory_get_instance_private(result);
	priv->spell_helper = cat_ref_ptr(spell_helper);
	priv->a_slot_key = cat_ref_ptr(a_slot_key);
	return result;
}


/********************* start DraIConnectorRequestFactory implementation *********************/

static DraAugmentRequest *l_factory_create_request(DraIConnectorRequestFactory *self, ChaDocument *document, ChaRevisionWo *a_new_revision) {
	DraSpellRequestFactory *instance = DRA_SPELL_REQUEST_FACTORY(self);
	DraSpellRequestFactoryPrivate *priv = dra_spell_request_factory_get_instance_private(instance);
	return (DraAugmentRequest *) dra_spell_request_new(priv->spell_helper, document, a_new_revision, priv->a_slot_key);
}

static CatStringWo *l_factory_get_slot_key(DraIConnectorRequestFactory *self) {
	DraSpellRequestFactory *instance = DRA_SPELL_REQUEST_FACTORY(self);
	DraSpellRequestFactoryPrivate *priv = dra_spell_request_factory_get_instance_private(instance);
	return priv->a_slot_key;
}


static void l_connector_request_factory_iface_init(DraIConnectorRequestFactoryInterface *iface) {
	iface->createRequest = l_factory_create_request;
	iface->getSlotKey = l_factory_get_slot_key;
}

/********************* end DraIConnectorRequestFactory implementation *********************/



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

