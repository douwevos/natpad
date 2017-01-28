/*
   File:    spieditorfactory.c
   Project: spider
   Author:  Douwe Vos
   Date:    Jul 18, 2016
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

#include "spieditorfactory.h"
#include "spieditorpanel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "SpiEditorFactory"
#include <logging/catlog.h>

struct _SpiEditorFactoryPrivate {
	LeaIPanelOwner *panel_owner;
	SpiEditorConnector *connector;
	ElkDocumentIO *document_io;
	VipIFile *document_file;
};

static void l_resource_factory_iface_init(ElkIResourceEditorFactoryInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(SpiEditorFactory, spi_editor_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(SpiEditorFactory)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IRESOURCE_EDITOR_FACTORY, l_resource_factory_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void spi_editor_factory_class_init(SpiEditorFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void spi_editor_factory_init(SpiEditorFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	SpiEditorFactory *instance = SPI_EDITOR_FACTORY(object);
	SpiEditorFactoryPrivate *priv = spi_editor_factory_get_instance_private(instance);
	cat_unref_ptr(priv->document_file);
	cat_unref_ptr(priv->document_io);
	cat_unref_ptr(priv->connector);
	cat_unref_ptr(priv->panel_owner);
	G_OBJECT_CLASS(spi_editor_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(spi_editor_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

SpiEditorFactory *spi_editor_factory_new(LeaIPanelOwner *panel_owner, SpiEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file) {
	SpiEditorFactory *result = g_object_new(SPI_TYPE_EDITOR_FACTORY, NULL);
	cat_ref_anounce(result);
	SpiEditorFactoryPrivate *priv = spi_editor_factory_get_instance_private(result);
	priv->panel_owner = cat_ref_ptr(panel_owner);
	priv->connector = cat_ref_ptr(connector);
	priv->document_io = cat_ref_ptr(document_io);
	priv->document_file = cat_ref_ptr(document_file);
	return result;
}


/********************* start ElkIResourceEditorFactory implementation *********************/

static GtkWidget *l_create_editor(ElkIResourceEditorFactory *self, MooNodeWo *moo_node) {
	SpiEditorFactoryPrivate *priv = spi_editor_factory_get_instance_private((SpiEditorFactory *) self);

	cat_log_debug("this=%p, priv->io=%o, priv=%p", self , priv->document_io, priv);

	ElkDocumentBin *document_bin = elk_document_io_open_document_for_file(priv->document_io, priv->document_file);
	SpiEditorPanel *result = spi_editor_panel_new(priv->panel_owner, document_bin, priv->connector);
	elk_editor_panel_set_moose_node((ElkEditorPanel *) result, moo_node);
	return (GtkWidget *) result;
}

static GtkWidget *l_create_outline(ElkIResourceEditorFactory *self, MooNodeWo *moo_node, GtkWidget *editor) {
//	ElkDocumentBin *document_bin = elk_document_io_open_document_for_file(priv->document_io, priv->document_file);
	return NULL;
}



static void l_resource_factory_iface_init(ElkIResourceEditorFactoryInterface *iface) {
	iface->createEditor = l_create_editor;
	iface->createOutline = l_create_outline;
}

/********************* end ElkIResourceEditorFactory implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	SpiEditorFactory *instance = SPI_EDITOR_FACTORY(self);
	SpiEditorFactoryPrivate *priv = spi_editor_factory_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, document_file=%O]", iname, self, priv->document_file);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
