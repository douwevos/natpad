/*
   File:    cowpaneldescription.c
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 30, 2014
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

#include "cowpaneldescription.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CowPanelDescription"
#include <logging/catlog.h>

struct _CowPanelDescriptionPrivate {
	CatStringWo *entry_name;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CowPanelDescription, cow_panel_description, G_TYPE_OBJECT,
		G_ADD_PRIVATE(CowPanelDescription)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cow_panel_description_class_init(CowPanelDescriptionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cow_panel_description_init(CowPanelDescription *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CowPanelDescription *instance = COW_PANEL_DESCRIPTION(object);
	CowPanelDescriptionPrivate *priv = cow_panel_description_get_instance_private(instance);
	cat_unref_ptr(priv->entry_name);
	G_OBJECT_CLASS(cow_panel_description_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cow_panel_description_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void cow_panel_description_construct(CowPanelDescription *description, CatStringWo *c_entry_name) {
	CowPanelDescriptionPrivate *priv = cow_panel_description_get_instance_private(description);
	priv->entry_name = c_entry_name;
}

CowPanelDescription *cow_panel_description_new(CatStringWo *c_entry_name) {
	CowPanelDescription *result = g_object_new(COW_TYPE_PANEL_DESCRIPTION, NULL);
	cat_ref_anounce(result);
	cow_panel_description_construct(result, c_entry_name);
	return result;
}


CatStringWo *cow_panel_description_get_entry_name(CowPanelDescription *panel_descr) {
	CowPanelDescriptionPrivate *priv = cow_panel_description_get_instance_private(panel_descr);
	return priv->entry_name;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CowPanelDescription *instance = COW_PANEL_DESCRIPTION(self);
	CowPanelDescriptionPrivate *priv = cow_panel_description_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p] entry_name=%o", iname, self, priv->entry_name);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/







