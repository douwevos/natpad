/*
   File:    leaactionmenuitem.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "leaactionmenuitem.h"
#include "leaiattachable.h"
#include "../key/leaikeybinding.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaActionMenuItem"
#include <logging/catlog.h>

static void l_attachable_iface_init(LeaIAttachableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaActionMenuItem, lea_action_menu_item, LEA_TYPE_FULL_MENU_ITEM, { // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IATTACHABLE, l_attachable_iface_init)
});

static void l_dispose(GObject *object);


static void lea_action_menu_item_class_init(LeaActionMenuItemClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void lea_action_menu_item_init(LeaActionMenuItem *node) {
}

static void l_dispose(GObject *object) {
	LeaActionMenuItem *instance = LEA_ACTION_MENU_ITEM(object);

	if (!instance->disposed) {
		instance->disposed = TRUE;
		G_OBJECT_CLASS(lea_action_menu_item_parent_class)->dispose(object);

		if (instance->action) {
			LeaAction *action = instance->action;
			instance->action = NULL;
			lea_action_detach(action, LEA_IATTACHABLE(instance));
			cat_unref_ptr(action);
		}
	}
}

static void l_update_label(LeaActionMenuItem *menu_item) {
	GtkWidget *child = gtk_bin_get_child(GTK_BIN(menu_item));

	if (GTK_IS_LABEL(child)) {
		CatStringWo *a_label = lea_action_get_label(menu_item->action);
		const char *lab_text = (a_label != NULL) ? cat_string_wo_getchars(a_label) : "";
		cat_log_detail("label=%s", lab_text);
		gtk_label_set_text_with_mnemonic(GTK_LABEL (child), lab_text);
	} else {
		cat_log_warn("NO BIN YET");
	}
	LeaIKeyBinding *key_binding = LEA_IKEY_BINDING(menu_item->action);
	cat_log_debug("key_binding=%o, menu_item->action=%o", key_binding, menu_item->action);
	LeaKeySequence *key_sequence = lea_ikey_binding_get_key_sequence(key_binding);
	cat_log_debug("key_sequence=%o", key_sequence);
	lea_full_menu_item_set_key_sequence(LEA_FULL_MENU_ITEM(menu_item), key_sequence);
}

static void l_activate(GtkWidget *widget, LeaActionMenuItem *menu_item) {
	LeaAction *action = LEA_ACTION(menu_item->action);
	LeaActionClass *action_class = LEA_ACTION_GET_CLASS(action);
	action_class->action_run(action);

}

LeaActionMenuItem *lea_action_menu_item_new(LeaAction *action) {
	LeaActionMenuItem *result = g_object_new(LEA_TYPE_ACTION_MENU_ITEM, NULL);
	cat_ref_anounce(result);
	result->disposed = FALSE;
	lea_full_menu_item_construct(LEA_FULL_MENU_ITEM(result));

	lea_action_attach(action, LEA_IATTACHABLE(result));

	result->action = cat_ref_ptr(action);
	l_update_label(result);
	
	lea_full_menu_item_set_stock_id(LEA_FULL_MENU_ITEM(result), lea_action_get_stock_id(action));
	
	g_signal_connect(result, "activate", (GCallback) l_activate, result);
	return result;
}


static void l_attachable_sensitivity_set(LeaIAttachable *self, gboolean flag) {
	gtk_widget_set_sensitive(GTK_WIDGET(self), flag);
}
static void l_attachable_iface_init(LeaIAttachableInterface *iface) {
	iface->sensitivitySet = l_attachable_sensitivity_set;
}
