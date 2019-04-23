/*
   File:    dracontexteditor.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 13, 2015
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

#include "dracontexteditor.h"
#include "../draeditorpanel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraContextEditor"
#include <logging/catlog.h>

struct _DraContextEditorPrivate {
	DraEditorPanel *editor_panel;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraContextEditor, dra_context_editor, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraContextEditor)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_context_editor_class_init(DraContextEditorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	clazz->format = NULL;
	clazz->openDeclaration = NULL;
}

static void dra_context_editor_init(DraContextEditor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraContextEditor *instance = DRA_CONTEXT_EDITOR(object);
//	DraContextEditorPrivate *priv = dra_context_editor_get_instance_private(instance);
//	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_context_editor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_context_editor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void dra_context_editor_construct(DraContextEditor *context_editor, struct _DraEditorPanel *editor_panel) {
	DraContextEditorPrivate *priv = dra_context_editor_get_instance_private(context_editor);
//	priv->editor_panel = cat_ref_ptr(editor_panel);
	priv->editor_panel = editor_panel;
}

DraContextEditor *dra_context_editor_new(struct _DraEditorPanel *editor_panel) {
	DraContextEditor *result = g_object_new(DRA_TYPE_CONTEXT_EDITOR, NULL);
	cat_ref_anounce(result);
	dra_context_editor_construct(result, editor_panel);
	return result;
}

struct _DraEditorPanel *dra_context_editor_get_panel(DraContextEditor *context_editor) {
	DraContextEditorPrivate *priv = dra_context_editor_get_instance_private(context_editor);
	return priv->editor_panel;
}


gboolean dra_context_editor_indent(DraContextEditor *context_editor, gboolean increase_indent) {
	DraContextEditorPrivate *priv = dra_context_editor_get_instance_private(context_editor);
	DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
	ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
	ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);

	ChaLineLocationWo *ll_first = NULL;
	ChaLineLocationWo *ll_last = NULL;
	int valid_end_column;
	cha_uow_get_first_and_last_row(document_view, &ll_first, &ll_last, &valid_end_column);
	if (ll_first==NULL || ll_last==NULL) {
		cat_unref_ptr(ll_first);
		cat_unref_ptr(ll_last);
		return FALSE;
	}

	ChaSelection *selection = cha_document_view_get_selection(document_view);

	if (cha_line_location_wo_equal(ll_first, ll_last) && increase_indent) {
		if (selection == NULL) {
			cat_unref_ptr(ll_first);
			cat_unref_ptr(ll_last);
			return FALSE;
		}
//		if (line_iterator->line_info.line->text_length>= end_column) {
//			return FALSE;
//		}
	}

	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

	long long int first_row = cha_revision_wo_calculate_row(e_revision, ll_first);
	long long int last_row = cha_revision_wo_calculate_row(e_revision, ll_last);



	const ChaDocumentViewContext dvctx = cha_document_view_get_context(document_view);

	const int space_width = dvctx.ps_space_width/PANGO_SCALE;
	const int tab_width = dvctx.tab_size;
	int char_idx, break_idx, view_pos;

	ChaCursorWo *start_cursor = NULL;
	ChaCursorWo *end_cursor = NULL;
	gboolean selection_is_reverse = FALSE;

	if (selection) {
		start_cursor = cha_selection_get_start(selection);
		end_cursor = cha_selection_get_end(selection);
		start_cursor = cha_cursor_wo_clone(start_cursor, CAT_CLONE_DEPTH_FULL);
		end_cursor = cha_cursor_wo_clone(end_cursor, CAT_CLONE_DEPTH_FULL);
		ChaCursorWo *rev_cursor = cha_revision_wo_get_cursor(e_revision);
		selection_is_reverse = cha_cursor_wo_equal(start_cursor, rev_cursor);
	} else {
		start_cursor = cha_revision_wo_get_cursor(e_revision);
		start_cursor = cha_cursor_wo_clone(start_cursor, CAT_CLONE_DEPTH_FULL);
		end_cursor = cat_ref_ptr(start_cursor);
	}

	ChaLineLocationWo *start_ll = cha_cursor_wo_get_line_location(start_cursor);
	ChaLineLocationWo *end_ll = cha_cursor_wo_get_line_location(end_cursor);

	long long int line_idx = first_row;
	ChaLineIterator *line_iterator = cha_line_iterator_new_line_location(e_revision, ll_first);

	ChaCursorMWo *cursor_a = cha_cursorm_wo_new(0,0);
	ChaCursorMWo *cursor_b = cha_cursorm_wo_new(0,0);

	int current_page_index = -1;
	ChaPageWo *e_current_page = NULL;

	while(line_idx<=last_row) {

		ChaLineLocationWo *line_location = (ChaLineLocationWo *) cat_iiterator_next((CatIIterator *) line_iterator);
		if (e_current_page==NULL || cha_line_location_wo_get_page_index(line_location)!=current_page_index) {
			if (e_current_page) {
				cha_page_wo_release_lines(e_current_page);
			}
			current_page_index = cha_line_location_wo_get_page_index(line_location);
			e_current_page = cha_revision_wo_editable_page_at(e_revision, current_page_index);
			if (e_current_page == NULL) {
				cat_log_warn("Unexpected NULL page: page_index=%d", current_page_index);
				break;
			}
			cha_page_wo_hold_lines(e_current_page);
		}

		gboolean is_on_start_cursor_line = cha_line_location_wo_equal(start_ll, line_location);
		gboolean is_on_end_cursor_line = cha_line_location_wo_equal(end_ll, line_location);

		ChaLineWo *e_line = cha_page_wo_editable_line_at(e_current_page, cha_line_location_wo_get_page_line_index(line_location));
		CatStringWo *e_text = cha_line_wo_get_text(e_line);
		int text_length = cha_line_wo_byte_count(e_line);

		break_idx = 0;
		if (!increase_indent) {
			view_pos = 0;
			for(char_idx=0; char_idx<text_length; char_idx++) {
				char ch = cat_string_wo_char_at(e_text, char_idx);
				if (ch=='\t') {
					break_idx = char_idx+1;
					char_idx = text_length;
				} else if (ch==' ') {
					view_pos = view_pos+space_width;
					if (view_pos>tab_width) {
						break_idx = char_idx;
						char_idx = text_length;
					} else if (view_pos == tab_width) {
						break_idx = char_idx+1;
						char_idx = text_length;
					}
				} else {
					break_idx = char_idx;
					char_idx = text_length;
				}
			}
			if (break_idx>0) {
				cha_cursorm_wo_set_row(cursor_a, line_idx);
				cha_cursorm_wo_set_column(cursor_a, 0);
				cha_cursorm_wo_set_row(cursor_b, line_idx);
				cha_cursorm_wo_set_column(cursor_b, break_idx);
				cha_revision_wo_remove_between_cursors(e_revision, cursor_a, cursor_b, TRUE, NULL);
				if (is_on_start_cursor_line) {
					int xb = cha_cursor_wo_get_x_cursor_bytes(start_cursor);
					xb -= break_idx;
					if (xb<0) {
						xb = 0;
					}
					cha_cursor_wo_set_x_cursor_bytes(start_cursor, xb);
				}

				if ((is_on_end_cursor_line) && (end_cursor!=start_cursor)) {
					int xb = cha_cursor_wo_get_x_cursor_bytes(end_cursor);
					xb -= break_idx;
					if (xb<0) {
						xb = 0;
					}
					cha_cursor_wo_set_x_cursor_bytes(end_cursor, xb);
				}
			}
		} else {
			cha_cursorm_wo_set_row(cursor_a, line_idx);
			cha_cursorm_wo_set_column(cursor_a, 0);

			CatArrayWo *lines = cat_array_wo_new_size(1);
			ChaLineWo *tab_line = cha_line_wo_new_with(cat_string_wo_new_with("\t"), CHA_LINE_END_NONE);
			cat_array_wo_append(lines, (GObject *) tab_line);
			cat_unref_ptr(tab_line);
			cha_revision_wo_insert_lines(e_revision, cursor_a, lines, TRUE);
			break_idx = -1;
			cat_unref_ptr(lines);

			if (is_on_start_cursor_line) {
				int xb = cha_cursor_wo_get_x_cursor_bytes(start_cursor);
				xb++;
				cha_cursor_wo_set_x_cursor_bytes(start_cursor, xb);
			}

			if ((is_on_end_cursor_line) && (end_cursor)) {
				int xb = cha_cursor_wo_get_x_cursor_bytes(end_cursor);
				xb++;
				cha_cursor_wo_set_x_cursor_bytes(end_cursor, xb);
			}

		}

		line_idx++;
	}
	cat_unref_ptr(line_iterator);
	cat_unref_ptr(cursor_a);
	cat_unref_ptr(cursor_b);

	if (e_current_page) {
		cha_page_wo_release_lines(e_current_page);
	}

	if (selection) {
		cha_selection_set_start(selection, start_cursor);
		cha_selection_set_end(selection, end_cursor);
		cha_document_view_notify_selection_changed(document_view);
		if (selection_is_reverse) {
			cha_revision_wo_set_cursor(e_revision, start_cursor);
		} else {
			cha_revision_wo_set_cursor(e_revision, end_cursor);
		}
	} else {
		cha_revision_wo_set_cursor(e_revision, start_cursor);
	}



	if (!is_editable) {
		cha_document_anchor_document(document);
	}
	cat_unref_ptr(start_cursor);
	cat_unref_ptr(end_cursor);
	cat_unref_ptr(ll_first);
	cat_unref_ptr(ll_last);
	return TRUE;
}


static int l_find_line_comment_offset(ChaLineWo *line) {
	int result = -1;
	CatStringWo *text = cha_line_wo_get_text(line);
	int text_len = cat_string_wo_length(text);
	int text_idx;
	gboolean skipping = TRUE;
	for(text_idx=0; text_idx<text_len; text_idx++) {
		char ch = cat_string_wo_char_at(text, text_idx);
		if (skipping) {
			if ((ch==' ') || (ch=='\t')) {
				continue;
			} else if (ch=='/') {
				skipping = FALSE;
				continue;
			}
			break;
		} else if (ch=='/') {
			result = text_idx-1;
		}
		break;
	}
	cat_log_debug("line=%p, result=%d", line, result);
	return result;
}

void dra_context_editor_toggle_line_comment(DraContextEditor *context_editor) {
	DraContextEditorPrivate *priv = dra_context_editor_get_instance_private(context_editor);
	DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
	ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
	ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);

	ChaLineLocationWo *ll_first = NULL;
	ChaLineLocationWo *ll_last = NULL;
	int valid_end_column;
	cha_uow_get_first_and_last_row(document_view, &ll_first, &ll_last, &valid_end_column);
	if (ll_first==NULL || ll_last==NULL) {
		cat_unref_ptr(ll_first);
		cat_unref_ptr(ll_last);
		return;
	}

	ChaSelection *selection = cha_document_view_get_selection(document_view);

	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

	long long int first_row = cha_revision_wo_calculate_row(e_revision, ll_first);
	long long int last_row = cha_revision_wo_calculate_row(e_revision, ll_last);

	ChaCursorWo *start_cursor = NULL;
	ChaCursorWo *end_cursor = NULL;
	gboolean selection_is_reverse = FALSE;

	if (selection) {
		start_cursor = cha_selection_get_start(selection);
		end_cursor = cha_selection_get_end(selection);
		start_cursor = cha_cursor_wo_clone(start_cursor, CAT_CLONE_DEPTH_FULL);
		end_cursor = cha_cursor_wo_clone(end_cursor, CAT_CLONE_DEPTH_FULL);
		ChaCursorWo *rev_cursor = cha_revision_wo_get_cursor(e_revision);
		selection_is_reverse = cha_cursor_wo_equal(start_cursor, rev_cursor);
	} else {
		start_cursor = cha_revision_wo_get_cursor(e_revision);
		start_cursor = cha_cursor_wo_clone(start_cursor, CAT_CLONE_DEPTH_FULL);
	}

	ChaLineLocationWo *start_ll = cha_cursor_wo_get_line_location(start_cursor);
	ChaLineLocationWo *end_ll = cha_cursor_wo_get_line_location(end_cursor);

	long long int line_idx = first_row;
	ChaLineIterator *line_iterator = cha_line_iterator_new_line_location(e_revision, ll_first);

	gboolean should_remove_comment = TRUE;
	int current_page_index = -1;
	ChaPageWo *e_current_page = NULL;

	while(should_remove_comment && line_idx<=last_row) {
		ChaLineLocationWo *line_location = (ChaLineLocationWo *) cat_iiterator_next((CatIIterator *) line_iterator);
		if (e_current_page==NULL || cha_line_location_wo_get_page_index(line_location)!=current_page_index) {
			if (e_current_page) {
				cha_page_wo_release_lines(e_current_page);
			}
			current_page_index = cha_line_location_wo_get_page_index(line_location);
			e_current_page = cha_revision_wo_editable_page_at(e_revision, current_page_index);
			if (e_current_page == NULL) {
				cat_log_warn("Unexpected NULL page: page_index=%d", current_page_index);
				break;
			}
			cha_page_wo_hold_lines(e_current_page);
		}
		ChaLineWo *line = cha_page_wo_line_at(e_current_page, cha_line_location_wo_get_page_line_index(line_location));
		if (l_find_line_comment_offset(line)<0) {
			should_remove_comment = FALSE;
		}
		line_idx++;
		cat_unref_ptr(line);
	}

	ChaCursorMWo *cursor_a = cha_cursorm_wo_new(0,0);
	ChaCursorMWo *cursor_b = cha_cursorm_wo_new(0,0);
	line_iterator = cha_line_iterator_new_line_location(e_revision, ll_first);
	line_idx = first_row;
	while(line_idx<=last_row) {
		ChaLineLocationWo *line_location = (ChaLineLocationWo *) cat_iiterator_next((CatIIterator *) line_iterator);
		if (e_current_page==NULL || cha_line_location_wo_get_page_index(line_location)!=current_page_index) {
			if (e_current_page) {
				cha_page_wo_release_lines(e_current_page);
			}
			current_page_index = cha_line_location_wo_get_page_index(line_location);
			e_current_page = cha_revision_wo_editable_page_at(e_revision, current_page_index);
			if (e_current_page == NULL) {
				cat_log_warn("Unexpected NULL page: page_index=%d", current_page_index);
				break;
			}
			cha_page_wo_hold_lines(e_current_page);
		}
		ChaLineWo *e_line = cha_page_wo_editable_line_at(e_current_page, cha_line_location_wo_get_page_line_index(line_location));

		gboolean is_on_start_cursor_line = cha_line_location_wo_equal(start_ll, line_location);
		gboolean is_on_end_cursor_line = cha_line_location_wo_equal(end_ll, line_location);

		if (should_remove_comment) {

			int comment_idx = l_find_line_comment_offset(e_line);

			cha_cursorm_wo_set_row(cursor_a, line_idx);
			cha_cursorm_wo_set_column(cursor_a, comment_idx);
			cha_cursorm_wo_set_row(cursor_b, line_idx);
			cha_cursorm_wo_set_column(cursor_b, comment_idx+2);
			cha_revision_wo_remove_between_cursors(e_revision, cursor_a, cursor_b, TRUE, NULL);
			if (is_on_start_cursor_line) {
				int xb = cha_cursor_wo_get_x_cursor_bytes(start_cursor);
				if (xb>comment_idx) {
					xb -= comment_idx;
				}
				cha_cursor_wo_set_x_cursor_bytes(start_cursor, xb);
			}

			if ((is_on_end_cursor_line) && (end_cursor)) {
				int xb = cha_cursor_wo_get_x_cursor_bytes(end_cursor);
				if (xb>comment_idx) {
					xb -= comment_idx;
				}
				cha_cursor_wo_set_x_cursor_bytes(end_cursor, xb);
			}
		} else {
			cha_cursorm_wo_set_row(cursor_a, line_idx);
			cha_cursorm_wo_set_column(cursor_a, 0);

			CatArrayWo *lines = cat_array_wo_new_size(1);
			ChaLineWo *tab_line = cha_line_wo_new_with(cat_string_wo_new_with("//"), CHA_LINE_END_NONE);
			cat_array_wo_append(lines, (GObject *) tab_line);
			cha_revision_wo_insert_lines(e_revision, cursor_a, lines, TRUE);

			if (is_on_start_cursor_line) {
				int xb = cha_cursor_wo_get_x_cursor_bytes(start_cursor);
				xb+=2;
				cha_cursor_wo_set_x_cursor_bytes(start_cursor, xb);
			}

			if ((is_on_end_cursor_line) && (end_cursor)) {
				int xb = cha_cursor_wo_get_x_cursor_bytes(end_cursor);
				xb+=2;
				cha_cursor_wo_set_x_cursor_bytes(end_cursor, xb);
			}

		}

		line_idx++;
	}

	if (e_current_page) {
		cha_page_wo_release_lines(e_current_page);
	}


	if (selection) {
		cha_selection_set_start(selection, start_cursor);
		cha_selection_set_end(selection, end_cursor);
		cha_document_view_notify_selection_changed(document_view);
		if (selection_is_reverse) {
			cha_revision_wo_set_cursor(e_revision, start_cursor);
		} else {
			cha_revision_wo_set_cursor(e_revision, end_cursor);
		}
	} else {
		cha_revision_wo_set_cursor(e_revision, start_cursor);
	}



	if (!is_editable) {
		cha_document_anchor_document(document);
	}
	cat_unref_ptr(ll_first);
	cat_unref_ptr(ll_last);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraContextEditor *instance = DRA_CONTEXT_EDITOR(self);
	DraContextEditorPrivate *priv = dra_context_editor_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor_panel=%p]", iname, self, priv->editor_panel);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
