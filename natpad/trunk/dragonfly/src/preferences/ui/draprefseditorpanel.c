/*
   File:    draprefseditorpanel.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 19, 2014
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

#include "draprefseditorpanel.h"

#include "dragladeprefseditor.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "DraPrefsEditorPanel"
#include <logging/catlog.h>

struct _DraPrefsEditorPanelPrivate {
	CowIEntryAccessor *entry_accessor;
	CatWo *e_prefs;
	ChaPreferencesWo *e_ast_prefs;

	GtkWidget *w_cb_blink_cursor;
	unsigned long s_blink_cursor;
	GtkWidget *w_cb_show_whitespace;
	unsigned long s_show_whitespace;
	GtkWidget *w_cb_wrap_lines;
	unsigned long s_wrap_lines;
	GtkWidget *w_cb_show_line_numbers;
	unsigned long s_show_line_numbers;
	GtkWidget *w_cb_limit_cursor;
	unsigned long s_limit_cursor;
	GtkWidget *w_cb_highlight_current_line;
	unsigned long s_highlight_current_line;
	GtkWidget *w_cb_deprecated_scrolling;
	unsigned long s_deprecated_scrolling;
	GtkWidget *w_cb_block_selection;
	unsigned long s_block_selection;
	GtkWidget *w_sb_tab_size;
	unsigned long s_tab_size;
	GtkWidget *w_fb_font;
	unsigned long s_font;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraPrefsEditorPanel, dra_prefs_editor_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(DraPrefsEditorPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);


static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);

static void dra_prefs_editor_panel_class_init(DraPrefsEditorPanelClass *clazz) {
	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_prefs_editor_panel_init(DraPrefsEditorPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraPrefsEditorPanel *instance = DRA_PREFS_EDITOR_PANEL(object);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(instance);
	cat_unref_ptr(priv->entry_accessor);
	G_OBJECT_CLASS(dra_prefs_editor_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_prefs_editor_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gboolean l_blink_cursor_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_show_whitespace_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_wrap_lines_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_show_line_numbers_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_limit_cursor_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_highlight_current_line_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_deprecated_scrolling_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_block_selection_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static void l_tab_size_changed(GtkSpinButton *text_entry, gpointer user_data);
static void l_font_set(GtkFontButton *font_button, gpointer user_data);

static gboolean l_font_filter(PangoFontFamily *family, PangoFontFace *face, gpointer data) {
	return pango_font_family_is_monospace(family);
//	const char *facename = pango_font_face_get_face_name(face);
//	PangoFontDescription *descr = pango_font_face_describe(face);
//	pango_fon
//
//    if (strstr(facename, "Italic") || strstr(facename, "Bold") ||
//	strstr(facename, "Oblique")) {
//	return FALSE;
//    } else {
//	return validate_single_font(family, FONT_FILTER_LATIN_MONO);
//    }
}

DraPrefsEditorPanel *dra_prefs_editor_panel_new(CowIEntryAccessor *entry_accessor) {
	DraPrefsEditorPanel *result = g_object_new(DRA_TYPE_PREFS_EDITOR_PANEL, NULL);
	cat_ref_anounce(result);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(result);
	priv->entry_accessor = cat_ref_ptr(entry_accessor);
	priv->e_ast_prefs = NULL;
	priv->e_prefs = NULL;
	cow_panel_construct((CowPanel *) result);

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) dragladeprefseditor_glade, (gsize) dragladeprefseditor_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);

	gtk_container_add((GtkContainer *) result, frame);

	priv->w_cb_blink_cursor = (GtkWidget *) gtk_builder_get_object(builder, "cb_blink_cursor");
	priv->s_blink_cursor  = g_signal_connect(priv->w_cb_blink_cursor, "toggled", G_CALLBACK(l_blink_cursor_toggled), result);
	priv->w_cb_show_whitespace = (GtkWidget *) gtk_builder_get_object(builder, "cb_show_whitespace");
	priv->s_show_whitespace  = g_signal_connect(priv->w_cb_show_whitespace, "toggled", G_CALLBACK(l_show_whitespace_toggled), result);
	priv->w_cb_wrap_lines = (GtkWidget *) gtk_builder_get_object(builder, "cb_wrap_lines");
	priv->s_wrap_lines = g_signal_connect(priv->w_cb_wrap_lines, "toggled", G_CALLBACK(l_wrap_lines_toggled), result);
	priv->w_cb_show_line_numbers = (GtkWidget *) gtk_builder_get_object(builder, "cb_show_line_numbers");
	priv->s_show_line_numbers = g_signal_connect(priv->w_cb_show_line_numbers, "toggled", G_CALLBACK(l_show_line_numbers_toggled), result);
	priv->w_cb_limit_cursor = (GtkWidget *) gtk_builder_get_object(builder, "cb_limit_cursor");
	priv->s_limit_cursor = g_signal_connect(priv->w_cb_limit_cursor, "toggled", G_CALLBACK(l_limit_cursor_toggled), result);
	priv->w_cb_highlight_current_line = (GtkWidget *) gtk_builder_get_object(builder, "cb_highlight_current_line");
	priv->s_highlight_current_line = g_signal_connect(priv->w_cb_highlight_current_line, "toggled", G_CALLBACK(l_highlight_current_line_toggled), result);
	priv->w_cb_deprecated_scrolling = (GtkWidget *) gtk_builder_get_object(builder, "cb_deprecated_scrolling");
	priv->s_deprecated_scrolling = g_signal_connect(priv->w_cb_deprecated_scrolling, "toggled", G_CALLBACK(l_deprecated_scrolling_toggled), result);
	priv->w_cb_block_selection = (GtkWidget *) gtk_builder_get_object(builder, "cb_block_selection");
	priv->s_block_selection = g_signal_connect(priv->w_cb_block_selection, "toggled", G_CALLBACK(l_block_selection_toggled), result);
	priv->w_sb_tab_size = (GtkWidget *) gtk_builder_get_object(builder, "sb_tab_size");
	priv->s_tab_size = g_signal_connect(priv->w_sb_tab_size, "value_changed", G_CALLBACK(l_tab_size_changed), result);
	priv->w_fb_font = (GtkWidget *) gtk_builder_get_object(builder, "fb_font");
	priv->s_font = g_signal_connect(priv->w_fb_font, "font-set", G_CALLBACK(l_font_set), result);

//	gtk_font_chooser_set_filter_func((GtkFontChooser *) priv->w_fb_font, l_font_filter, NULL, NULL);

	return result;
}

static gboolean l_notify_modification(DraPrefsEditorPanel *panel) {
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);
	cow_panel_set_configuration((CowPanel *) panel, (GObject *) priv->e_prefs);
	return TRUE;
}

static gboolean l_blink_cursor_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	DraPrefsEditorPanel *panel = DRA_PREFS_EDITOR_PANEL(user_data);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);
	cha_preferences_wo_set_blink_cursor(priv->e_ast_prefs, gtk_toggle_button_get_active(toggle_button));
	return l_notify_modification(panel);
}

static gboolean l_show_whitespace_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	DraPrefsEditorPanel *panel = DRA_PREFS_EDITOR_PANEL(user_data);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);
	cha_preferences_wo_set_show_whitespace(priv->e_ast_prefs, gtk_toggle_button_get_active(toggle_button));
	return l_notify_modification(panel);
}

static gboolean l_wrap_lines_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	DraPrefsEditorPanel *panel = DRA_PREFS_EDITOR_PANEL(user_data);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);
	cha_preferences_wo_set_wrap_lines(priv->e_ast_prefs, gtk_toggle_button_get_active(toggle_button));
	return l_notify_modification(panel);
}

static gboolean l_show_line_numbers_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	DraPrefsEditorPanel *panel = DRA_PREFS_EDITOR_PANEL(user_data);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);
	cha_preferences_wo_set_show_line_numbers(priv->e_ast_prefs, gtk_toggle_button_get_active(toggle_button));
	return l_notify_modification(panel);
}

static gboolean l_limit_cursor_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	DraPrefsEditorPanel *panel = DRA_PREFS_EDITOR_PANEL(user_data);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);
	cha_preferences_wo_set_limit_cursor(priv->e_ast_prefs, gtk_toggle_button_get_active(toggle_button));
	return l_notify_modification(panel);
}

static gboolean l_highlight_current_line_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	DraPrefsEditorPanel *panel = DRA_PREFS_EDITOR_PANEL(user_data);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);
	cha_preferences_wo_set_highlight_current_line(priv->e_ast_prefs, gtk_toggle_button_get_active(toggle_button));
	return l_notify_modification(panel);
}


static gboolean l_deprecated_scrolling_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	DraPrefsEditorPanel *panel = DRA_PREFS_EDITOR_PANEL(user_data);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);
	cha_preferences_wo_set_deprecated_scrolling(priv->e_ast_prefs, gtk_toggle_button_get_active(toggle_button));
	return l_notify_modification(panel);
}

static gboolean l_block_selection_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	DraPrefsEditorPanel *panel = DRA_PREFS_EDITOR_PANEL(user_data);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);
	cha_preferences_wo_set_block_selection(priv->e_ast_prefs, gtk_toggle_button_get_active(toggle_button));
	return l_notify_modification(panel);
}

static void l_tab_size_changed(GtkSpinButton *text_entry, gpointer user_data) {
	DraPrefsEditorPanel *panel = DRA_PREFS_EDITOR_PANEL(user_data);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);
	int val = gtk_spin_button_get_value_as_int(text_entry);
	cat_log_error("ss val=%d",val);
	cha_preferences_wo_set_tab_size(priv->e_ast_prefs, val);
	l_notify_modification(panel);
}

static void l_font_set(GtkFontButton *font_button, gpointer user_data) {
	DraPrefsEditorPanel *panel = DRA_PREFS_EDITOR_PANEL(user_data);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);
	const gchar *font_name = gtk_font_button_get_font_name(font_button);
	cha_preferences_wo_set_font_name(priv->e_ast_prefs, cat_string_wo_new_with(font_name));
	l_notify_modification(panel);
}


static void l_refresh_form(DraPrefsEditorPanel *panel) {
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(panel);



	g_signal_handler_block(priv->w_cb_blink_cursor, priv->s_blink_cursor);
	g_signal_handler_block(priv->w_cb_show_whitespace, priv->s_show_whitespace);
	g_signal_handler_block(priv->w_cb_wrap_lines, priv->s_wrap_lines);
	g_signal_handler_block(priv->w_cb_show_line_numbers, priv->s_show_line_numbers);
	g_signal_handler_block(priv->w_cb_limit_cursor, priv->s_limit_cursor);
	g_signal_handler_block(priv->w_cb_highlight_current_line, priv->s_highlight_current_line);
	g_signal_handler_block(priv->w_cb_deprecated_scrolling, priv->s_deprecated_scrolling);
	g_signal_handler_block(priv->w_cb_block_selection, priv->s_block_selection);
	g_signal_handler_block(priv->w_sb_tab_size, priv->s_tab_size);
	g_signal_handler_block(priv->w_fb_font, priv->s_font);

	gboolean has_prefs = priv->e_ast_prefs!=NULL;

	gtk_widget_set_sensitive(priv->w_cb_blink_cursor, has_prefs);
	gtk_widget_set_sensitive(priv->w_cb_show_whitespace, has_prefs);
	gtk_widget_set_sensitive(priv->w_cb_wrap_lines, has_prefs);
	gtk_widget_set_sensitive(priv->w_cb_show_line_numbers, has_prefs);
	gtk_widget_set_sensitive(priv->w_cb_limit_cursor, has_prefs);
	gtk_widget_set_sensitive(priv->w_cb_highlight_current_line, has_prefs);
	gtk_widget_set_sensitive(priv->w_cb_deprecated_scrolling, has_prefs);
	gtk_widget_set_sensitive(priv->w_cb_block_selection, has_prefs);
	gtk_widget_set_sensitive(priv->w_sb_tab_size, has_prefs);
	gtk_widget_set_sensitive(priv->w_fb_font, has_prefs);

	if (has_prefs) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_cb_blink_cursor), cha_preferences_wo_get_blink_cursor(priv->e_ast_prefs));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_cb_show_whitespace), cha_preferences_wo_get_show_whitespace(priv->e_ast_prefs));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_cb_wrap_lines), cha_preferences_wo_get_wrap_lines(priv->e_ast_prefs));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_cb_show_line_numbers), cha_preferences_wo_get_show_line_numbers(priv->e_ast_prefs));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_cb_limit_cursor), cha_preferences_wo_get_limit_cursor(priv->e_ast_prefs));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_cb_highlight_current_line), cha_preferences_wo_get_highlight_current_line(priv->e_ast_prefs));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_cb_deprecated_scrolling), cha_preferences_wo_get_deprecated_scrolling(priv->e_ast_prefs));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(priv->w_cb_block_selection), cha_preferences_wo_get_block_selection(priv->e_ast_prefs));
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(priv->w_sb_tab_size), cha_preferences_wo_get_tab_size(priv->e_ast_prefs));
		CatStringWo *font_name = cha_preferences_wo_get_font_name(priv->e_ast_prefs);
		if (font_name) {
			gtk_font_button_set_font_name(GTK_FONT_BUTTON(priv->w_fb_font), cat_string_wo_getchars(font_name));
		}
	}

	g_signal_handler_unblock(priv->w_cb_blink_cursor, priv->s_blink_cursor);
	g_signal_handler_unblock(priv->w_cb_show_whitespace, priv->s_show_whitespace);
	g_signal_handler_unblock(priv->w_cb_wrap_lines, priv->s_wrap_lines);
	g_signal_handler_unblock(priv->w_cb_show_line_numbers, priv->s_show_line_numbers);
	g_signal_handler_unblock(priv->w_cb_limit_cursor, priv->s_limit_cursor);
	g_signal_handler_unblock(priv->w_cb_highlight_current_line, priv->s_highlight_current_line);
	g_signal_handler_unblock(priv->w_cb_deprecated_scrolling, priv->s_deprecated_scrolling);
	g_signal_handler_unblock(priv->w_cb_block_selection, priv->s_block_selection);
	g_signal_handler_unblock(priv->w_sb_tab_size, priv->s_tab_size);
	g_signal_handler_unblock(priv->w_fb_font, priv->s_font);
}


static void l_panel_reconfigure(CowPanel *self, GObject *config) {
	DraPrefsEditorPanel *instance = DRA_PREFS_EDITOR_PANEL(self);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(instance);

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
		ChaPreferencesWo *ast_prefs = (ChaPreferencesWo *) cow_ientry_accessor_get(priv->entry_accessor, e_prefs);
		ChaPreferencesWo *e_ast_prefs = NULL;
		if (ast_prefs == NULL) {
			e_ast_prefs = dra_preferences_wo_new();
		} else {
			e_ast_prefs = dra_preferences_wo_clone(ast_prefs, CAT_CLONE_DEPTH_MAIN);
		}
		cow_ientry_accessor_set(priv->entry_accessor, e_prefs, e_ast_prefs);
		cat_log_debug("e_ast_prefs=%o", e_ast_prefs);
		priv->e_ast_prefs = e_ast_prefs;
	}

	l_refresh_form(instance);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraPrefsEditorPanel *instance = DRA_PREFS_EDITOR_PANEL(self);
	DraPrefsEditorPanelPrivate *priv = dra_prefs_editor_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p e_ast_prefs=%p]", iname, self, priv->e_ast_prefs);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

