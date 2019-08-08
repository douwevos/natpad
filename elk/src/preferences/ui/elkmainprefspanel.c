/*
   File:    elkmainprefspanel.c
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 25, 2014
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

#include "elkmainprefspanel.h"
#include "../elkpreferenceswo.h"

#include "elkglademain.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkMainPrefsPanel"
#include <logging/catlog.h>

struct _ElkMainPrefsPanelPrivate {

	ElkPreferencesWo *e_prefs;
	GtkWidget *w_chk_one_instance;
	gulong s_chk_one_instance;
	GtkWidget *w_chk_full_screen;
	gulong s_chk_full_screen;
	GtkWidget *w_chk_multiple_resource_editors;
	gulong s_chk_multiple_resource_editors;
	GtkWidget *w_chk_backup_files;
	gulong s_chk_backup_files;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkMainPrefsPanel, elk_main_prefs_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(ElkMainPrefsPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);

static void elk_main_prefs_panel_class_init(ElkMainPrefsPanelClass *clazz) {
	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void elk_main_prefs_panel_init(ElkMainPrefsPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkMainPrefsPanel *instance = ELK_MAIN_PREFS_PANEL(object);
	ElkMainPrefsPanelPrivate *priv = elk_main_prefs_panel_get_instance_private(instance);
	cat_unref_ptr(priv->e_prefs);
	G_OBJECT_CLASS(elk_main_prefs_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_main_prefs_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gboolean l_one_instance_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_full_screen_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_multiple_resource_editors_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_backup_files_toggled(GtkToggleButton *toggle_button, gpointer user_data);


ElkMainPrefsPanel *elk_main_prefs_panel_new() {
	ElkMainPrefsPanel *result = g_object_new(ELK_TYPE_MAIN_PREFS_PANEL, NULL);
	cat_ref_anounce(result);
	ElkMainPrefsPanelPrivate *priv = elk_main_prefs_panel_get_instance_private(result);
	priv->e_prefs = NULL;
	cow_panel_construct((CowPanel *) result);


	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) elkglademain_glade, (gsize) elkglademain_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);

	gtk_container_add((GtkContainer *) result, frame);

	priv->w_chk_one_instance = (GtkWidget *) gtk_builder_get_object(builder, "chk_one_instance");
	priv->w_chk_full_screen = (GtkWidget *) gtk_builder_get_object(builder, "chk_full_screen");
	priv->w_chk_multiple_resource_editors = (GtkWidget *) gtk_builder_get_object(builder, "chk_multiple_resource_editors");
	priv->w_chk_backup_files = (GtkWidget *) gtk_builder_get_object(builder, "chk_backup_files");

	priv->s_chk_one_instance = g_signal_connect(priv->w_chk_one_instance, "toggled", G_CALLBACK(l_one_instance_toggled), result);
	priv->s_chk_full_screen = g_signal_connect(priv->w_chk_full_screen, "toggled", G_CALLBACK(l_full_screen_toggled), result);
	priv->s_chk_multiple_resource_editors = g_signal_connect(priv->w_chk_multiple_resource_editors, "toggled", G_CALLBACK(l_multiple_resource_editors_toggled), result);
	priv->s_chk_backup_files = g_signal_connect(priv->w_chk_backup_files, "toggled", G_CALLBACK(l_backup_files_toggled), result);

	return result;
}

static gboolean l_one_instance_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	ElkMainPrefsPanel *instance = ELK_MAIN_PREFS_PANEL(user_data);
	ElkMainPrefsPanelPrivate *priv = elk_main_prefs_panel_get_instance_private(instance);
	elk_preferences_wo_set_one_instance(priv->e_prefs, gtk_toggle_button_get_active(toggle_button));
	cow_panel_set_configuration((CowPanel *) instance, (GObject *) priv->e_prefs);
	return TRUE;
}


static gboolean l_full_screen_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	ElkMainPrefsPanel *instance = ELK_MAIN_PREFS_PANEL(user_data);
	ElkMainPrefsPanelPrivate *priv = elk_main_prefs_panel_get_instance_private(instance);
	elk_preferences_wo_set_full_screen(priv->e_prefs, gtk_toggle_button_get_active(toggle_button));
	cow_panel_set_configuration((CowPanel *) instance, (GObject *) priv->e_prefs);
	return TRUE;
}


static gboolean l_multiple_resource_editors_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	ElkMainPrefsPanel *instance = ELK_MAIN_PREFS_PANEL(user_data);
	ElkMainPrefsPanelPrivate *priv = elk_main_prefs_panel_get_instance_private(instance);
	elk_preferences_wo_set_multiple_resource_editors(priv->e_prefs, gtk_toggle_button_get_active(toggle_button));
	cow_panel_set_configuration((CowPanel *) instance, (GObject *) priv->e_prefs);
	return TRUE;
}

static gboolean l_backup_files_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	ElkMainPrefsPanel *instance = ELK_MAIN_PREFS_PANEL(user_data);
	ElkMainPrefsPanelPrivate *priv = elk_main_prefs_panel_get_instance_private(instance);
	elk_preferences_wo_set_backup_files(priv->e_prefs, gtk_toggle_button_get_active(toggle_button));
	cow_panel_set_configuration((CowPanel *) instance, (GObject *) priv->e_prefs);
	return TRUE;
}


static void l_panel_reconfigure(CowPanel *self, GObject *config) {
	ElkMainPrefsPanel *instance = ELK_MAIN_PREFS_PANEL(self);
	ElkMainPrefsPanelPrivate *priv = elk_main_prefs_panel_get_instance_private(instance);

	ElkPreferencesWo *e_prefs = ELK_PREFERENCES_WO(config);
	cat_log_debug("e_prefsx=%o", e_prefs);
	e_prefs = elk_preferences_wo_clone(e_prefs, CAT_CLONE_DEPTH_MAIN);

	cat_unref_ptr(priv->e_prefs);
	priv->e_prefs = e_prefs;
	cat_log_debug("e_prefs=%o", e_prefs);

	gboolean one_instance = FALSE;
	gboolean full_screen = FALSE;
	gboolean multiple_resource_editors = FALSE;
	gboolean backup_files = FALSE;
	if (e_prefs) {
		one_instance = elk_preferences_wo_one_instance(e_prefs);
		full_screen = elk_preferences_wo_full_screen(e_prefs);
		multiple_resource_editors = elk_preferences_wo_multiple_resource_editors(e_prefs);
		backup_files = elk_preferences_wo_backup_files(e_prefs);
	}

	g_signal_handler_block(priv->w_chk_one_instance, priv->s_chk_one_instance);
	g_signal_handler_block(priv->w_chk_full_screen, priv->s_chk_full_screen);
	g_signal_handler_block(priv->w_chk_multiple_resource_editors, priv->s_chk_multiple_resource_editors);
	g_signal_handler_block(priv->w_chk_backup_files, priv->s_chk_backup_files);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_chk_one_instance), one_instance);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_chk_full_screen), full_screen);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_chk_multiple_resource_editors), multiple_resource_editors);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_chk_backup_files), backup_files);

	g_signal_handler_unblock(priv->w_chk_one_instance, priv->s_chk_one_instance);
	g_signal_handler_unblock(priv->w_chk_full_screen, priv->s_chk_full_screen);
	g_signal_handler_unblock(priv->w_chk_multiple_resource_editors, priv->s_chk_multiple_resource_editors);
	g_signal_handler_unblock(priv->w_chk_backup_files, priv->s_chk_backup_files);

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
