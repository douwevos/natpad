/*
   File:    chechameleoneditorfactory.c
   Project: cheetah
   Author:  Douwe Vos
   Date:    Mar 22, 2015
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

#include "chechameleoneditorfactory.h"
#include "cheeditorpanel.h"
#include <elk.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CheChameleonEditorFactory"
#include <logging/catlog.h>

struct _CheChameleonEditorFactoryPrivate {
	LeaIPanelOwner *panel_owner;
	DraIConnectorRequestFactory *connector;
	ElkDocumentIO *document_io;
	VipIFile *document_file;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_resource_factory_iface_init(ElkIResourceEditorFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CheChameleonEditorFactory, che_chameleon_editor_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(CheChameleonEditorFactory)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IRESOURCE_EDITOR_FACTORY, l_resource_factory_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void che_chameleon_editor_factory_class_init(CheChameleonEditorFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void che_chameleon_editor_factory_init(CheChameleonEditorFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CheChameleonEditorFactory *instance = CHE_CHAMELEON_EDITOR_FACTORY(object);
	CheChameleonEditorFactoryPrivate *priv = che_chameleon_editor_factory_get_instance_private(instance);
	cat_unref_ptr(priv->panel_owner);
	cat_unref_ptr(priv->connector);
	cat_unref_ptr(priv->document_io);
	cat_unref_ptr(priv->document_file);
	G_OBJECT_CLASS(che_chameleon_editor_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(che_chameleon_editor_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CheChameleonEditorFactory *che_chameleon_editor_factory_new(LeaIPanelOwner *panel_owner, DraIConnectorRequestFactory *connector, ElkDocumentIO *document_io, VipIFile *document_file) {
	CheChameleonEditorFactory *result = g_object_new(CHE_TYPE_CHAMELEON_EDITOR_FACTORY, NULL);
	cat_ref_anounce(result);
	CheChameleonEditorFactoryPrivate *priv = che_chameleon_editor_factory_get_instance_private(result);
	priv->panel_owner = cat_ref_ptr(panel_owner);
	priv->connector = cat_ref_ptr(connector);
	priv->document_io = cat_ref_ptr(document_io);
	priv->document_file = cat_ref_ptr(document_file);
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	CheChameleonEditorFactory *instance = CHE_CHAMELEON_EDITOR_FACTORY(self);
//	CheChameleonEditorFactoryPrivate *priv = che_chameleon_editor_factory_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/



/********************* start ElkIResourceEditorFactory implementation *********************/

static GtkWidget *l_create_editor(ElkIResourceEditorFactory *self, MooNodeWo *moo_node) {
	CheChameleonEditorFactoryPrivate *priv = che_chameleon_editor_factory_get_instance_private((CheChameleonEditorFactory *) self);

	ElkDocumentBin *document_bin = elk_document_io_open_document_for_file(priv->document_io, priv->document_file);
	CheEditorPanel *result = che_editor_panel_new(priv->panel_owner, document_bin, priv->connector);


//	AstDocument *document = elk_document_io_read_vip_file(priv->document_io, priv->file_to_open);
//
//	CatStringWo *a_markup_hook_key = CAT_S(l_s_txt_markup_hook_key);
//
//	result = (AstEditor *) che_editor_new(priv->elk_service, a_markup_hook_key, document, FALSE);
//
//	elk_highlighter_map_connect_editor(ELK_HIGHLIGHTER_MAP(priv->highlighter_map), result, a_markup_hook_key);
//
//	elk_editor_set_moose_node(ELK_EDITOR(result), moo_node);
//
//	cat_unref_ptr(document);
//	cat_unref_ptr(a_markup_hook_key);
	return (GtkWidget *) result;
}


static void l_resource_factory_iface_init(ElkIResourceEditorFactoryInterface *iface) {
	iface->createEditor = l_create_editor;
}

/********************* end ElkIResourceEditorFactory implementation *********************/
