/*
   File:    drabasicaugmentrequest.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 12, 2019
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

#include "drabasicaugmentrequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraBasicAugmentRequest"
#include <logging/catlog.h>

struct _DraBasicAugmentRequestPrivate {
	void *dummy;
};

G_DEFINE_TYPE_WITH_CODE(DraBasicAugmentRequest, dra_basic_augment_request, DRA_TYPE_AUGMENT_REQUEST,  // @suppress("Unused static function")
	G_ADD_PRIVATE(DraBasicAugmentRequest)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);

static void dra_basic_augment_request_class_init(DraBasicAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAugmentRequestClass *aug_class = DRA_AUGMENT_REQUEST_CLASS(clazz);
	aug_class->runAugment = l_run_augment;
}

static void dra_basic_augment_request_init(DraBasicAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraBasicAugmentRequest *instance = DRA_BASIC_AUGMENT_REQUEST(object);
//	DraBasicAugmentRequestPrivate *priv = dra_basic_augment_request_get_instance_private(instance);
	G_OBJECT_CLASS(dra_basic_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_basic_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraBasicAugmentRequest *dra_basic_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *a_slot_key) {
	DraBasicAugmentRequest *result = g_object_new(DRA_TYPE_BASIC_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	dra_augment_request_construct((DraAugmentRequest *) result, document, a_revision, a_slot_key);
	return result;
}


static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {

	DraSpellHelper *spell_helper = dra_augment_request_get_spell_helper(request);
	long long int row = 0;

	ChaLineIterator *line_iterator = cha_line_iterator_new(a_revision, 0);
	while(cat_iiterator_has_next((CatIIterator *) line_iterator)) {
		ChaLineLocationWo *line_location = (ChaLineLocationWo *) cat_iiterator_next((CatIIterator *) line_iterator);
		cat_log_debug("next iter:%o", line_location);
		ChaLineWo *a_raw_line = cha_revision_wo_line_at_location(a_revision, line_location);
		CatStringWo *a_text = cha_line_wo_get_text(a_raw_line);
		dra_spell_helper_scan(spell_helper, line_tag_printer, a_text, 0, row);
		row++;
		cat_unref_ptr(a_raw_line);
	}

	cat_unref_ptr(line_iterator);

//
//	ChaRevisionReader *revision_reader = cha_revision_reader_new(a_revision, NULL, NULL);
//	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
//	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);
//
////	JagPScanner *scanner = jagp_scanner_factory_create_scanner(priv->scanner_factory, utf8_scanner);
//
//	JagPNames *names = jagp_names_new();
//	JagPTokenizer *tokenizer = jagp_tokenizer_new(utf8_scanner, names);
//
//	JagPLexerImpl *lexer_impl = jagp_lexer_impl_new(tokenizer);
//	cat_log_debug("lexer_impl=%O", lexer_impl);
//	JagPParser *parser = jagp_parser_new((JagPILexer *) lexer_impl, names);
//	jagp_parser_run(parser);
//	cat_unref(lexer_impl);
//
//	/* tag lines */
//	CatArrayWo *message_list = jagp_parser_get_messages(parser);
//	if (message_list) {
//		long left_row, right_row;
//		int left_column, right_column;
//		CatIIterator *iter = cat_array_wo_iterator(message_list);
//		while(cat_iiterator_has_next(iter)) {
//			GroRunMessage *message = (GroRunMessage *) cat_iiterator_next(iter);
//			cat_log_debug("message=%O", message);
//			GroRunLocation *location = grorun_message_get_location(message);
//			grorun_location_get_all(location, &left_column, &left_row, &right_column, &right_row);
//
//			DraLineTagWo *line_tag = dra_line_tag_wo_new(left_row, DRA_TAG_TYPE_PARSER_ERROR);
//			dra_line_tag_wo_set_color(line_tag, 0.9,0,0.7);
//			dra_line_tag_wo_set_start_and_end_index(line_tag, left_column, right_column-1);
//			dra_line_tag_wo_set_text(line_tag, grorun_message_get_text(message));
//			dra_keyword_printer_print_line_tag(line_tag_printer, line_tag);
//		}
//		cat_unref_ptr(iter);
//	}
//
//	dra_keyword_printer_flush_line_tags(line_tag_printer);
//	dra_keyword_printer_reset(line_tag_printer);
//
//
//	/* highlight tokens */
//
//	DraSpellHelper *spell_helper = dra_augment_request_get_spell_helper(request);
//
//	CatArrayWo *token_cash = jagp_lexer_impl_get_all_tokens(lexer_impl);
//	CatIIterator *iter = cat_array_wo_iterator(token_cash);
//	while(cat_iiterator_has_next(iter)) {
//		JagPToken *token = (JagPToken *) cat_iiterator_next(iter);
//		cat_log_debug("token=%O", token);
//
//		int left_column, right_column;
//		long long left_row, right_row;
//		jagp_cursor_values(token->cur_start, &left_row, &left_column);
//		jagp_cursor_values(token->cur_end, &right_row, &right_column);
//
//		dra_keyword_printer_print_fg_color(keyword_printer, left_row, left_column, right_row, right_column, l_map_symbol(token));
////
////		int sym_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
//		if (token->kind==JAGP_KIND_COMMENT_FULL || token->kind==JAGP_KIND_COMMENT_EOL) {
//			GObject *val = token->value;
//			if (CAT_IS_STRING_WO(val)) {
//				dra_spell_helper_scan(spell_helper, line_tag_printer, (CatStringWo *) val, left_column, left_row);
//			}
//		}
//
////		if (token->is_error) {
////			cat_log_debug("error !!! row=%d, token->left=%d, token->right=%d", token->left_row, token->left, token->right);
////			DraLineTagWo *line_tag = dra_line_tag_wo_new(token->left_row);
////			dra_line_tag_wo_set_color(line_tag, 1,0,0);
////			dra_line_tag_wo_set_start_and_end_index(line_tag, token->left, token->right-1);
////			dra_keyword_printer_print_line_tag(priv->line_tag_printer, line_tag);
////		}
//	}
//	cat_unref_ptr(iter);
//
//
////
////
////    JagScanner *scanner = jag_scanner_new(utf8_scanner);
////
////    JagPlainParser *plain_parser = jag_plain_parser_new(JAG_ISCANNER(scanner), keyword_printer, line_tag_printer);
////    jag_plain_parser_run(plain_parser);
////
//	cat_unref_ptr(utf8_scanner);
//	cat_unref_ptr(names);
//	cat_unref_ptr(tokenizer);
//	cat_unref_ptr(parser);
	return TRUE;
}
