/*
   File:    elkdialogs.c
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 4, 2015
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

#include "elkdialogs.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkDialogs"
#include <logging/catlog.h>

struct _ElkDialogsPrivate {
	LeaFrame *frame;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkDialogs, elk_dialogs, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ElkDialogs)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void elk_dialogs_class_init(ElkDialogsClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void elk_dialogs_init(ElkDialogs *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkDialogs *instance = ELK_DIALOGS(object);
	ElkDialogsPrivate *priv = elk_dialogs_get_instance_private(instance);
	cat_unref_ptr(priv->frame);
	G_OBJECT_CLASS(elk_dialogs_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_dialogs_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkDialogs *elk_dialogs_new(LeaFrame *frame) {
	ElkDialogs *result = g_object_new(ELK_TYPE_DIALOGS, NULL);
	cat_ref_anounce(result);
	ElkDialogsPrivate *priv = elk_dialogs_get_instance_private(result);
	priv->frame = cat_ref_ptr(frame);
	return result;
}



CatStringWo *elk_dialogs_save_file_selector(ElkDialogs *dialogs, ElkSaveDialog *save_dialog) {
	ElkDialogsPrivate *priv = elk_dialogs_get_instance_private(dialogs);
	GtkWidget *top_window = gtk_widget_get_toplevel(GTK_WIDGET(priv->frame));

	GtkWidget *dialog = gtk_file_chooser_dialog_new((const gchar *) "Select file or enter name to save", GTK_WINDOW(top_window),
					(GtkFileChooserAction) GTK_FILE_CHOOSER_ACTION_SAVE,
					"_Cancel", GTK_RESPONSE_CANCEL,
					"_OK", GTK_RESPONSE_OK,
					NULL);
	GtkFileChooser *file_chooser = GTK_FILE_CHOOSER(dialog);

	gtk_file_chooser_set_do_overwrite_confirmation(file_chooser, FALSE);


	GtkBox *w_encoding = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *w_lab_encoding = gtk_label_new_with_mnemonic("_Encoding");
	gtk_box_pack_start(w_encoding, w_lab_encoding, FALSE, FALSE, 0);

	GtkListStore *model = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_OBJECT);
	GtkWidget *w_cmb_encoding = gtk_combo_box_new_with_model_and_entry(model);
	gtk_combo_box_set_entry_text_column((GtkComboBox *) w_cmb_encoding, 0);
	gtk_box_pack_start(w_encoding, w_cmb_encoding, FALSE, FALSE, 0);


	ChaDocumentManager *doc_manager = cha_document_get_document_manager(save_dialog->document);
	ChaCharsetConverterFactory *factory = cha_document_manager_get_converter_factory(doc_manager);
	CatArrayWo *converter_names = cha_charset_converter_factory_enlist_names(factory);
	CatIIterator *iter = cat_array_wo_iterator(converter_names);
	gtk_list_store_clear(model);
	while(cat_iiterator_has_next(iter)) {
		CatStringWo *name = (CatStringWo *) cat_iiterator_next(iter);
		GtkTreeIter titer;
		gtk_list_store_append(model, &titer);
		gtk_list_store_set(model, &titer, 0, cat_string_wo_getchars(name), 1, name, -1);
	}
	cat_unref_ptr(iter);

	gtk_file_chooser_set_extra_widget(file_chooser, w_encoding);

	gtk_file_chooser_set_show_hidden(file_chooser, FALSE);
	gtk_widget_show_all(dialog);

	save_dialog->selected_charset = NULL;

	gint response = gtk_dialog_run(GTK_DIALOG(dialog));
	CatStringWo *result = NULL;
	if (response==GTK_RESPONSE_OK) {
		gchar *fname = gtk_file_chooser_get_filename(file_chooser);
		if (fname!=NULL) {
			result = cat_string_wo_new_with(fname);
		}


		GtkTreeIter titer;

		if (gtk_combo_box_get_active_iter(w_cmb_encoding, &titer)) {
			CatStringWo *selected_converter = NULL;
			GValue val = {0};
			gtk_tree_model_get_value(model, &titer, 1, &val);
			CatStringWo *nm = g_value_get_object(&val);
			cat_log_debug("nm=%O", nm);
			save_dialog->selected_charset = cat_ref_ptr(nm);	// TODO unref
		}
	}

	cat_unref_ptr(converter_names);

	gtk_widget_destroy(dialog);
	return result;
}


GtkResponseType elk_dialogs_save_before_close(ElkDialogs *dialogs) {
	ElkDialogsPrivate *priv = elk_dialogs_get_instance_private(dialogs);
	GtkWidget *top_window = gtk_widget_get_toplevel(GTK_WIDGET(priv->frame));

	GtkWidget *dialog = gtk_message_dialog_new_with_markup(GTK_WINDOW(top_window), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_NONE, "Save changes before closing ?");
	gtk_dialog_add_button((GtkDialog *) dialog, "_Close without Saving", GTK_RESPONSE_NO);
	gtk_dialog_add_button((GtkDialog *) dialog, "_Abort", GTK_RESPONSE_CANCEL);
	gtk_dialog_add_button((GtkDialog *) dialog, "_Save", GTK_RESPONSE_YES);

	gtk_widget_show_all(dialog);

	gint response = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	return response;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ElkDialogs *instance = ELK_DIALOGS(self);
	ElkDialogsPrivate *priv = elk_dialogs_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p frame=%p]", iname, self, priv->frame);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
