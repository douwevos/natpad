/*
   File:    elkexplorer.c
   Project: elk
   Author:  Douwe Vos
   Date:    Nov 23, 2013
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


#include "elkexplorer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkExplorer"
#include <logging/catlog.h>

struct _ElkExplorerPrivate {
	ElkIService *elk_service;
};


G_DEFINE_TYPE(ElkExplorer, elk_explorer, MOO_TYPE_EXPLORER)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_activate_node(MooExplorer *self, MooNodeLayout *node_layout);


static void elk_explorer_class_init(ElkExplorerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(ElkExplorerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	MooExplorerClass *moo_clazz = MOO_EXPLORER_CLASS(clazz);
	moo_clazz->activateNode = l_activate_node;
}

static void elk_explorer_init(ElkExplorer *instance) {
	ElkExplorerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, ELK_TYPE_EXPLORER, ElkExplorerPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkExplorer *instance = ELK_EXPLORER(object);
	ElkExplorerPrivate *priv = instance->priv;
	cat_unref_ptr(priv->elk_service);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkExplorer *elk_explorer_new(ElkIService *elk_service, MooService *moo_service, MooNodeRenderRegistry *render_registry) {
	ElkExplorer *result = g_object_new(ELK_TYPE_EXPLORER, NULL);
	cat_ref_anounce(result);
	ElkExplorerPrivate *priv = result->priv;
	priv->elk_service = cat_ref_ptr(elk_service);
	moo_explorer_construct((MooExplorer *) result, moo_service, render_registry);
	return result;
}




static void l_activate_node(MooExplorer *self, MooNodeLayout *node_layout) {
	ElkExplorerPrivate *priv = ELK_EXPLORER_GET_PRIVATE(self);
	MooNodeWo *node = moo_node_layout_get_node(node_layout);
	ElkIServiceInterface *iface = ELK_ISERVICE_GET_INTERFACE(priv->elk_service);
	if (iface->openNewFileEditor) {
		iface->openNewFileEditor(priv->elk_service, node);
	}
}


