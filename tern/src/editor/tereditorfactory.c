/*
   File:    tereditorfactory.c
   Project: tern
   Author:  Douwe Vos
   Date:    Mar 12, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "tereditorfactory.h"
#include "tereditorpanel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerEditorFactory"
#include <logging/catlog.h>

struct _TerEditorFactoryPrivate {
	LeaIPanelOwner *panel_owner;
	ElkDocumentIO *document_io;
	VipIFile *document_file;
	TerEditorConnectorMap *connector_map;
	TerGrammarMap *grammar_map;
	CatHashMapWo *a_editor_prefs_map;
	CatStringWo *a_clazz_name;
};

static void l_resource_factory_iface_init(ElkIResourceEditorFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerEditorFactory, ter_editor_factory, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(TerEditorFactory)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IRESOURCE_EDITOR_FACTORY, l_resource_factory_iface_init)
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_editor_factory_class_init(TerEditorFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_editor_factory_init(TerEditorFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerEditorFactory *instance = TER_EDITOR_FACTORY(object);
	TerEditorFactoryPrivate *priv = ter_editor_factory_get_instance_private(instance);
	cat_unref_ptr(priv->panel_owner);
	cat_unref_ptr(priv->document_io);
	cat_unref_ptr(priv->document_file);
	cat_unref_ptr(priv->a_editor_prefs_map);
	cat_unref_ptr(priv->a_clazz_name);
	cat_unref_ptr(priv->grammar_map);

	G_OBJECT_CLASS(ter_editor_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_editor_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

TerEditorFactory *ter_editor_factory_new(LeaIPanelOwner *panel_owner, ElkDocumentIO *document_io, TerEditorConnectorMap *connector_map, TerGrammarMap *grammar_map, VipIFile *document_file, CatHashMapWo *a_editor_prefs_map, CatStringWo *a_clazz_name) {
	TerEditorFactory *result = g_object_new(TER_TYPE_EDITOR_FACTORY, NULL);
	cat_ref_anounce(result);
	TerEditorFactoryPrivate *priv = ter_editor_factory_get_instance_private(result);
	priv->panel_owner = cat_ref_ptr(panel_owner);
	priv->document_io = cat_ref_ptr(document_io);
	priv->document_file = cat_ref_ptr(document_file);
	priv->a_editor_prefs_map = cat_ref_ptr(a_editor_prefs_map);
	priv->a_clazz_name = cat_ref_ptr(a_clazz_name);
	priv->connector_map = cat_ref_ptr(connector_map);
	priv->grammar_map = cat_ref_ptr(grammar_map);
	return result;
}


/********************* start ElkIResourceEditorFactory implementation *********************/

static GtkWidget *l_create_editor(ElkIResourceEditorFactory *self, MooNodeWo *moo_node) {
	TerEditorFactory *instance = TER_EDITOR_FACTORY(self);
	TerEditorFactoryPrivate *priv = ter_editor_factory_get_instance_private(instance);

	TerEditorConnector *connector = ter_editor_connector_map_get_connector(priv->connector_map, priv->a_clazz_name);
	ElkDocumentBin *document_bin = elk_document_io_open_document_for_file(priv->document_io, priv->document_file);
	TerEditorPanel *result = ter_editor_panel_new(priv->panel_owner, document_bin, priv->a_clazz_name, connector);

//	TerHighlighterMap *highlighter_map = ter_highlighter_map_container_get_map(priv->map_container, priv->a_clazz_name);
//	cat_log_debug("highlighter_map=%o", highlighter_map);
//	ter_editor_panel_set_highlighter_map(result, highlighter_map);
	return (GtkWidget *) result;
}

static void l_resource_factory_iface_init(ElkIResourceEditorFactoryInterface *iface) {
	iface->createEditor = l_create_editor;
}

/********************* end ElkIResourceEditorFactory implementation *********************/
