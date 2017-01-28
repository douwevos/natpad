/*
   File:    chaeditorprinter.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 4, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "chaeditorprinter.h"
#include "charevisionprinter.h"
#include "chaprintsettings.h"
#include <gtk/gtk.h>

#include "chaprintpanel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaEditorPrinter"
#include <logging/catlog.h>

struct _ChaEditorPrinterPrivate {
	ChaEditor *editor;
	CatStringWo *a_markup_key;
	ChaPrintSettings *cha_print_settings;

	GtkWidget *w_but_overide;
	unsigned long s_but_overide;

	GtkWidget *w_grid_overide;

	GtkWidget *w_but_numbers;
	GtkWidget *w_box_number_col;
	GtkWidget *w_but_num_color;
	GtkWidget *w_font;
	GtkWidget *w_line_wrapping;
	GtkWidget *w_auto_indent_wrapped;
	GtkWidget *w_enable_markup;
	GtkWidget *w_enable_header_and_footer;
	GtkWidget *w_txt_header;
	GtkWidget *w_txt_footer;

	ChaRevisionPrinter *editor_printer;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaEditorPrinter, cha_editor_printer, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaEditorPrinter)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_editor_printer_class_init(ChaEditorPrinterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_editor_printer_init(ChaEditorPrinter *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaEditorPrinter *instance = CHA_EDITOR_PRINTER(object);
//	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(instance);
	G_OBJECT_CLASS(cha_editor_printer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_editor_printer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaEditorPrinter *cha_editor_printer_new(ChaEditor *editor, CatStringWo *a_markup_key) {
	ChaEditorPrinter *result = g_object_new(CHA_TYPE_EDITOR_PRINTER, NULL);
	cat_ref_anounce(result);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(result);
	priv->editor = cat_ref_ptr(editor);
	priv->a_markup_key = cat_ref_ptr(a_markup_key);
	priv->cha_print_settings = cha_print_settings_new();
	priv->editor_printer = NULL;



	ChaPreferencesWo *a_prefs = cha_editor_get_preferences(priv->editor);
	cha_print_settings_set_print_line_numbers(priv->cha_print_settings, cha_preferences_wo_get_show_line_numbers(a_prefs));
	cha_print_settings_set_wrap_lines(priv->cha_print_settings, cha_preferences_wo_get_wrap_lines(a_prefs));
	cha_print_settings_set_print_markup(priv->cha_print_settings, TRUE);
	cha_print_settings_set_auto_indent_wrapped_lines(priv->cha_print_settings, TRUE);

	CatStringWo *font_name = cha_preferences_wo_get_font_name(a_prefs);
	cha_print_settings_set_font_name(priv->cha_print_settings, font_name);

	return result;
}

static void l_begin_print(GtkPrintOperation *operation, GtkPrintContext *context, gpointer user_data);
static void l_end_print(GtkPrintOperation *operation, GtkPrintContext *context, gpointer user_data);
static void l_draw_page(GtkPrintOperation *operation, GtkPrintContext *context, gint page_nr, gpointer user_data);
static GtkWidget *l_create_custom_widget(GtkPrintOperation *operation, gpointer user_data);

void cha_editor_printer_do_print(ChaEditorPrinter *editor_printer, gboolean do_preview) {
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);

//	ast_print_settings_set_color_map(priv->ast_print_settings, ast_preferences_wo_get_color_map(a_ast_prefs));
//	ast_print_settings_set_wrap_lines(priv->ast_print_settings, ast_preferences_wo_get_wrap_lines(a_ast_prefs));


	GtkWidget *toplevel = gtk_widget_get_toplevel(GTK_WIDGET(priv->editor));
	GtkPrintOperation *print_operation = gtk_print_operation_new();

	g_signal_connect(G_OBJECT(print_operation), "begin-print", G_CALLBACK (l_begin_print), editor_printer);
	g_signal_connect(G_OBJECT(print_operation), "draw-page", G_CALLBACK (l_draw_page), editor_printer);
	g_signal_connect(G_OBJECT(print_operation), "end-print", G_CALLBACK (l_end_print), editor_printer);

	g_signal_connect(G_OBJECT(print_operation), "create-custom-widget", G_CALLBACK(l_create_custom_widget), editor_printer);


	GError *error = NULL;
	gtk_print_operation_set_custom_tab_label(print_operation, "Natpad");
	gtk_print_operation_set_embed_page_setup(print_operation, TRUE);
	gtk_print_operation_run(print_operation, do_preview ? GTK_PRINT_OPERATION_ACTION_PREVIEW : GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, GTK_WINDOW (toplevel), &error);


	if (error) {
		GtkWidget *dialog;

		dialog = gtk_message_dialog_new (GTK_WINDOW (toplevel), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", error->message);
		g_error_free (error);

		g_signal_connect (dialog, "response",
		G_CALLBACK (gtk_widget_destroy), NULL);

		gtk_widget_show (dialog);
	}
}




#define FONT_SIZE 12.0

static void l_begin_print(GtkPrintOperation *operation, GtkPrintContext *context, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);

	double width = gtk_print_context_get_width(context);
	double height = gtk_print_context_get_height(context);

	double margin_top = 0;
	double margin_bottom = 0;
	double margin_left = 0;
	double margin_right = 0;

	gtk_print_context_get_hard_margins(context, &margin_top, &margin_bottom, &margin_left, &margin_right);
	GtkPageSetup *page_setup = gtk_print_context_get_page_setup(context);
	margin_top = gtk_page_setup_get_top_margin(page_setup, GTK_UNIT_PIXEL);
	margin_bottom = gtk_page_setup_get_bottom_margin(page_setup, GTK_UNIT_PIXEL);
	margin_left = gtk_page_setup_get_left_margin(page_setup, GTK_UNIT_PIXEL);
	margin_right = gtk_page_setup_get_right_margin(page_setup, GTK_UNIT_PIXEL);



	PangoContext *pango_context = gtk_print_context_create_pango_context(context);
	PangoLayout *pango_layout = gtk_print_context_create_pango_layout(context);
	cairo_t *cairo = gtk_print_context_get_cairo_context(context);


	priv->editor_printer = cha_revision_printer_new(priv->cha_print_settings, priv->editor, priv->a_markup_key, cairo, pango_context, pango_layout, width, height, margin_top, margin_bottom, margin_left, margin_right);

	int page_count = cha_revision_printer_get_page_count(priv->editor_printer);
	cat_log_debug("page_count=%d", page_count);
	gtk_print_operation_set_n_pages(operation, page_count);


}


static void l_end_print(GtkPrintOperation *operation, GtkPrintContext *context, gpointer user_data) {
//	AstEditorPrinter *editor_printer = AST_EDITOR_PRINTER(user_data);
//	AstEditorPrinterPrivate *priv = editor_printer->priv;
}


static void l_draw_page(GtkPrintOperation *operation, GtkPrintContext *context, gint page_nr, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);

	cat_log_debug("draw page:%d", page_nr);
	cha_revision_printer_draw_page(priv->editor_printer, page_nr);
}



static gboolean l_but_numbers_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);
	gboolean is_active = gtk_toggle_button_get_active(toggle_button);
	gtk_widget_set_sensitive(priv->w_but_num_color, is_active);
	cha_print_settings_set_print_line_numbers(priv->cha_print_settings, is_active);
	GdkRGBA color;
	gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(priv->w_but_num_color), &color);
	cha_print_settings_set_line_number_color(priv->cha_print_settings, color);
	gtk_widget_set_sensitive(priv->w_box_number_col, is_active);
	return TRUE;
}

static gboolean l_but_number_col_toggled(GtkColorButton *color_button, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);
	GdkRGBA color;
	gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(priv->w_but_num_color), &color);
	cha_print_settings_set_line_number_color(priv->cha_print_settings, color);
	return TRUE;
}



static void l_font_set(GtkFontButton *font_button, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);
	const gchar *font_name = gtk_font_button_get_font_name(font_button);
	cha_print_settings_set_font_name(priv->cha_print_settings, cat_string_wo_new_with(font_name));
}

static gboolean l_but_line_wrapping_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);
	gboolean is_active = gtk_toggle_button_get_active(toggle_button);
	cha_print_settings_set_wrap_lines(priv->cha_print_settings, is_active);
	gtk_widget_set_sensitive(priv->w_auto_indent_wrapped, is_active);
	return TRUE;
}

static gboolean l_but_auto_indent_wrapped_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);
	gboolean is_active = gtk_toggle_button_get_active(toggle_button);
	cha_print_settings_set_auto_indent_wrapped_lines(priv->cha_print_settings, is_active);
	return TRUE;
}


static gboolean l_but_enable_markup_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);
	gboolean is_active = gtk_toggle_button_get_active(toggle_button);
	cha_print_settings_set_print_markup(priv->cha_print_settings, is_active);
	return TRUE;
}



static void l_txt_header_changed(GtkEntry *entry, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);
	CatStringWo *new_text = cat_string_wo_new_with(gtk_entry_get_text(GTK_ENTRY(priv->w_txt_header)));
	cha_print_settings_set_header_text(priv->cha_print_settings, new_text);
	cat_unref_ptr(new_text);
}



static void l_txt_footer_changed(GtkEntry *entry, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);
	CatStringWo *new_text = cat_string_wo_new_with(gtk_entry_get_text(GTK_ENTRY(priv->w_txt_footer)));
	cha_print_settings_set_footer_text(priv->cha_print_settings, new_text);
	cat_unref_ptr(new_text);
}


static gboolean l_but_enable_header_and_footer_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);
	gboolean is_active = gtk_toggle_button_get_active(toggle_button);

	cha_print_settings_set_print_header_and_footer(priv->cha_print_settings, is_active);

	gtk_widget_set_sensitive(priv->w_txt_header, is_active);
	gtk_widget_set_sensitive(priv->w_txt_footer, is_active);
	return TRUE;
}



static gboolean l_but_overide_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);
	gboolean is_active = gtk_toggle_button_get_active(toggle_button);


	gtk_widget_set_sensitive(priv->w_grid_overide, is_active);


	if (is_active) {
		/* override editor settings */

		cha_print_settings_set_print_line_numbers(priv->cha_print_settings, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(priv->w_but_numbers)));
		GdkRGBA num_color;
		gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(priv->w_but_num_color), &num_color);
		cha_print_settings_set_line_number_color(priv->cha_print_settings, num_color);

		l_but_numbers_toggled(GTK_TOGGLE_BUTTON(priv->w_but_numbers), user_data);
		l_but_number_col_toggled(GTK_COLOR_BUTTON(priv->w_but_num_color), user_data);
		l_but_line_wrapping_toggled(GTK_TOGGLE_BUTTON(priv->w_line_wrapping), user_data);
		l_but_auto_indent_wrapped_toggled(GTK_TOGGLE_BUTTON(priv->w_auto_indent_wrapped), user_data);
		l_but_enable_markup_toggled(GTK_TOGGLE_BUTTON(priv->w_enable_markup), user_data);
	} else {

		/* use editor settings */

		ChaPreferencesWo *a_prefs = cha_editor_get_preferences(priv->editor);

		gtk_toggle_button_set_active((GtkToggleButton *) priv->w_but_numbers, cha_preferences_wo_get_show_line_numbers(a_prefs));
		gtk_toggle_button_set_active((GtkToggleButton *) priv->w_line_wrapping, cha_preferences_wo_get_wrap_lines(a_prefs));
//		gtk_font_button_set_font_name((priv->w_font, cha_preferences_wo_get_wrap_lines(a_prefs));


		CatStringWo *font_name = cha_preferences_wo_get_font_name(a_prefs);
		gtk_font_button_set_font_name((GtkFontButton *) priv->w_font, cat_string_wo_getchars(font_name));


		cha_print_settings_set_font_name(priv->cha_print_settings, font_name);


		cha_print_settings_set_print_markup(priv->cha_print_settings, TRUE);
//		cha_print_settings_set_print_line_numbers(priv->cha_print_settings, cha_preferences_wo_get_show_line_numbers(a_prefs));

//		cha_print_settings_set_wrap_lines(priv->cha_print_settings, cha_preferences_wo_get_wrap_lines(a_prefs));
		cha_print_settings_set_auto_indent_wrapped_lines(priv->cha_print_settings, TRUE);

//		cha_print_settings_set_font_name(priv->cha_print_settings, cat_string_wo_new_with(cat_string_wo_getchars(cha_preferences_wo_get_font_family(a_prefs))));
//		cha_print_settings_set_font_size(priv->cha_print_settings, 10.0);
//		cha_print_settings_set_font_is_bold(priv->cha_print_settings, cha_preferences_wo_get_font_weight(a_prefs)>PANGO_WEIGHT_NORMAL);
	}

	return TRUE;
}




static GtkWidget *l_create_custom_widget(GtkPrintOperation *operation, gpointer user_data) {
	ChaEditorPrinter *editor_printer = CHA_EDITOR_PRINTER(user_data);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(editor_printer);
	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) chaprintpanel_glade, (gsize) chaprintpanel_glade_len, (GError **) (&gerror));
	GtkWidget *panel = (GtkWidget *) gtk_builder_get_object(builder, "print_panel");

	priv->w_but_overide = (GtkWidget *) gtk_builder_get_object(builder, "chk_overide");
	priv->s_but_overide = g_signal_connect(priv->w_but_overide, "toggled", G_CALLBACK(l_but_overide_toggled), editor_printer);

	priv->w_grid_overide = (GtkWidget *) gtk_builder_get_object(builder, "grid_overide");


	priv->w_but_numbers = (GtkWidget *) gtk_builder_get_object(builder, "but_numbers");
	priv->w_box_number_col = (GtkWidget *) gtk_builder_get_object(builder, "box_number_col");
	priv->w_but_num_color = (GtkWidget *) gtk_builder_get_object(builder, "col_line_number");
	priv->w_font = (GtkWidget *) gtk_builder_get_object(builder, "font");
	priv->w_line_wrapping = (GtkWidget *) gtk_builder_get_object(builder, "chk_wrap_lines");
	priv->w_auto_indent_wrapped = (GtkWidget *) gtk_builder_get_object(builder, "chk_auto_indent");
	priv->w_enable_markup = (GtkWidget *) gtk_builder_get_object(builder, "chk_markup");
	priv->w_enable_header_and_footer = (GtkWidget *) gtk_builder_get_object(builder, "chk_header_and_footer");
	priv->w_txt_header = (GtkWidget *) gtk_builder_get_object(builder, "txt_header");
	priv->w_txt_footer = (GtkWidget *) gtk_builder_get_object(builder, "txt_footer");

	gtk_toggle_button_set_active((GtkToggleButton *) priv->w_but_numbers, cha_print_settings_print_line_numbers(priv->cha_print_settings));
	GdkRGBA rgba = cha_print_settings_get_line_number_color(priv->cha_print_settings);
	gtk_color_chooser_set_rgba((GtkColorChooser *) priv->w_but_num_color, &rgba);
	CatStringWo *font_name = cha_print_settings_get_font_name(priv->cha_print_settings);
	if (font_name) {
		gtk_font_button_set_font_name((GtkFontButton *) priv->w_font, cat_string_wo_getchars(font_name));
	}
	gtk_toggle_button_set_active((GtkToggleButton *) priv->w_line_wrapping, cha_print_settings_get_wrap_lines(priv->cha_print_settings));
	gtk_toggle_button_set_active((GtkToggleButton *) priv->w_enable_markup, cha_print_settings_get_print_markup(priv->cha_print_settings));
	gtk_toggle_button_set_active((GtkToggleButton *) priv->w_enable_header_and_footer, cha_print_settings_get_print_header_and_footer(priv->cha_print_settings));
	CatStringWo *header_text = cha_print_settings_get_footer_text(priv->cha_print_settings);
	gtk_entry_set_text((GtkEntry *) priv->w_txt_header, cat_string_wo_getchars(header_text));
	CatStringWo *footer_text = cha_print_settings_get_footer_text(priv->cha_print_settings);
	gtk_entry_set_text((GtkEntry *) priv->w_txt_footer, cat_string_wo_getchars(footer_text));



	g_signal_connect(priv->w_but_numbers, "toggled", G_CALLBACK(l_but_numbers_toggled), editor_printer);
	g_signal_connect(priv->w_but_num_color, "color-set", G_CALLBACK(l_but_number_col_toggled), editor_printer);
	g_signal_connect(priv->w_font, "font-set", G_CALLBACK(l_font_set), editor_printer);
	g_signal_connect(priv->w_line_wrapping, "toggled", G_CALLBACK(l_but_line_wrapping_toggled), editor_printer);
	g_signal_connect(priv->w_auto_indent_wrapped, "toggled", G_CALLBACK(l_but_auto_indent_wrapped_toggled), editor_printer);
	g_signal_connect(priv->w_enable_markup, "toggled", G_CALLBACK(l_but_enable_markup_toggled), editor_printer);
	g_signal_connect(priv->w_enable_header_and_footer, "toggled", G_CALLBACK(l_but_enable_header_and_footer_toggled), editor_printer);
	g_signal_connect(priv->w_txt_header, "changed", G_CALLBACK(l_txt_header_changed), editor_printer);
	g_signal_connect(priv->w_txt_footer, "changed", G_CALLBACK(l_txt_footer_changed), editor_printer);




	l_but_overide_toggled(GTK_TOGGLE_BUTTON(priv->w_but_overide), user_data);
	gtk_widget_show_all(panel);
	return panel;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaEditorPrinter *instance = CHA_EDITOR_PRINTER(self);
	ChaEditorPrinterPrivate *priv = cha_editor_printer_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor=%p]", iname, self, priv->editor);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

