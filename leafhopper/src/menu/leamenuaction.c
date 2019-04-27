/*
   File:    leamenuaction.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    2013
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

#include "leamenuaction.h"
#include "../action/leaactionmenuitem.h"
#include "../action/leaactiongroup.h"
#include "../action/leaactionprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaMenuAction"
#include <logging/catlog.h>

struct _LeaMenuActionPrivate {
	LeaAction *action;
	LeaMenuSetupType menu_type;
	CatArrayWo *e_children;				// <LeaMenuAction *>
	GtkWidget *menu_item;
	GtkWidget *menu_shell;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_attachable_iface_init(LeaIAttachableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaMenuAction, lea_menu_action, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(LeaMenuAction)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IATTACHABLE, l_attachable_iface_init);
});

static void l_dispose(GObject *object);
static void l_widget_map_cb(GtkWidget *widget, gpointer data);

static void lea_menu_action_class_init(LeaMenuActionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void lea_menu_action_init(LeaMenuAction *node) {
}

static void l_week_notify(gpointer data, GObject *ptr_of_obj) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaMenuAction *instance = LEA_MENU_ACTION(object);
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(instance);
	if (priv->menu_item) {
		g_object_weak_unref((GObject *) (priv->menu_item), (GWeakNotify) l_week_notify, object);
		priv->menu_item = NULL;
	}
	priv->menu_shell = NULL;
	cat_unref_ptr(priv->action);
	cat_unref_ptr(priv->e_children);

	G_OBJECT_CLASS(lea_menu_action_parent_class)->dispose(object);
	cat_log_detail("end-dispose:%p", object);
}

LeaMenuAction *lea_menu_action_new(LeaAction *action, LeaMenuSetupType setupType) {
	LeaMenuAction *result = g_object_new(LEA_TYPE_MENU_ACTION, NULL);
	cat_ref_anounce(result);
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(result);
	priv->action = cat_ref_ptr(action);
	priv->menu_type = setupType;
	priv->e_children = cat_array_wo_new();
	const char *label = cat_string_wo_getchars(lea_action_get_label(action));
	switch(setupType) {
		case LEA_MENU_TYPE_BAR : {
			priv->menu_shell = gtk_menu_bar_new();
			priv->menu_item = NULL;
		} break;
		case LEA_MENU_TYPE_SUB : {
			priv->menu_shell = gtk_menu_new();
			priv->menu_item = gtk_menu_item_new_with_mnemonic(label);
			g_object_weak_ref((GObject *) (priv->menu_item), (GWeakNotify) l_week_notify, result);
			gtk_menu_item_set_submenu((GtkMenuItem *) priv->menu_item, priv->menu_shell);
		} break;
		case LEA_MENU_TYPE_ITEM : {
			priv->menu_shell = NULL;
			priv->menu_item = (GtkWidget *) lea_action_menu_item_new(action);
			g_object_weak_ref((GObject *) (priv->menu_item), (GWeakNotify) l_week_notify, result);
		} break;
	}
	if (priv->menu_item) {
		gtk_widget_set_sensitive(priv->menu_item, lea_action_is_sensitive(action));
		g_signal_connect(priv->menu_item, "map", (GCallback) l_widget_map_cb, result);
	}


	lea_action_attach(action, (LeaIAttachable *) result);
	return result;
}

LeaAction *lea_menu_action_get_action(LeaMenuAction *menu_action) {
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(menu_action);
	return priv->action;
}

GtkMenuShell *lea_menu_action_get_menu_shell(LeaMenuAction *menu_action) {
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(menu_action);
	return (GtkMenuShell *) priv->menu_shell;
}

GtkWidget *lea_menu_action_get_widget(LeaMenuAction *menu_action) {
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(menu_action);
	return priv->menu_item;
}

static void l_widget_map_cb(GtkWidget *widget, gpointer data) {
	LeaMenuAction *menu_action = LEA_MENU_ACTION(data);

	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(menu_action);
	if (priv->action) {
		LeaActionClass *action_class = LEA_ACTION_GET_CLASS(priv->action);
		if (action_class->refresh) {
			action_class->refresh(priv->action);
		}
	}

}

static GList *l_search(GList *list, void *data, int *pos) {
	int idx = *pos;
	GList *cur = g_list_nth(list, *pos);
	while(cur!=NULL) {
		if (cur->data==data) {
			*pos = idx;
			break;
		}
		cur = cur->next;
		idx++;
		if (cur==list) {
			cur = NULL;
		}
	}
	return cur;
}

/**
 * The Gtk menu shell does not allow to move its children so we get the private data and move it ourself.
 */
typedef struct _LeaMenuShellChildren LeaMenuShellChildren;

struct _LeaMenuShellChildren {
	GList *children;
};

static CatArrayWo *l_create_flattened_list(LeaActionGroup *action_group, LeaKeyContext *active_context, gboolean no_flatten_groups) {
	CatArrayWo *e_ordered = lea_action_group_enlist_ordered(action_group);
	cat_log_debug("ordered=%o", e_ordered);

	CatArrayWo *e_flattened = cat_array_wo_new();
	if (e_ordered == NULL) {
		return e_flattened;
	}
	int idx;
	for(idx=0; idx<cat_array_wo_size(e_ordered); idx++) {
		LeaAction *child = (LeaAction *) cat_array_wo_get(e_ordered, idx);
		cat_log_detail("child=%o", child);
		if (!lea_action_is_visible(child)) {
			continue;
		}
		if (LEA_IS_ACTION_GROUP(child) && !no_flatten_groups) {
			CatArrayWo *e_child_group = lea_action_group_enlist_ordered(LEA_ACTION_GROUP(child));
			if (e_child_group && cat_array_wo_size(e_child_group)>0) {
				cat_array_wo_append(e_flattened, NULL);
				CatIIterator *child_iter = cat_array_wo_iterator(e_child_group);
				while(cat_iiterator_has_next(child_iter)) {
					LeaAction *action = (LeaAction *) cat_iiterator_next(child_iter);
					if (lea_action_is_visible(action)) {
						cat_array_wo_append(e_flattened, (GObject *) action);
					}
				}
				cat_unref_ptr(child_iter);
				cat_array_wo_append(e_flattened, NULL);
			}
			cat_unref_ptr(e_child_group);
		} else {
			cat_array_wo_append(e_flattened, (GObject *) child);
		}
	}
	cat_unref_ptr(e_ordered);

	/* clean up flattened list */
	gboolean last_was_null = TRUE;
	for(idx=0; idx<cat_array_wo_size(e_flattened); idx++) {
		GObject *child = cat_array_wo_get(e_flattened, idx);
		if (child==NULL) {
			if (last_was_null) {
				cat_array_wo_remove(e_flattened, idx, NULL);
				idx--;
			} else {
				last_was_null = TRUE;
			}
		} else {
			last_was_null = FALSE;
		}
	}
	if (cat_array_wo_size(e_flattened)>0 && cat_array_wo_get_last(e_flattened)==NULL) {
		cat_array_wo_remove_last(e_flattened, NULL);
	}
	return e_flattened;
}

static gboolean l_refresh_menu_shell(CatArrayWo *e_children, GtkMenuShell *menu_shell) {
	LeaMenuShellChildren *lmsc = (LeaMenuShellChildren *) menu_shell->priv;

	gboolean result = FALSE;

	int srcidx;
	int dstidx=0;
	for(srcidx=0; srcidx<cat_array_wo_size(e_children); srcidx++) {
		LeaMenuAction *action = (LeaMenuAction *) cat_array_wo_get(e_children, srcidx);
		cat_log_debug("srcidx=%d, action=%o", srcidx, action);
		if (action) {
			GtkWidget *widget = lea_menu_action_get_widget(action);
			int outidx = dstidx;
			GList *main_list = gtk_container_get_children((GtkContainer *) menu_shell);

			GList *elem = l_search(main_list, widget, &outidx);
			if (elem) {
				cat_log_detail("element %p already added %d should be at %d", widget, outidx, dstidx);
				/* element was in container lets check its position */
				if (outidx!=dstidx) {
					/* a little tricky part ... moving the entry */
					void *data = cat_ref_ptr(elem->data);
					main_list = g_list_remove_link(main_list, elem);
					main_list = g_list_insert(main_list, data, dstidx);
					lmsc->children = main_list;
					cat_unref_ptr(data);

				}
			} else {
				cat_log_detail("adding widget=%o to shell=%o", widget, menu_shell);
				/* element was not in the container ... lets add it */
				gtk_menu_shell_insert(menu_shell, widget, dstidx);
			}
			gtk_widget_set_visible(widget, TRUE);
			result = TRUE;
		} else {
			/* should be a seperator in the menu at the dstidx location */
			GList *main_list = gtk_container_get_children((GtkContainer *) menu_shell);
			void *entryptr = g_list_nth_data(main_list, dstidx);
			cat_log_detail("seperator ? ");
			if (!GTK_IS_SEPARATOR_MENU_ITEM(entryptr)) {
				/* separator was not there ... lets add it */
				GtkWidget *widget = gtk_separator_menu_item_new();
				gtk_menu_shell_insert(menu_shell, widget, dstidx);
				gtk_widget_set_visible(widget, TRUE);
				cat_log_detail("seperator added");
			} else {
				gtk_widget_set_visible(GTK_WIDGET(entryptr), TRUE);
			}
		}

		dstidx++;
	}


	/* remove all other elements from the container */
	while(TRUE) {
		GList *main_list = gtk_container_get_children((GtkContainer *) menu_shell);
		GList *link = g_list_nth(main_list, dstidx);
		if (link==NULL) {
			break;
		}
		cat_log_debug("removing extra:%p, dstidx=%d", link->data, dstidx);
		gtk_container_remove(GTK_CONTAINER(menu_shell), GTK_WIDGET(link->data));
	}

	return result;
}

LeaMenuAction *l_find_menu_action(CatArrayWo *e_children, LeaAction *action, int idx) {
	if (action==NULL) {
		if (cat_array_wo_get(e_children, idx)==NULL) {
			if (idx>=0 && idx<cat_array_wo_size(e_children)) {
				return NULL;
			}
		}
		cat_array_wo_insert(e_children, NULL, idx);
		return NULL;
	}
	int index;
	for(index=idx; index<cat_array_wo_size(e_children); index++) {
		LeaMenuAction *menu_action = (LeaMenuAction *) cat_array_wo_get(e_children, index);
		if (menu_action == NULL) {
			continue;
		}
		if (lea_menu_action_get_action(menu_action)==action) {
			if (idx!=index) {
				cat_ref_ptr(menu_action);
				cat_array_wo_remove(e_children, index, NULL);
				cat_array_wo_insert(e_children, (GObject *) menu_action, idx);
				cat_unref(menu_action);
			}
			return menu_action;
		}
	}
	LeaMenuAction *menu_action = lea_menu_action_new(action, LEA_IS_ACTION_GROUP(action) ? LEA_MENU_TYPE_SUB : LEA_MENU_TYPE_ITEM);
	cat_array_wo_insert(e_children, (GObject *) menu_action, idx);
	cat_unref(menu_action);
	return menu_action;
}

static gboolean l_update_children(LeaMenuAction *menu_action, CatArrayWo *e_actions, LeaKeyContext *active_context) {
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(menu_action);
	int idx;
	gboolean result = FALSE;
	for(idx=0; idx<cat_array_wo_size(e_actions); idx++) {
		LeaAction *action = (LeaAction *) cat_array_wo_get(e_actions, idx);
		cat_log_debug("searching:action=%o, idx=%d", action, idx);

		LeaMenuAction *found_menu_action = l_find_menu_action(priv->e_children, action, idx);

		cat_log_debug("found_menu_action=%o, idx=%d", found_menu_action, idx);


		if (found_menu_action) {
			if (lea_menu_action_update(found_menu_action, active_context)) {
				result = TRUE;
			} else {
				cat_array_wo_remove(e_actions, idx, NULL);
				idx--;
			}
		}
	}

	/* remove stuff */
	if (cat_array_wo_size(priv->e_children)>cat_array_wo_size(e_actions)) {
		cat_array_wo_limit(priv->e_children, 0, cat_array_wo_size(e_actions));
	}
	return result;
}

gboolean lea_menu_action_update(LeaMenuAction *menu_action, LeaKeyContext *active_context) {
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(menu_action);
	gboolean result = FALSE;
	cat_log_indent_level++;
	cat_log_debug("menu_action=%o, action=%o", menu_action, priv->action);
	if (LEA_IS_ACTION_GROUP(priv->action)) {
		LeaActionGroup *action_group = (LeaActionGroup *) priv->action;
		GtkMenuShell *menu_shell = lea_menu_action_get_menu_shell(menu_action);
		cat_log_debug("menu_shell=%o", menu_shell);
		if (menu_shell) {
			CatArrayWo *e_flattened = l_create_flattened_list(action_group, active_context, GTK_IS_MENU_BAR(menu_shell));
			if (l_update_children(menu_action, e_flattened, active_context)) {
				result = TRUE;
			}
			if (l_refresh_menu_shell(priv->e_children, menu_shell)) {
				result = TRUE;
			}
			cat_unref_ptr(e_flattened);
			gtk_widget_set_visible((GtkWidget *) menu_shell, result);
		}

		GtkWidget *widget = lea_menu_action_get_widget(menu_action);
		if (widget) {
			gtk_widget_set_visible(widget, result);
		}

	} else {
		/* normal action ... not much to do here */
		result = TRUE;
	}
	cat_log_indent_level--;
	return result;
}


/********************* begin CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaMenuAction *instance = LEA_MENU_ACTION(self);
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: action=%o, type=%d]", iname, self, priv->action, priv->menu_type);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


/********************* begin LeaIAttachable implementation *********************/

static void l_attachable_sensitivity_set(LeaIAttachable *self, gboolean sensitivity) {
	LeaMenuAction *instance = LEA_MENU_ACTION(self);
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(instance);
	if (priv->menu_item) {
		gtk_widget_set_sensitive(priv->menu_item, sensitivity);
	}
}

static void l_attachable_visibility_set(LeaIAttachable *self, gboolean sensitivity) {
	LeaMenuAction *instance = LEA_MENU_ACTION(self);
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(instance);
	if (priv->menu_item) {
		gtk_widget_set_visible(priv->menu_item, sensitivity);
	}
}

static void l_attachable_toggability_set(LeaIAttachable *self, gboolean toggable) {
	LeaMenuAction *instance = LEA_MENU_ACTION(self);
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(instance);
	if (LEA_IS_FULL_MENU_ITEM(priv->menu_item)) {
		lea_full_menu_item_set_toggable((LeaFullMenuItem *) priv->menu_item, toggable);
	}
}

static void l_attachable_toggled(LeaIAttachable *self, gboolean toggled) {
	LeaMenuAction *instance = LEA_MENU_ACTION(self);
	LeaMenuActionPrivate *priv = lea_menu_action_get_instance_private(instance);
	if (LEA_IS_FULL_MENU_ITEM(priv->menu_item)) {
		lea_full_menu_item_set_toggled((LeaFullMenuItem *) priv->menu_item, toggled);
	}
}

static void l_attachable_iface_init(LeaIAttachableInterface *iface) {
	iface->sensitivitySet = l_attachable_sensitivity_set;
	iface->visibilitySet = l_attachable_visibility_set;
	iface->toggabilitySet = l_attachable_toggability_set;
	iface->toggled = l_attachable_toggled;
}

/********************* end LeaIAttachable implementation *********************/
