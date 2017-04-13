/*
   File:    chasearchdialog.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 8, 2015
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

#include "chasearchdialog.h"
#include "chasearchservice.h"
#include "chasearchquerywo.h"
#include "chasearchoccurrence.h"
#include "chasearchpageoccurrences.h"
#include <string.h>

#include "chagladesearch.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaSearchDialog"
#include <logging/catlog.h>

struct _ChaSearchDialogPrivate {
	ChaSearchService *search_service;
	ChaSearchQueryWo *query;
	CatArrayWo *occurrences_list;
	ChaSearchQueryWo *occurrences_query;

	ChaEditor *editor;
	GtkDialog *dialog;
	GtkWidget *w_but_find;
	GtkWidget *w_but_replace;
	GtkWidget *w_but_replace_all;
	GtkWidget *w_but_mark_lines;
	GtkWidget *w_but_close;


	GtkWidget *w_grid2;

	GtkWidget *w_txt_find;
	GtkWidget *w_txt_replace;

	GtkWidget *w_rb_text;
	GtkWidget *w_cb_match_case;
	GtkWidget *w_cb_entire_words;
	GtkWidget *w_cb_regexp;

	GtkWidget *w_cb_backward;
	GtkWidget *w_cb_wrap;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaSearchDialog, cha_search_dialog, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaSearchDialog)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_search_dialog_class_init(ChaSearchDialogClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_search_dialog_init(ChaSearchDialog *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaSearchDialog *instance = CHA_SEARCH_DIALOG(object);
//	ChaSearchDialogPrivate *priv = cha_search_dialog_get_instance_private(instance);
	G_OBJECT_CLASS(cha_search_dialog_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_search_dialog_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static void l_on_find_clicked(GtkButton *button, gpointer user_data);
static void l_on_close_clicked(GtkButton *button, gpointer user_data);
static gboolean l_find_text_changed(GtkEntry *entry, gpointer user_data);
static void l_find_text_activate(GtkEntry *entry, gpointer user_data);
static void l_on_mark_lines_clicked(GtkButton *button, gpointer user_data);
static gboolean l_rb_text_toggled(GtkToggleButton *toggle_button, gpointer user_data);
static gboolean l_delete_event(GtkWindow *window, GdkEvent *event) {
	cat_log_debug("hidding");
	gtk_widget_hide(GTK_WIDGET(window));
	return TRUE;
}

ChaSearchDialog *cha_search_dialog_new(struct _ChaSearchService *search_service) {
	ChaSearchDialog *result = g_object_new(CHA_TYPE_SEARCH_DIALOG, NULL);
	cat_ref_anounce(result);
	ChaSearchDialogPrivate *priv = cha_search_dialog_get_instance_private(result);
	priv->search_service = search_service;
	priv->query = cha_search_query_wo_new();
	priv->occurrences_query = NULL;
	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) chagladesearch_glade, (gsize) chagladesearch_glade_len, (GError **) (&gerror));
	GtkDialog *dialog = (GtkDialog *) gtk_builder_get_object(builder, "dialog1");
	priv->dialog = dialog;

	priv->w_but_find = (GtkWidget *) gtk_builder_get_object(builder, "but_find");

	priv->w_but_replace = (GtkWidget *) gtk_builder_get_object(builder, "but_replace");
	priv->w_but_replace_all = (GtkWidget *) gtk_builder_get_object(builder, "but_replace_all");
	priv->w_but_mark_lines = (GtkWidget *) gtk_builder_get_object(builder, "but_mark_lines");
	priv->w_but_close = (GtkWidget *) gtk_builder_get_object(builder, "but_close");

	priv->w_grid2 = (GtkWidget *) gtk_builder_get_object(builder, "grid2");

	priv->w_txt_find = (GtkWidget *) gtk_builder_get_object(builder, "txt_find");
	priv->w_txt_replace = (GtkWidget *) gtk_builder_get_object(builder, "txt_replace");

	priv->w_rb_text = (GtkWidget *) gtk_builder_get_object(builder, "rb_text");

	priv->w_cb_match_case = (GtkWidget *) gtk_builder_get_object(builder, "cb_match_case");
	priv->w_cb_entire_words = (GtkWidget *) gtk_builder_get_object(builder, "cb_entire_words");
	priv->w_cb_regexp = (GtkWidget *) gtk_builder_get_object(builder, "cb_regexp");

	priv->w_cb_backward = (GtkWidget *) gtk_builder_get_object(builder, "cb_backward");
	priv->w_cb_wrap = (GtkWidget *) gtk_builder_get_object(builder, "cb_wrap_search");


	g_signal_connect(priv->w_txt_find, "changed", G_CALLBACK(l_find_text_changed), result);
	g_signal_connect(priv->w_txt_find, "activate", G_CALLBACK(l_find_text_activate), result);
	g_signal_connect(priv->w_but_find, "clicked", G_CALLBACK(l_on_find_clicked), result);
	g_signal_connect(priv->w_but_mark_lines, "clicked", G_CALLBACK(l_on_mark_lines_clicked), result);
	g_signal_connect(priv->w_but_close, "clicked", G_CALLBACK(l_on_close_clicked), result);
	g_signal_connect(priv->w_rb_text, "toggled", G_CALLBACK(l_rb_text_toggled), result);

	g_signal_connect(dialog, "delete-event", G_CALLBACK(l_delete_event), result);

	return result;
}

void cha_search_dialog_show(ChaSearchDialog *dialog, GtkWindow *owner ) {
	ChaSearchDialogPrivate *priv = cha_search_dialog_get_instance_private(dialog);

	cat_log_debug("owner=%O", owner);
	if (owner) {
		gtk_window_set_transient_for((GtkWindow *) priv->dialog, owner);
	}

	gtk_editable_select_region(GTK_EDITABLE(priv->w_txt_find), 0 , -1);
	gtk_widget_grab_focus(priv->w_txt_find);

	gtk_widget_show_all((GtkWidget *) priv->dialog);
	gtk_window_present(GTK_WINDOW(priv->dialog));
}


static void l_on_find_clicked(GtkButton *button, gpointer user_data) {
	ChaSearchDialog *search_dialog = CHA_SEARCH_DIALOG(user_data);
	ChaSearchDialogPrivate *priv = cha_search_dialog_get_instance_private(search_dialog);

	gboolean match_case = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(priv->w_cb_match_case));
	cha_search_query_wo_set_match_case(priv->query, match_case);
	gboolean match_words = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(priv->w_cb_entire_words));
	cha_search_query_wo_set_match_words(priv->query, match_words);

	gboolean regexp = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(priv->w_cb_regexp));
	cha_search_query_wo_set_regexp(priv->query, regexp);



	ChaSearchQueryWo *query = cha_search_query_wo_clone(priv->query, CAT_CLONE_DEPTH_NONE);
	cha_search_service_set_editor(priv->search_service, priv->editor);
	cha_search_service_set_query(priv->search_service, query);

	gboolean backward_search = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(priv->w_cb_backward));
	gboolean wrap_search = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(priv->w_cb_wrap));

	cha_search_service_find(priv->search_service, backward_search, wrap_search);
}

static void l_on_mark_lines_clicked(GtkButton *button, gpointer user_data) {

	ChaSearchDialog *search_dialog = CHA_SEARCH_DIALOG(user_data);
	ChaSearchDialogPrivate *priv = cha_search_dialog_get_instance_private(search_dialog);

	gboolean match_case = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(priv->w_cb_match_case));
	cha_search_query_wo_set_match_case(priv->query, match_case);
	gboolean regexp = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(priv->w_cb_regexp));
	cha_search_query_wo_set_regexp(priv->query, regexp);

	ChaSearchQueryWo *query = cha_search_query_wo_clone(priv->query, CAT_CLONE_DEPTH_NONE);
	cha_search_service_set_editor(priv->search_service, priv->editor);
	cha_search_service_set_query(priv->search_service, query);

	cha_search_service_mark(priv->search_service);
}

static void l_on_close_clicked(GtkButton *button, gpointer user_data) {
	ChaSearchDialog *search_dialog = CHA_SEARCH_DIALOG(user_data);
	ChaSearchDialogPrivate *priv = cha_search_dialog_get_instance_private(search_dialog);
	gtk_widget_hide(GTK_WIDGET(priv->dialog));
}

static gunichar l_utf8_get_last_unichar(const gchar *txt, int len) {
	gunichar result = 0;
	gchar *current = (gchar *) txt;
	while(TRUE) {
		gchar *next = g_utf8_next_char(current);
		if (next>=txt+len) {
			result = g_utf8_get_char(current);
			break;
		}
		current = next;
	}
	return result;
}

static gchar *l_convert_hex_to_text(const gchar *txt);

static gboolean l_find_text_changed(GtkEntry *entry, gpointer user_data) {
	ChaSearchDialog *search_dialog = CHA_SEARCH_DIALOG(user_data);
	ChaSearchDialogPrivate *priv = cha_search_dialog_get_instance_private(search_dialog);
	const gchar *txt = gtk_entry_get_text(entry);
	cat_log_debug("text changed:%s", txt);
	if (txt==NULL || txt[0]==0) {
		gtk_widget_set_sensitive(priv->w_but_find, FALSE);
		gtk_widget_set_sensitive(priv->w_but_mark_lines, FALSE);
		gtk_widget_set_sensitive(priv->w_cb_entire_words, FALSE);
	} else {
		gtk_widget_set_sensitive(priv->w_but_find, priv->editor!=NULL);
		gtk_widget_set_sensitive(priv->w_but_mark_lines, priv->editor!=NULL);

		int txt_len = strlen(txt);
		gunichar uni_char = g_utf8_get_char_validated(txt, txt_len);

		gboolean not_whole_word = FALSE;

		gboolean is_active = gtk_toggle_button_get_active((GtkToggleButton *) priv->w_rb_text);
		if  (is_active) {
			not_whole_word = g_unichar_isspace(uni_char);

			if (!not_whole_word) {
				uni_char = l_utf8_get_last_unichar(txt, txt_len);
				cat_log_debug("last_char=%c", uni_char);
				not_whole_word = g_unichar_isspace(uni_char);
			}

			CatStringWo *st = cat_string_wo_new_with_len(txt, txt_len);
			cha_search_query_wo_set_text(priv->query, st);
			cat_unref_ptr(st);
		} else {
			gchar *hex_as_text = l_convert_hex_to_text(txt);
			CatStringWo *st = cat_string_wo_new_with(hex_as_text);
			cha_search_query_wo_set_text(priv->query, st);
			cat_unref_ptr(st);
			cat_free_ptr(hex_as_text);
		}

		gtk_widget_set_sensitive(priv->w_cb_entire_words, !not_whole_word);
	}
	return TRUE;
}

static void l_find_text_activate(GtkEntry *entry, gpointer user_data) {
	l_on_find_clicked(NULL, user_data);
}

static const gchar *_TXT_HEX = "0123456789ABCDEF";

static gchar *l_convert_txt_to_hex(const gchar *text) {
	int len = strlen(text);
	gchar *result = g_malloc(len*3);
	int idx, out, left;
	out = 0;
	for(idx=0; idx<len; idx++) {
		int ch = (int) text[idx];
		left = (ch>>4) & 0xf;
		result[out++] = _TXT_HEX[left];
		result[out++] = _TXT_HEX[ch & 0xf];
		if (idx==len-1) {
			result[out++] = 0;
		} else {
			result[out++] = ' ';
		}
	}
	return result;
}

static int l_convert_single_to_hex(char ch) {
	if (ch>='0' && ch<='9') {
		return (int) (ch-'0');
	}

	if (ch>='A' && ch<='F') {
		return 10+(int) (ch-'A');
	}
	if (ch>='a' && ch<='f') {
		return 10+(int) (ch-'a');
	}
	return -1;
}

static gchar *l_convert_hex_to_text(const gchar *txt) {
	GString *buf = g_string_new(NULL);
	int len = strlen(txt);
	int idx;
	int shift;
	gboolean first = TRUE;
	for(idx=0; idx<len; idx++) {
		int sh = l_convert_single_to_hex(txt[idx]);
		if (sh==-1) {
			if (!first) {
				g_string_append_c(buf, (char) shift);
				first = TRUE;
			}
		} else {
			if (first) {
				shift = sh;
				first = FALSE;
			} else {
				shift = (shift<<4) + sh;
				g_string_append_c(buf, (char) shift);
				first = TRUE;
			}
		}
	}
	gchar *result = strdup(buf->str);
	g_string_free(buf, TRUE);
	return result;
}


static gboolean l_rb_text_toggled(GtkToggleButton *toggle_button, gpointer user_data) {
	ChaSearchDialog *search_dialog = CHA_SEARCH_DIALOG(user_data);
	ChaSearchDialogPrivate *priv = cha_search_dialog_get_instance_private(search_dialog);
	gboolean is_active = gtk_toggle_button_get_active(toggle_button);

	gchar *txt = NULL;
	char *ttxt = (gchar *) gtk_entry_get_text(GTK_ENTRY(priv->w_txt_find));

	if (ttxt!=NULL && ttxt[0]!=0) {
		if (!is_active) {
			txt = l_convert_txt_to_hex(ttxt);
		} else {
			txt = l_convert_hex_to_text(ttxt);

		}
		gtk_entry_set_text(GTK_ENTRY(priv->w_txt_find), txt);
	}


	cat_log_debug("_radio_text_toggled: is_active=%d\n", is_active);
	gtk_widget_set_sensitive(priv->w_cb_entire_words, is_active);
	gtk_widget_set_sensitive(priv->w_cb_match_case, is_active);
	gtk_widget_set_sensitive(priv->w_cb_regexp, is_active);
	return TRUE;
}


static void l_update_state(ChaSearchDialog *search_dialog) {
	ChaSearchDialogPrivate *priv = cha_search_dialog_get_instance_private(search_dialog);
	if (priv->editor) {
		GtkEntryBuffer *entry_buffer = gtk_entry_get_buffer((GtkEntry *) priv->w_txt_find);
		gboolean has_find_text = gtk_entry_buffer_get_length(entry_buffer)>0;

		gtk_widget_set_sensitive(priv->w_but_find, has_find_text);
		gtk_widget_set_sensitive(priv->w_but_replace, has_find_text);
		gtk_widget_set_sensitive(priv->w_but_replace_all, has_find_text);
		gtk_widget_set_sensitive(priv->w_but_mark_lines, has_find_text);

		gtk_widget_set_sensitive(priv->w_grid2, TRUE);

	} else {
		gtk_widget_set_sensitive(priv->w_grid2, FALSE);
		gtk_widget_set_sensitive(priv->w_but_find, FALSE);
		gtk_widget_set_sensitive(priv->w_but_replace, FALSE);
		gtk_widget_set_sensitive(priv->w_but_replace_all, FALSE);
		gtk_widget_set_sensitive(priv->w_but_mark_lines, FALSE);
	}
}


void cha_search_dialog_set_editor(ChaSearchDialog *search_dialog, ChaEditor *editor) {
	ChaSearchDialogPrivate *priv = cha_search_dialog_get_instance_private(search_dialog);
//	if (priv->editor==editor) {
//		return;
//	}
	gboolean had_editor = priv->editor!=NULL;
	cat_ref_swap(priv->editor, editor);
	gboolean has_editor = priv->editor!=NULL;
	ChaDocumentView *document_view = cha_editor_get_document_view(editor);
	ChaSelection *selection = cha_document_view_get_selection(document_view);
	if (selection) {
		ChaSelectionClass *selection_class = CHA_SELECTION_GET_CLASS(selection);
		CatStringWo *selected_text = selection_class->getAsText(selection, document_view, FALSE);


		gboolean is_text = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(priv->w_rb_text));
		if (is_text) {
			gtk_entry_set_text(GTK_ENTRY(priv->w_txt_find), cat_string_wo_getchars(selected_text));
		} else {
			/* hex text */
		}
	}
	if (has_editor!=had_editor) {
		l_update_state(search_dialog);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaSearchDialog *instance = CHA_SEARCH_DIALOG(self);
	ChaSearchDialogPrivate *priv = cha_search_dialog_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor=%p]", iname, self, priv->editor);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
