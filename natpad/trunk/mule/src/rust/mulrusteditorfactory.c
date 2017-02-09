/*
   File:    mulrusteditorfactory.c
   Project: mule
   Author:  Douwe Vos
   Date:    Jan 9, 2017
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

#include "mulrusteditorfactory.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulRustEditorFactory"
#include <logging/catlog.h>

struct _MulRustEditorFactoryPrivate {
	LeaIPanelOwner *panel_owner;
	MulRustEditorConnector *connector;
	ElkDocumentIO *document_io;
	VipIFile *document_file;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_resource_factory_iface_init(ElkIResourceEditorFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulRustEditorFactory, mul_rust_editor_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(MulRustEditorFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IRESOURCE_EDITOR_FACTORY, l_resource_factory_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_rust_editor_factory_class_init(MulRustEditorFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_rust_editor_factory_init(MulRustEditorFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulRustEditorFactory *instance = MUL_RUST_EDITOR_FACTORY(object);
	MulRustEditorFactoryPrivate *priv = mul_rust_editor_factory_get_instance_private(instance);
	cat_unref_ptr(priv->panel_owner);
	cat_unref_ptr(priv->connector);
	cat_unref_ptr(priv->document_io);
	cat_unref_ptr(priv->document_file);
	G_OBJECT_CLASS(mul_rust_editor_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_rust_editor_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MulRustEditorFactory *mul_rust_editor_factory_new(LeaIPanelOwner *panel_owner, MulRustEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file) {
	MulRustEditorFactory *result = g_object_new(MUL_TYPE_RUST_EDITOR_FACTORY, NULL);
	cat_ref_anounce(result);
	MulRustEditorFactoryPrivate *priv = mul_rust_editor_factory_get_instance_private(result);
	priv->panel_owner = cat_ref_ptr(panel_owner);
	priv->connector = cat_ref_ptr(connector);
	priv->document_io = cat_ref_ptr(document_io);
	priv->document_file = cat_ref_ptr(document_file);
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

/********************* start ElkIResourceEditorFactory implementation *********************/

static GtkWidget *l_create_editor(ElkIResourceEditorFactory *self, MooNodeWo *moo_node) {
	MulRustEditorFactoryPrivate *priv = mul_rust_editor_factory_get_instance_private((MulRustEditorFactory *) self);
	ElkDocumentBin *document_bin = elk_document_io_open_document_for_file(priv->document_io, priv->document_file);
	ElkEditorPanel *result = elk_editor_panel_new(priv->panel_owner, document_bin, (DraIConnectorRequestFactory *) priv->connector);
	return (GtkWidget *) result;
}

static void l_resource_factory_iface_init(ElkIResourceEditorFactoryInterface *iface) {
	iface->createEditor = l_create_editor;
}

/********************* end ElkIResourceEditorFactory implementation *********************/
