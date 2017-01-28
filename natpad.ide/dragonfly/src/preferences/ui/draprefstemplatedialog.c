/*
   File:    elkprefstemplatedialog.c
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 25, 2014
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

#include <gtk/gtk.h>

#include "dragladeprefstemplate.h"

#include <logging/catlogdefs.h>

#include "draprefstemplatedialog.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraPrefsTemplateDialog"
#include <logging/catlog.h>

struct _DraPrefsTemplateDialogPrivate {
	GtkDialog *dialog;
	ChaPrefsTemplateWo *e_template;

	GtkWidget *w_name;
	gulong s_txt_name;
	GtkWidget *w_description;
	gulong s_txt_description;
	GtkWidget *w_template;
	gulong s_txt_template;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraPrefsTemplateDialog, dra_prefs_template_dialog, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraPrefsTemplateDialog)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_prefs_template_dialog_class_init(DraPrefsTemplateDialogClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_prefs_template_dialog_init(DraPrefsTemplateDialog *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraPrefsTemplateDialog *instance = DRA_PREFS_TEMPLATE_DIALOG(object);
	DraPrefsTemplateDialogPrivate *priv = dra_prefs_template_dialog_get_instance_private(instance);
	cat_unref_ptr(priv->e_template);
	G_OBJECT_CLASS(dra_prefs_template_dialog_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_prefs_template_dialog_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_on_txt_name_changed(GtkEntry *entry, gpointer user_data);
static void l_on_txt_description_changed(GtkEntry *entry, gpointer user_data);
static void l_refresh_from(DraPrefsTemplateDialog *template_dialog);

DraPrefsTemplateDialog *dra_prefs_template_dialog_new(ChaPrefsTemplateWo *e_template) {
	DraPrefsTemplateDialog *result = g_object_new(DRA_TYPE_PREFS_TEMPLATE_DIALOG, NULL);
	cat_ref_anounce(result);
	DraPrefsTemplateDialogPrivate *priv = dra_prefs_template_dialog_get_instance_private(result);
	priv->e_template = cat_ref_ptr(e_template);

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) dragladeprefstemplate_glade, (gsize) dragladeprefstemplate_glade_len, (GError **) (&gerror));
	priv->dialog = (GtkDialog *) gtk_builder_get_object(builder, "dialog_main");

	gtk_widget_set_size_request((GtkWidget *) priv->dialog, 450, 480);

	priv->w_name = (GtkWidget *) gtk_builder_get_object(builder, "txt_name");
	priv->s_txt_name = g_signal_connect(priv->w_name, "changed", G_CALLBACK(l_on_txt_name_changed), result);

	priv->w_description = (GtkWidget *) gtk_builder_get_object(builder, "txt_description");
	priv->s_txt_description = g_signal_connect(priv->w_description, "changed", G_CALLBACK(l_on_txt_description_changed), result);

	priv->w_template = (GtkWidget *) gtk_builder_get_object(builder, "txt_template");

	l_refresh_from(result);

	return result;
}



int dra_prefs_template_dialog_run(DraPrefsTemplateDialog *dialog) {
	DraPrefsTemplateDialogPrivate *priv = dra_prefs_template_dialog_get_instance_private(dialog);
	int result = gtk_dialog_run(priv->dialog);

	GtkTextBuffer *text_buffer = gtk_text_view_get_buffer((GtkTextView *) priv->w_template);
	GtkTextIter start;
	GtkTextIter end;
	gtk_text_buffer_get_start_iter(text_buffer, &start);
	gtk_text_buffer_get_end_iter(text_buffer, &end);
	char *text = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);

	CatStringWo *new_template = cat_string_wo_new_with(text);
	cha_prefs_template_wo_set_template(priv->e_template, new_template);
	cat_unref_ptr(new_template);

	return result;
}

void dra_prefs_template_dialog_close(DraPrefsTemplateDialog *dialog) {
	DraPrefsTemplateDialogPrivate *priv = dra_prefs_template_dialog_get_instance_private(dialog);
	if (priv->dialog) {
		gtk_widget_destroy(GTK_WIDGET(priv->dialog));
		priv->dialog = NULL;
	}
}



static void l_refresh_from(DraPrefsTemplateDialog *template_dialog) {
	DraPrefsTemplateDialogPrivate *priv = dra_prefs_template_dialog_get_instance_private(template_dialog);

	g_signal_handler_block(priv->w_name, priv->s_txt_name);
	gtk_entry_set_text(GTK_ENTRY(priv->w_name), cat_string_wo_getchars(cha_prefs_template_wo_get_name(priv->e_template)));
	g_signal_handler_unblock(priv->w_name, priv->s_txt_name);

	g_signal_handler_block(priv->w_description, priv->s_txt_description);
	gtk_entry_set_text(GTK_ENTRY(priv->w_description), cat_string_wo_getchars(cha_prefs_template_wo_get_description(priv->e_template)));
	g_signal_handler_unblock(priv->w_description, priv->s_txt_description);

	CatStringWo *template_text = cha_prefs_template_wo_get_template(priv->e_template);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer((GtkTextView *) priv->w_template);
	gtk_text_buffer_set_text(buffer, cat_string_wo_getchars(template_text), cat_string_wo_length(template_text));
}




static void l_on_txt_name_changed(GtkEntry *entry, gpointer user_data) {
	DraPrefsTemplateDialog *instance = DRA_PREFS_TEMPLATE_DIALOG(user_data);
	DraPrefsTemplateDialogPrivate *priv = dra_prefs_template_dialog_get_instance_private(instance);
	CatStringWo *new_name = cat_string_wo_new_with(gtk_entry_get_text(entry));
	cha_prefs_template_wo_set_name(priv->e_template, new_name);
	cat_unref_ptr(new_name);
}

static void l_on_txt_description_changed(GtkEntry *entry, gpointer user_data) {
	DraPrefsTemplateDialog *instance = DRA_PREFS_TEMPLATE_DIALOG(user_data);
	DraPrefsTemplateDialogPrivate *priv = dra_prefs_template_dialog_get_instance_private(instance);
	CatStringWo *new_description = cat_string_wo_new_with(gtk_entry_get_text(entry));
	cha_prefs_template_wo_set_description(priv->e_template, new_description);
	cat_unref_ptr(new_description);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraPrefsTemplateDialog *instance = DRA_PREFS_TEMPLATE_DIALOG(self);
	DraPrefsTemplateDialogPrivate *priv = dra_prefs_template_dialog_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p e_template=%p]", iname, self, priv->e_template);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
