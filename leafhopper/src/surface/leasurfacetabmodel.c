/*
   File:    leasurfacetabmodel.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 18, 2014
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

#include "leasurfacetabmodel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaSurfaceTabModel"
#include <logging/catlog.h>


struct _LeaSurfaceTabModelPrivate {
	int tab_index_selected;
	CatArrayWo *e_tabs;
	CatWeakList *listeners;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaSurfaceTabModel, lea_surface_tab_model, G_TYPE_OBJECT,
		G_ADD_PRIVATE(LeaSurfaceTabModel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_surface_tab_model_class_init(LeaSurfaceTabModelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_surface_tab_model_init(LeaSurfaceTabModel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaSurfaceTabModel *instance = LEA_SURFACE_TAB_MODEL(object);
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(instance);
	cat_unref_ptr(priv->e_tabs);
	cat_unref_ptr(priv->listeners);
	G_OBJECT_CLASS(lea_surface_tab_model_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_surface_tab_model_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaSurfaceTabModel *lea_surface_tab_model_new() {
	LeaSurfaceTabModel *result = g_object_new(LEA_TYPE_SURFACE_TAB_MODEL, NULL);
	cat_ref_anounce(result);
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(result);
	priv->e_tabs = cat_array_wo_new();
	priv->listeners = cat_weak_list_new();
	priv->tab_index_selected = -1;
	return result;
}


void lea_surface_tab_model_add_tab(LeaSurfaceTabModel *tab_model, LeaSurfaceTab *tab) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	gboolean notify_tab_selected = FALSE;
	int index = cat_array_wo_append(priv->e_tabs, (GObject *) tab);
	if (priv->tab_index_selected<0) {
		priv->tab_index_selected = index;
		notify_tab_selected = TRUE;
	}
	cat_log_debug("index=%d, notify_tab_selected=%d, tab=%o", index, notify_tab_selected, tab);

	CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
	while(cat_iiterator_has_next(iter)) {
		LeaITabModelListener *listener = (LeaITabModelListener *) cat_iiterator_next(iter);
		cat_log_trace("notifying tab_added to %o", listener);
		lea_itab_model_listener_tab_added(listener, tab);
		if (notify_tab_selected) {
			cat_log_trace("notifying tab_selected to %o", listener);
			lea_itab_model_listener_tab_selected(listener, priv->tab_index_selected, tab);
		}
	}
	cat_unref_ptr(iter);
	cat_log_trace("done");
}



void lea_surface_tab_model_add_tab_at(LeaSurfaceTabModel *tab_model, LeaSurfaceTab *tab, int index) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	if (index<0 || index>=cat_array_wo_size(priv->e_tabs)) {
		lea_surface_tab_model_add_tab(tab_model, tab);
		return;
	}

	gboolean notify_tab_selected = FALSE;
	cat_array_wo_insert(priv->e_tabs, (GObject *) tab, index);
	if (priv->tab_index_selected<0) {
		priv->tab_index_selected = index;
		notify_tab_selected = TRUE;
	} else if (priv->tab_index_selected>=index) {
		priv->tab_index_selected++;
	}
	cat_log_debug("index=%d, notify_tab_selected=%d, tab=%o", index, notify_tab_selected, tab);

	CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
	while(cat_iiterator_has_next(iter)) {
		LeaITabModelListener *listener = (LeaITabModelListener *) cat_iiterator_next(iter);
		cat_log_trace("notifying tab_added to %o", listener);
		lea_itab_model_listener_tab_added(listener, tab);
		if (notify_tab_selected) {
			cat_log_trace("notifying tab_selected to %o", listener);
			lea_itab_model_listener_tab_selected(listener, priv->tab_index_selected, tab);
		}
	}
	cat_unref_ptr(iter);
	cat_log_trace("done");
}



void lea_surface_tab_model_remove_tab_at(LeaSurfaceTabModel *tab_model, int idx) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	gboolean notify_tab_selected = FALSE;
	LeaSurfaceTab *tab = NULL;
	LeaSurfaceTab *tab_selected = NULL;
	cat_array_wo_remove(priv->e_tabs, idx, (GObject **) (&tab));
	if (priv->tab_index_selected==idx) {
		if (priv->tab_index_selected>=cat_array_wo_size(priv->e_tabs)) {
			priv->tab_index_selected--;
		}
		if (priv->tab_index_selected>=0) {
			tab_selected = (LeaSurfaceTab *) cat_array_wo_get(priv->e_tabs, priv->tab_index_selected);
		}
		notify_tab_selected = TRUE;
	} else if (idx<priv->tab_index_selected) {
		priv->tab_index_selected--;
	}
	cat_log_debug("index=%d, notify_tab_selected=%d, tab=%o", idx, notify_tab_selected, tab);

	CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
	while(cat_iiterator_has_next(iter)) {
		LeaITabModelListener *listener = (LeaITabModelListener *) cat_iiterator_next(iter);
		cat_log_trace("notifying tab_added to %o", listener);
		lea_itab_model_listener_tab_removed(listener, tab);
		if (notify_tab_selected) {
			cat_log_trace("notifying tab_selected to %o", listener);
			lea_itab_model_listener_tab_selected(listener, priv->tab_index_selected, tab_selected);
		}
	}
	cat_unref_ptr(iter);
	cat_unref_ptr(tab);
	cat_log_trace("done");
}


LeaSurfaceTab *lea_surface_tab_model_get_tab_at(LeaSurfaceTabModel *tab_model, int idx) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	LeaSurfaceTab *result = NULL;
	if ((priv->e_tabs!=NULL) && idx>=0 && idx<cat_array_wo_size(priv->e_tabs)) {
		result = (LeaSurfaceTab *) cat_array_wo_get(priv->e_tabs, idx);
	}
	return result;
}

int lea_surface_tab_model_count(LeaSurfaceTabModel *tab_model) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	return priv->e_tabs==NULL ? 0 : cat_array_wo_size(priv->e_tabs);
}

int lea_surface_tab_model_find_tab_owning_widget(LeaSurfaceTabModel *tab_model, GtkWidget *widget_on_tab) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	if (priv->e_tabs==NULL) {
		return -1;
	}
	CatIIterator *iter = cat_array_wo_iterator(priv->e_tabs);
	int found_index = -1;
	int index = 0;
	while(cat_iiterator_has_next(iter)) {
		LeaSurfaceTab *tab = (LeaSurfaceTab *) cat_iiterator_next(iter);
		if (lea_surface_tab_get_widget(tab) == widget_on_tab) {
			found_index = index;
			break;
		}
		index++;
	}
	cat_unref_ptr(iter);
	return found_index;
}


CatIIterator *lea_surface_tab_model_iterator(LeaSurfaceTabModel *tab_model) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	return priv->e_tabs==NULL ? NULL : cat_array_wo_iterator(priv->e_tabs);
}

LeaSurfaceTab *lea_surface_tab_model_get_selected_tab(LeaSurfaceTabModel *tab_model) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	LeaSurfaceTab *result = NULL;
	if (priv->tab_index_selected>=0 || priv->tab_index_selected<cat_array_wo_size(priv->e_tabs)) {
		result = (LeaSurfaceTab *) cat_array_wo_get(priv->e_tabs, priv->tab_index_selected);
	}
	return result;
}

int lea_surface_tab_model_get_selected_tab_index(LeaSurfaceTabModel *tab_model) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	return priv->tab_index_selected;
}


void lea_surface_tab_model_set_selected_tab(LeaSurfaceTabModel *tab_model, int index) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	if (priv->tab_index_selected != index) {
		if (priv->tab_index_selected>=0 || priv->tab_index_selected<cat_array_wo_size(priv->e_tabs)) {
			LeaSurfaceTab *old_tab = (LeaSurfaceTab *) cat_array_wo_get(priv->e_tabs, priv->tab_index_selected);
			GtkWidget *old_widget = lea_surface_tab_get_widget(old_tab);
			gtk_widget_hide(old_widget);
		}
		priv->tab_index_selected = index;
	}

	if (priv->e_tabs) {
		LeaSurfaceTab *tab = (LeaSurfaceTab *) cat_array_wo_get(priv->e_tabs, priv->tab_index_selected);
		CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
		while(cat_iiterator_has_next(iter)) {
			LeaITabModelListener *listener = (LeaITabModelListener *) cat_iiterator_next(iter);
			lea_itab_model_listener_tab_selected(listener, priv->tab_index_selected, tab);
		}
		cat_unref_ptr(iter);
	}
}



void lea_surface_tab_model_add_listener(LeaSurfaceTabModel *tab_model, LeaITabModelListener *listener) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	cat_weak_list_append_once(priv->listeners, (GObject *) listener);

}

void lea_surface_tab_model_remove_listener(LeaSurfaceTabModel *tab_model, LeaITabModelListener *listener) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);
	cat_weak_list_remove(priv->listeners, (GObject *) listener);
}


void lea_surface_tab_model_destroy(LeaSurfaceTabModel *tab_model) {
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(tab_model);

	CatIIterator *iter = cat_array_wo_iterator(priv->e_tabs);
	while(cat_iiterator_has_next(iter)) {
		LeaSurfaceTab *tab = (LeaSurfaceTab *) cat_iiterator_next(iter);
		GtkWidget *widget = lea_surface_tab_get_widget(tab);
		cat_ref_ptr(widget);
		lea_surface_tab_unset_widget(tab);
		gtk_widget_unparent(widget);
		gtk_widget_destroy(widget);
	}
	cat_unref_ptr(iter);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaSurfaceTabModel *instance = LEA_SURFACE_TAB_MODEL(self);
	LeaSurfaceTabModelPrivate *priv = lea_surface_tab_model_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p tab_index_selected=%d, count=%d]", iname, self, priv->tab_index_selected, cat_array_wo_size(priv->e_tabs));
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
