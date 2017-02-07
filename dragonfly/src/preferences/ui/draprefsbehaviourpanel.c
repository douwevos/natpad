/*
   File:    draprefsbehavourpanel.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jan 22, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "draprefsbehaviourpanel.h"
#include "../drapreferenceswo.h"

#include "dragladeprefsbehaviour.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "DraPrefsBehaviourPanel"
#include <logging/catlog.h>

struct _DraPrefsBehaviourPanelPrivate {
	CowIEntryAccessor *entry_accessor;
	CatWo *e_prefs;
	ChaPreferencesWo *e_ast_prefs;

	GtkWidget *w_cb_block_selection;
	unsigned long s_block_selection;

	GtkWidget *w_cb_limit_cursor;
	unsigned long s_limit_cursor;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraPrefsBehaviourPanel, dra_prefs_behaviour_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(DraPrefsBehaviourPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);

static void dra_prefs_behaviour_panel_class_init(DraPrefsBehaviourPanelClass *clazz) {
	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_prefs_behaviour_panel_init(DraPrefsBehaviourPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraPrefsBehaviourPanel *instance = DRA_PREFS_BEHAVIOUR_PANEL(object);
//	DraPrefsBehaviourPanelPrivate *priv = dra_prefs_behaviour_panel_get_instance_private(instance);
	G_OBJECT_CLASS(dra_prefs_behaviour_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_prefs_behaviour_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static gboolean l_block_selection_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_limit_cursor_toggled(GtkToggleButton *toggle_button, gpointer user_data);

DraPrefsBehaviourPanel *dra_prefs_behaviour_panel_new(CowIEntryAccessor *entry_accessor) {
	DraPrefsBehaviourPanel *result = g_object_new(DRA_TYPE_PREFS_BEHAVIOUR_PANEL, NULL);
	cat_ref_anounce(result);
	DraPrefsBehaviourPanelPrivate *priv = dra_prefs_behaviour_panel_get_instance_private(result);
	priv->entry_accessor = cat_ref_ptr(entry_accessor);
	priv->e_ast_prefs = NULL;
	priv->e_prefs = NULL;
	cow_panel_construct((CowPanel *) result);

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) dragladeprefsbehaviour_glade, (gsize) dragladeprefsbehaviour_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);

	gtk_container_add((GtkContainer *) result, frame);

	priv->w_cb_block_selection = (GtkWidget *) gtk_builder_get_object(builder, "cb_block_selection");
	priv->s_block_selection = g_signal_connect(priv->w_cb_block_selection, "toggled", G_CALLBACK(l_block_selection_toggled), result);
	priv->w_cb_limit_cursor = (GtkWidget *) gtk_builder_get_object(builder, "cb_limit_cursor");
	priv->s_limit_cursor = g_signal_connect(priv->w_cb_limit_cursor, "toggled", G_CALLBACK(l_limit_cursor_toggled), result);

	return result;
}


static gboolean l_notify_modification(DraPrefsBehaviourPanel *panel) {
	DraPrefsBehaviourPanelPrivate *priv = dra_prefs_behaviour_panel_get_instance_private(panel);
	cow_panel_set_configuration((CowPanel *) panel, (GObject *) priv->e_prefs);
	return TRUE;
}

static gboolean l_block_selection_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	DraPrefsBehaviourPanel *panel = DRA_PREFS_BEHAVIOUR_PANEL(user_data);
	DraPrefsBehaviourPanelPrivate *priv = dra_prefs_behaviour_panel_get_instance_private(panel);
	cha_preferences_wo_set_block_selection(priv->e_ast_prefs, gtk_toggle_button_get_active(toggle_button));
	return l_notify_modification(panel);
}

static gboolean l_limit_cursor_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	DraPrefsBehaviourPanel *panel = DRA_PREFS_BEHAVIOUR_PANEL(user_data);
	DraPrefsBehaviourPanelPrivate *priv = dra_prefs_behaviour_panel_get_instance_private(panel);
	cha_preferences_wo_set_limit_cursor(priv->e_ast_prefs, gtk_toggle_button_get_active(toggle_button));
	return l_notify_modification(panel);
}


static void l_refresh_form(DraPrefsBehaviourPanel *panel) {
	DraPrefsBehaviourPanelPrivate *priv = dra_prefs_behaviour_panel_get_instance_private(panel);

	g_signal_handler_block(priv->w_cb_block_selection, priv->s_block_selection);
	g_signal_handler_block(priv->w_cb_limit_cursor, priv->s_limit_cursor);

	gboolean has_prefs = priv->e_ast_prefs!=NULL;

	gtk_widget_set_sensitive(priv->w_cb_block_selection, has_prefs);
	gtk_widget_set_sensitive(priv->w_cb_limit_cursor, has_prefs);

	if (has_prefs) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_cb_block_selection), cha_preferences_wo_get_block_selection(priv->e_ast_prefs));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_cb_limit_cursor), cha_preferences_wo_get_limit_cursor(priv->e_ast_prefs));
	}

	g_signal_handler_unblock(priv->w_cb_block_selection, priv->s_block_selection);
	g_signal_handler_unblock(priv->w_cb_limit_cursor, priv->s_limit_cursor);
}


static void l_panel_reconfigure(CowPanel *self, GObject *config) {
	DraPrefsBehaviourPanel *instance = DRA_PREFS_BEHAVIOUR_PANEL(self);
	DraPrefsBehaviourPanelPrivate *priv = dra_prefs_behaviour_panel_get_instance_private(instance);

	cat_log_debug("config=%o", config);
	CatWo *e_prefs = CAT_WO(config);
	if (e_prefs!=NULL) {
		e_prefs = cat_wo_clone(e_prefs, CAT_CLONE_DEPTH_MAIN);
	}
	cat_unref_ptr(priv->e_prefs);
	cat_unref_ptr(priv->e_ast_prefs);
	priv->e_prefs = e_prefs;

	if (e_prefs!=NULL) {

		cat_log_debug("e_prefs=%O", e_prefs);
		DraPreferencesWo *ast_prefs = (DraPreferencesWo *) cow_ientry_accessor_get(priv->entry_accessor, e_prefs);
		DraPreferencesWo *e_ast_prefs = NULL;
		if (ast_prefs == NULL) {
			e_ast_prefs = dra_preferences_wo_new();
		} else {
			e_ast_prefs = dra_preferences_wo_clone(ast_prefs, CAT_CLONE_DEPTH_MAIN);
		}
		cow_ientry_accessor_set(priv->entry_accessor, e_prefs, e_ast_prefs);
		cat_log_debug("e_ast_prefs=%o", e_ast_prefs);
		priv->e_ast_prefs = (ChaPreferencesWo *) e_ast_prefs;
	}

	l_refresh_form(instance);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraPrefsBehaviourPanel *instance = DRA_PREFS_BEHAVIOUR_PANEL(self);
	DraPrefsBehaviourPanelPrivate *priv = dra_prefs_behaviour_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
