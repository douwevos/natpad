/*
   File:    mooexplorerservice.c
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Nov 24, 2013
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


#include "mooexplorerservice.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooExplorerService"
#include <logging/catlog.h>

struct _MooExplorerServicePrivate {
	MooService *moo_service;
	LeaFrame *frame;
	MooPanelOwner *panel_owner;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooExplorerService, moo_explorer_service, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_explorer_service_class_init(MooExplorerServiceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_explorer_service_init(MooExplorerService *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooExplorerService *instance = MOO_EXPLORER_SERVICE(object);
	MooExplorerServicePrivate *priv = moo_explorer_service_get_instance_private(instance);
	cat_unref_ptr(priv->frame);
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->panel_owner);
	G_OBJECT_CLASS(moo_explorer_service_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_explorer_service_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MooExplorerService *moo_explorer_service_new(MooService *moo_service, LeaFrame *frame) {
	MooExplorerService *result = g_object_new(MOO_TYPE_EXPLORER_SERVICE, NULL);
	cat_ref_anounce(result);
	MooExplorerServicePrivate *priv = moo_explorer_service_get_instance_private(result);
	priv->frame = cat_ref_ptr(frame);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->panel_owner = moo_panel_owner_new(moo_service, frame);
	return result;
}

MooPanelOwner *moo_explorer_service_get_panel_owner(MooExplorerService *explorer_service) {
	MooExplorerServicePrivate *priv = moo_explorer_service_get_instance_private(explorer_service);
	return priv->panel_owner;
}
