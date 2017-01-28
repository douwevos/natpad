/*
   File:    cowpanel.c
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 29, 2014
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

#include "cowpanel.h"
#include "cowdialog.h"
#include "../cowcontainer.h"
#include "../cowichangelistener.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CowPanel"
#include <logging/catlog.h>

struct _CowPanelPrivate {
	CowDialog *dialog;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_change_listener_iface_init(CowIChangeListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CowPanel, cow_panel, GTK_TYPE_BIN,
		G_ADD_PRIVATE(CowPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(COW_TYPE_ICHANGE_LISTENER, l_change_listener_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_add_notify(CowPanel *self, struct _CowDialog *dialog);
static void l_remove_notify(CowPanel *self);

static void cow_panel_class_init(CowPanelClass *clazz) {

	clazz->addNotify = l_add_notify;
	clazz->removeNotify = l_remove_notify;
	clazz->reconfigure = NULL;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cow_panel_init(CowPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CowPanel *instance = COW_PANEL(object);
	CowPanelPrivate *priv = cow_panel_get_instance_private(instance);
	if (priv->dialog) {
		CowDialog *dialog = priv->dialog;
		priv->dialog = NULL;
		CowContainer *container = cow_dialog_get_edit_container(dialog);
		if (container) {
			cow_container_remove_change_listener(container, (CowIChangeListener *) instance);
		}
	}

	G_OBJECT_CLASS(cow_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cow_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void cow_panel_construct(CowPanel *cow_panel) {
	CowPanelPrivate *priv = cow_panel_get_instance_private(cow_panel);
	priv->dialog = NULL;

	gtk_widget_set_vexpand_set(GTK_WIDGET(cow_panel), TRUE);
	gtk_widget_set_valign(GTK_WIDGET(cow_panel), GTK_ALIGN_FILL);
}

GObject *cow_panel_get_configuration(CowPanel *panel) {
	CowPanelPrivate *priv = cow_panel_get_instance_private(panel);
	GObject *result = NULL;
	if (priv->dialog) {
		CowContainer *e_container = cow_dialog_get_edit_container(priv->dialog);
		if (e_container) {
			result = cow_container_get_configuration(e_container);
		}
	}
	return result;
}

void cow_panel_set_configuration(CowPanel *panel, GObject *configuration) {
	CowPanelPrivate *priv = cow_panel_get_instance_private(panel);
	cat_log_debug("configuration=%p", configuration);
	if (priv->dialog) {
		CowContainer *e_container = cow_dialog_get_edit_container(priv->dialog);
		if (e_container) {
			cow_container_set_configuration(e_container, configuration);
		}
	}
}


static void l_config_changed(CowIChangeListener *self, GObject *new_config);

static void l_add_notify(CowPanel *self, struct _CowDialog *dialog) {
	CowPanelPrivate *priv = cow_panel_get_instance_private(self);
	priv->dialog = dialog;
	CowContainer *container = cow_dialog_get_edit_container(dialog);
	if (container) {
		cow_container_add_change_listener(container, (CowIChangeListener *) self);
		l_config_changed((CowIChangeListener *) self, cow_container_get_configuration(container));
	}
}

static void l_remove_notify(CowPanel *self) {
	CowPanelPrivate *priv = cow_panel_get_instance_private(self);
	if (priv->dialog) {
		CowContainer *container = cow_dialog_get_edit_container(priv->dialog);
		if (container) {
			cow_container_remove_change_listener(container, (CowIChangeListener *) self);
		}
	}
	priv->dialog = NULL;
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

/********************* start CowIChangeListener implementation *********************/

static void l_config_changed(CowIChangeListener *self, GObject *new_config) {
	CowPanelClass *panel_class = COW_PANEL_GET_CLASS(self);
	if (panel_class->reconfigure) {
		CowPanel *panel = COW_PANEL(self);
		panel_class->reconfigure(panel, new_config);
	}
}

static void l_change_listener_iface_init(CowIChangeListenerInterface *iface) {
	iface->configChanged = l_config_changed;
}

/********************* end CowIChangeListener implementation *********************/
