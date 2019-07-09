/*
   File:    jorcontexteditor.c
   Project: jordanella
   Author:  Douwe Vos
   Date:    3 Jul 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#include "jorcontexteditor.h"
#include "jorscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JorContextEditor"
#include <logging/catlog.h>

struct _JorContextEditorPrivate {
	void *dummy;
};

G_DEFINE_TYPE_WITH_CODE(JorContextEditor, jor_context_editor, DRA_TYPE_CONTEXT_EDITOR,  // @suppress("Unused static function")
	G_ADD_PRIVATE(JorContextEditor)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_format(DraContextEditor *context_editor);

static void jor_context_editor_class_init(JorContextEditorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraContextEditorClass *dra_class = DRA_CONTEXT_EDITOR_CLASS(clazz);
	dra_class->format = l_format;
}

static void jor_context_editor_init(JorContextEditor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JorContextEditor *instance = JOR_CONTEXT_EDITOR(object);
//	JorContextEditorPrivate *priv = jor_context_editor_get_instance_private(instance);
	G_OBJECT_CLASS(jor_context_editor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jor_context_editor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JorContextEditor *jor_context_editor_new(DraEditorPanel *editor_panel, JorEditorConnector *connector) {
	JorContextEditor *result = g_object_new(JOR_TYPE_CONTEXT_EDITOR, NULL);
	cat_ref_anounce(result);
	JorContextEditorPrivate *priv = jor_context_editor_get_instance_private(result);
	dra_context_editor_construct((DraContextEditor *) result, editor_panel);
	return result;
}

#define TAB_SIZE                      3
#define FORCE_LINE_BREAK_AFTER_COMMA  FALSE


static void l_format(DraContextEditor *context_editor) {
	DraEditorPanel *panel = dra_context_editor_get_panel(context_editor);
	DraEditor *editor = dra_editor_panel_get_editor(panel);

	ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
	ChaRevisionWo *a_revision = cha_document_get_current_revision_ref(document);

	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

	ChaRevisionReader *revision_reader = cha_revision_reader_new(a_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);
	JorScanner *scanner = jor_scanner_new(utf8_scanner);

	CatArrayWo *token_in = cat_array_wo_new();
	CatArrayWo *token_out = cat_array_wo_new();

	int indent = 0;
	int out_row = 0;
	int out_column = 0;
	int out_offset = 0;

	gboolean done = FALSE;


	JorToken *last_token = NULL;

	while(!done) {
		JorToken *token = jor_scanner_next(scanner);
		cat_log_debug("token=%O", token);
		JorToken *out = NULL;
		switch(token->code) {
			case JOR_SYMBOL_MAP_OPEN :
			case JOR_SYMBOL_LIST_OPEN : {
				if (!FORCE_LINE_BREAK_AFTER_COMMA && last_token!=NULL && last_token->code==JOR_SYMBOL_COMMA) {
					out_column = last_token->end_column+1;
					out_offset = last_token->end_offset+1;
					out_row = last_token->end_row;
				}
				out = jor_token_new(token->code, out_column, out_offset, out_row, out_column+1, out_offset+1, out_row);
				out_row++;
				indent++;
				out_column = indent * TAB_SIZE;
				out_offset = out_column;
			} break;
			case JOR_SYMBOL_MAP_CLOSE :
			case JOR_SYMBOL_LIST_CLOSE : {
				out_row++;
				indent--;
				out_column = indent * TAB_SIZE;
				out = jor_token_new(token->code, out_column, out_offset, out_row, out_column+1, out_offset+1, out_row);
				out_column++;
				out_offset++;
			} break;


			case JOR_SYMBOL_INVALID :
			case JOR_SYMBOL_NULL :
			case JOR_SYMBOL_TRUE :
			case JOR_SYMBOL_FALSE :
			case JOR_SYMBOL_NUMBER :
			case JOR_SYMBOL_KEY :
			case JOR_SYMBOL_STRING : {
				int start_column = out_column;
				int start_offset = out_offset;
				out_column += token->end_column-token->start_column;
				out_offset += token->end_offset-token->start_offset;
				out = jor_token_new(token->code, start_column, start_offset, out_row, out_column, out_offset, out_row);
			} break;

			case JOR_SYMBOL_COMMA : {
				out = jor_token_new(token->code, out_column, out_offset, out_row, out_column+1, out_offset+1, out_row);
				out_column += 2;
				out_column = indent * TAB_SIZE;
				out_offset = out_column;
				out_row++;
			} break;

			case JOR_SYMBOL_COLON : {
				out = jor_token_new(token->code, out_column+1, out_offset+1, out_row, out_column+2, out_offset+2, out_row);
				out_column += 3;
				out_offset += 3;
			} break;

			case JOR_SYMBOL_EOF :
				done = TRUE;
				break;
		}

		if (out) {
			cat_array_wo_append(token_in, (GObject *) token);
			cat_array_wo_append(token_out, (GObject *) out);
			last_token = out;
			cat_unref_ptr(out);
		}
		cat_unref_ptr(token);
	}

	ChaLineEnd line_end = cha_revision_wo_get_line_ends(e_revision);
	if (line_end == CHA_LINE_END_NONE) {
		line_end = CHA_LINE_END_LF;
	}
	CatStringWo *empty_str = cat_string_wo_new_anchored("", 0);
	int count = cat_array_wo_size(token_out);
	int row = 0;
	int column = 0;
	int page_index;

	JorToken *mtoken_out_ahead = NULL;
	JorToken *mtoken_in_ahead = NULL;
	for(int idx=count-1; idx>=0; idx--) {
		JorToken *mtoken_out = (JorToken *) cat_array_wo_get(token_out, idx);
		JorToken *mtoken_in = (JorToken *) cat_array_wo_get(token_in, idx);

		cat_log_debug("idx=%d, in=%O, out=%O", idx, mtoken_in, mtoken_out);

		if (mtoken_out==NULL) {
			continue;
		}


		if (mtoken_out_ahead!=NULL) {

			ChaCursorMWo *sOne = cha_cursorm_wo_new(mtoken_in->end_row, mtoken_in->end_offset);
			ChaCursorMWo *sTwo = cha_cursorm_wo_new(mtoken_in_ahead->start_row, mtoken_in_ahead->start_offset);

			cat_log_debug("removing between %O and %O, line-count=%ld", sOne, sTwo, cha_revision_wo_count_lines(e_revision));


			cha_revision_wo_remove_between_cursors(e_revision, sOne, sTwo, TRUE, NULL);

			cat_log_debug("finished remove");

			CatArrayWo *lines_to_insert = cat_array_wo_new();
			long long ro = mtoken_out->end_row;
			while(ro<mtoken_out_ahead->start_row) {
				ChaLineWo *empty_line = cha_line_wo_new_anchored(cat_ref_ptr(empty_str), line_end);
				cat_array_wo_append(lines_to_insert, (GObject *) empty_line);
				ro++;
				cat_unref_ptr(empty_line);
			}

			int space_count = 0;
			if (mtoken_out->end_row==mtoken_out_ahead->start_row) {
				space_count = mtoken_out_ahead->start_column - mtoken_out->end_column;
			} else {
				space_count = mtoken_out_ahead->start_column;
			}

			CatStringWo *sl = cat_string_wo_new();
			while(space_count>0) {
				cat_log_trace("space_count=%d", space_count);
				if (space_count>=4) {
					cat_string_wo_append_chars_len(sl, "    ", 4);
					space_count-=4;
				} else {
					cat_string_wo_append_chars_len(sl, "    ", space_count);
					space_count = 0;
				}
			}
			ChaLineWo *sl_line = cha_line_wo_new_anchored(sl, CHA_LINE_END_NONE);
			cat_array_wo_append(lines_to_insert, (GObject *) sl_line);
			cat_unref_ptr(sl_line);

			cha_revision_wo_insert_lines(e_revision, sOne, lines_to_insert, TRUE);
			cat_unref_ptr(lines_to_insert);
			cat_unref_ptr(sOne);
			cat_unref_ptr(sTwo);
		}

		mtoken_out_ahead = mtoken_out;
		mtoken_in_ahead = mtoken_in;

	}

	cat_log_debug("done");

	ChaCursorWo *eco = cha_cursor_wo_new();
	cha_revision_wo_set_cursor(e_revision, eco);
	cat_unref_ptr(eco);

	cha_document_anchor_document(document);

	cat_unref_ptr(token_out);
	cat_unref_ptr(token_in);

	cat_unref_ptr(scanner);
	cat_unref_ptr(revision_reader);
	cat_unref_ptr(a_revision);

	cat_unref_ptr(empty_str);

}
