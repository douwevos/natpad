/*
   File:    cowpanelmodel.c
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 27, 2014
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

#include "cowpanelmodelprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CowPanelModel"
#include <logging/catlog.h>

struct _CowPanelModelPrivate {
	CatArrayWo *panel_factories;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CowPanelModel, cow_panel_model, G_TYPE_OBJECT,
		G_ADD_PRIVATE(CowPanelModel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cow_panel_model_class_init(CowPanelModelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cow_panel_model_init(CowPanelModel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CowPanelModel *instance = COW_PANEL_MODEL(object);
	CowPanelModelPrivate *priv = cow_panel_model_get_instance_private(instance);
	cat_unref_ptr(priv->panel_factories);
	G_OBJECT_CLASS(cow_panel_model_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cow_panel_model_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CowPanelModel *cow_panel_model_new() {
	CowPanelModel *result = g_object_new(COW_TYPE_PANEL_MODEL, NULL);
	cat_ref_anounce(result);
	CowPanelModelPrivate *priv = cow_panel_model_get_instance_private(result);
	priv->panel_factories = cat_array_wo_new();
	return result;
}


void cow_panel_model_add_factory(CowPanelModel *model, CowIPanelFactory *panel_factory) {
	CowPanelModelPrivate *priv = cow_panel_model_get_instance_private(model);
	int index = cat_array_wo_find_index(priv->panel_factories, panel_factory, -1);
	if (index>=0) {
		return;
	}
	cat_array_wo_append(priv->panel_factories, (GObject *) panel_factory);
}

void cow_panel_model_remove_factory(CowPanelModel *model, CowIPanelFactory *panel_factory) {
	CowPanelModelPrivate *priv = cow_panel_model_get_instance_private(model);
	int index = cat_array_wo_find_index(priv->panel_factories, panel_factory, -1);
	if (index>=0) {
		cat_array_wo_remove(priv->panel_factories, index, NULL);
	}
}

CatIIterator *cow_panel_model_factory_iterator(CowPanelModel *model) {
	CowPanelModelPrivate *priv = cow_panel_model_get_instance_private(model);
	return cat_array_wo_iterator(priv->panel_factories);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CowPanelModel *instance = COW_PANEL_MODEL(self);
	CowPanelModelPrivate *priv = cow_panel_model_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p] panel-factories=%o", iname, self, priv->panel_factories);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

