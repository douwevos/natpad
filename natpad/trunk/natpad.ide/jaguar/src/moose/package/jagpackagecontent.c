/*
   File:    jagpackagecontent.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 20, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#include "jagpackagecontent.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPackageContent"
#include <logging/catlog.h>

struct _JagPackageContentPrivate {
	void *dummy;
};


static CatS jag_s_package_content_key = CAT_S_DEF("JagPackageContent");

static void l_content_iface_init(MooIContentInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPackageContent, jag_package_content, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ICONTENT, l_content_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_package_content_class_init(JagPackageContentClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagPackageContentPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_package_content_init(JagPackageContent *instance) {
	JagPackageContentPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_PACKAGE_CONTENT, JagPackageContentPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPackageContent *instance = JAG_PACKAGE_CONTENT(object);
//	JagPackageContentPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPackageContent *jag_package_content_new() {
	JagPackageContent *result = g_object_new(JAG_TYPE_PACKAGE_CONTENT, NULL);
	cat_ref_anounce(result);
//	JagPackageContentPrivate *priv = result->priv;
	return result;
}



CatStringWo *jag_package_content_key() {
	return CAT_S(jag_s_package_content_key);
}


/********************* start MooIContent implementation *********************/


static CatStringWo *l_content_get_key(MooIContent *self) {
	return jag_package_content_key();
}

static MooIContent *l_content_anchor(MooIContent *self, int version) {
	return self;
}



static void l_content_iface_init(MooIContentInterface *iface) {
	iface->getKey = l_content_get_key;
	iface->anchor = l_content_anchor;
}

/********************* end MooIContent implementation *********************/




