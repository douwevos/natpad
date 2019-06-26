/*
   File:    drastatusbarcontent.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 15, 2015
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

#include "drastatusbarcontent.h"
#include "draeditor.h"
#include <chameleon.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraStatusBarContent"
#include <logging/catlog.h>

struct _DraStatusBarContentPrivate {
	DraEditorPanel *editor_panel;
	GtkWidget *w_lab_location;
	GtkWidget *w_lab_mode;
	GtkWidget *w_lab_read_only;
	GtkWidget *w_lab_encoding;
	GtkImage *w_img_word_wrap;
	ChaCursorWo *last_cursor;
	CatStringWo *last_encoding_name;
	ChaEditMode edit_mode;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface);
static void l_document_view_listener_iface_init(ChaIDocumentViewListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraStatusBarContent, dra_status_bar_content, GTK_TYPE_BOX,
		G_ADD_PRIVATE(DraStatusBarContent)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_IDOCUMENT_LISTENER, l_document_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_IDOCUMENT_VIEW_LISTENER, l_document_view_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_status_bar_content_class_init(DraStatusBarContentClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_status_bar_content_init(DraStatusBarContent *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraStatusBarContent *instance = DRA_STATUS_BAR_CONTENT(object);
	DraStatusBarContentPrivate *priv = dra_status_bar_content_get_instance_private(instance);
	priv->editor_panel = NULL;
	G_OBJECT_CLASS(dra_status_bar_content_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_status_bar_content_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraStatusBarContent *dra_status_bar_content_new(LeaAction *action_toggle_word_wrap, LeaAction *action_toggle_show_whitespaces, LeaAction *action_toggle_mark_occurrences) {
	DraStatusBarContent *result = g_object_new(DRA_TYPE_STATUS_BAR_CONTENT,
            "orientation", GTK_ORIENTATION_HORIZONTAL,
            "spacing",     2, NULL);
	cat_ref_anounce(result);
	DraStatusBarContentPrivate *priv = dra_status_bar_content_get_instance_private(result);
//	GTK_BOX_construct((GtkBox *) result);
	priv->w_lab_location = gtk_label_new("0,0");
	gtk_label_set_width_chars((GtkLabel *) priv->w_lab_location, 15);
	gtk_box_pack_start((GtkBox *) result, priv->w_lab_location, FALSE,TRUE,4);
	priv->w_lab_mode = gtk_label_new("Insert");
	gtk_label_set_width_chars((GtkLabel *) priv->w_lab_mode, 15);
	gtk_box_pack_start((GtkBox *) result, priv->w_lab_mode, FALSE,TRUE,4);

	priv->w_lab_read_only = gtk_label_new("Read only");
	gtk_label_set_width_chars((GtkLabel *) priv->w_lab_read_only, 12);
	gtk_box_pack_start((GtkBox *) result, priv->w_lab_read_only, FALSE,TRUE,4);

	priv->w_lab_encoding = gtk_label_new("UTF-8");
	gtk_label_set_width_chars((GtkLabel *) priv->w_lab_encoding, 12);
	gtk_box_pack_start((GtkBox *) result, priv->w_lab_encoding, FALSE,TRUE,4);

	gtk_widget_set_size_request((GtkWidget *) result, 300, 20);


	LeaToggleImage *ti = lea_toggle_image_new(action_toggle_word_wrap, "/home/dvos/natpad-workspace/natpad/natpad/dist/icons/line_wrap_96.png");
	gtk_box_pack_start((GtkBox *) result, ti, FALSE,TRUE,1);
	LeaToggleImage *ti_ws = lea_toggle_image_new(action_toggle_show_whitespaces, "/home/dvos/natpad-workspace/natpad/natpad/dist/icons/show_whitespaces_96.png");
	gtk_box_pack_start((GtkBox *) result, ti_ws, FALSE,TRUE,1);
	LeaToggleImage *ti_mo = lea_toggle_image_new(action_toggle_mark_occurrences, "/home/dvos/natpad-workspace/natpad/natpad/dist/icons/mark_occurrences_96.png");
	gtk_box_pack_start((GtkBox *) result, ti_mo, FALSE,TRUE,1);

	return result;
}


static void l_update_cursor_location(DraStatusBarContent *status_bar_content) {
	DraStatusBarContentPrivate *priv = dra_status_bar_content_get_instance_private(status_bar_content);
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
		ChaRevisionWo *a_rev = cha_document_get_current_revision_ref(document);
		ChaCursorWo *cursor = cha_revision_wo_get_cursor(a_rev);
		if (priv->last_cursor!=cursor) {
			priv->last_cursor = cursor;
			ChaCursorMWo *curm = cha_revision_wo_to_cursorm(a_rev, cursor);
			long long int row = cha_cursorm_wo_get_row(curm)+1;
			cat_unref_ptr(curm);


			int col = cha_cursor_wo_get_x_cursor_bytes(cursor);
			ChaLineLocationWo *ll = cha_cursor_wo_get_line_location(cursor);
			ChaPageWo *page = cha_revision_wo_page_at(a_rev, cha_line_location_wo_get_page_index(ll));
			cha_page_wo_hold_lines(page);

			const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(page, cha_line_location_wo_get_page_line_index(ll), TRUE);
			if (!utf8_text.only_ascii) {
				const CatStringWo *line_str = cat_string_wo_new_data_len(utf8_text.text, utf8_text.text_len);
				col = cat_string_wo_from_utf8_pos(line_str, col);
				cat_unref_ptr(line_str);
			}
			cha_utf8_text_cleanup(&utf8_text);
			col++;
			cha_page_wo_release_lines(page);

			CatStringWo *e_txt = cat_string_wo_new();
			cat_string_wo_format(e_txt, "Ln %ld, Col %d(%d)", row, col, cha_cursor_wo_get_x_sub(cursor));
			gtk_label_set_text((GtkLabel *) priv->w_lab_location, cat_string_wo_getchars(e_txt));
			cat_unref_ptr(e_txt);
		}
		cat_unref_ptr(a_rev);
	} else {
		priv->last_cursor = NULL;
		gtk_label_set_text((GtkLabel *) priv->w_lab_location, "--");
	}
}

static void l_update_encoding(DraStatusBarContent *status_bar_content) {
	DraStatusBarContentPrivate *priv = dra_status_bar_content_get_instance_private(status_bar_content);
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
		ChaIConverter *converter = cha_document_get_input_converter(document);
		CatStringWo *encoding_name = cha_iconverter_get_name(converter);
		if (!cat_string_wo_equal(encoding_name, priv->last_encoding_name)) {
			priv->last_encoding_name = encoding_name;
			gtk_label_set_text((GtkLabel *) priv->w_lab_encoding, cat_string_wo_getchars(encoding_name));
		}
	} else {
		priv->last_cursor = NULL;
		gtk_label_set_text((GtkLabel *) priv->w_lab_encoding, "--");
	}
}
static void l_update_mode(DraStatusBarContent *status_bar_content) {
	DraStatusBarContentPrivate *priv = dra_status_bar_content_get_instance_private(status_bar_content);
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);
		ChaEditMode edit_mode = cha_document_view_get_edit_mode(document_view);
		if (priv->edit_mode!=edit_mode) {
			priv->edit_mode = edit_mode;
			switch(edit_mode) {
				case CHA_EDIT_MODE_INSERT : {
					gtk_label_set_text((GtkLabel *) priv->w_lab_mode, "Insert");
				} break;
				case CHA_EDIT_MODE_INSERT_CONTEXT : {
					gtk_label_set_text((GtkLabel *) priv->w_lab_mode, "Context Insert");
				} break;
				case CHA_EDIT_MODE_OVERWRITE : {
					gtk_label_set_text((GtkLabel *) priv->w_lab_mode, "Overwrite");
				} break;
			}

		}
	} else {
		gtk_label_set_text((GtkLabel *) priv->w_lab_mode, "--");
		priv->edit_mode = -1;
	}
}

static void l_update_read_write_info(DraStatusBarContent *status_bar_content) {
	DraStatusBarContentPrivate *priv = dra_status_bar_content_get_instance_private(status_bar_content);
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
		gboolean is_read_only = cha_document_is_read_only(document);
		if (is_read_only) {
			gtk_label_set_text((GtkLabel *) priv->w_lab_read_only, "Read Only");
		} else {
			gtk_label_set_text((GtkLabel *) priv->w_lab_read_only, "Writable");
		}
	} else {
		gtk_label_set_text((GtkLabel *) priv->w_lab_read_only, "--");
	}
}



void dra_status_bar_content_set_active_editor_panel(DraStatusBarContent *status_bar_content, DraEditorPanel *editor_panel) {
	DraStatusBarContentPrivate *priv = dra_status_bar_content_get_instance_private(status_bar_content);
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
		cha_document_remove_listener(document, CHA_IDOCUMENT_LISTENER(status_bar_content));
		ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);
		cha_document_view_remove_listener(document_view, CHA_IDOCUMENT_VIEW_LISTENER(status_bar_content));
		cat_unref_ptr(priv->editor_panel);
	}

	priv->editor_panel = cat_ref_ptr(editor_panel);
	if (editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(editor_panel);
		ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
		cha_document_add_listener(document, CHA_IDOCUMENT_LISTENER(status_bar_content));
		ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);
		cha_document_view_add_listener(document_view, CHA_IDOCUMENT_VIEW_LISTENER(status_bar_content));
	}
	l_update_cursor_location(status_bar_content);
	l_update_encoding(status_bar_content);
	l_update_mode(status_bar_content);
	l_update_read_write_info(status_bar_content);
}


/********************* start ChaIDocumentViewListener implementation *********************/

static void l_edit_mode_changed(ChaIDocumentViewListener *self, ChaEditMode edit_mode) {
	l_update_mode(DRA_STATUS_BAR_CONTENT(self));
}


static void l_document_view_listener_iface_init(ChaIDocumentViewListenerInterface *iface) {
	iface->editModeChanged = l_edit_mode_changed;
}

/********************* end ChaIDocumentViewListener implementation *********************/


/********************* start ChaIDocumentListener implementation *********************/

static void l_on_new_revision(ChaIDocumentListener *self, ChaRevisionWo *a_new_revision) {
	DraStatusBarContent *status_bar = DRA_STATUS_BAR_CONTENT(self);
	l_update_cursor_location(status_bar);
	l_update_encoding(status_bar);
}

void l_on_mode_changed(ChaIDocumentListener *self, const ChaModeInfo *mode_info) {
	l_update_read_write_info(DRA_STATUS_BAR_CONTENT(self));
}


static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface) {
	iface->onNewRevision = l_on_new_revision;
	iface->onModeChanged = l_on_mode_changed;
}

/********************* end ChaIDocumentListener implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraStatusBarContent *instance = DRA_STATUS_BAR_CONTENT(self);
	DraStatusBarContentPrivate *priv = dra_status_bar_content_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor_panel=%p]", iname, self, priv->editor_panel);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
