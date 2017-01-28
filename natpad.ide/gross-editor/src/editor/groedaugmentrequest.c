/*
   File:    groedaugmentrequest.c
   Project: gross-editor
   Author:  Douwe Vos
   Date:    Dec 27, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "groedaugmentrequest.h"
#include "../parser/groedparser.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroEdAugmentRequest"
#include <logging/catlog.h>

struct _GroEdAugmentRequestPrivate {
	GroRunModel *model;
	GroRunITokenFactory *token_factory;
	GroScannerFactory *scanner_factory;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroEdAugmentRequest, groed_augment_request, DRA_TYPE_AUGMENT_REQUEST,
		G_ADD_PRIVATE(GroEdAugmentRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);

static void groed_augment_request_class_init(GroEdAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAugmentRequestClass *aug_class = DRA_AUGMENT_REQUEST_CLASS(clazz);
	aug_class->runAugment = l_run_augment;
}

static void groed_augment_request_init(GroEdAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroEdAugmentRequest *instance = GROED_AUGMENT_REQUEST(object);
	GroEdAugmentRequestPrivate *priv = groed_augment_request_get_instance_private(instance);
	cat_unref_ptr(priv->model);
	cat_unref_ptr(priv->scanner_factory);
	cat_unref_ptr(priv->token_factory);
	G_OBJECT_CLASS(groed_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(groed_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

GroEdAugmentRequest *groed_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key,
		GroRunModel *model, GroRunITokenFactory *token_factory, GroScannerFactory *scanner_factory) {
	GroEdAugmentRequest *result = g_object_new(GROED_TYPE_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	GroEdAugmentRequestPrivate *priv = groed_augment_request_get_instance_private(result);
	priv->model = cat_ref_ptr(model);
	priv->scanner_factory = cat_ref_ptr(scanner_factory);
	priv->token_factory = cat_ref_ptr(token_factory);
	dra_augment_request_construct((DraAugmentRequest *) result, document, a_revision, slot_key);
	return result;
}


static int l_map_symbol(GroRunIToken *token);

static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {
	GroEdAugmentRequestPrivate *priv = groed_augment_request_get_instance_private(GROED_AUGMENT_REQUEST(request));

	ChaRevisionReader *revision_reader = cha_revision_reader_new(a_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);


	GroScanner *scanner = gro_scanner_factory_create_scanner(priv->scanner_factory, utf8_scanner);

//	GroEdPlainParser *plain_parser = groed_plain_parser_new(scanner, token_factory, priv->model, keyword_printer, line_tag_printer);
//	groed_plain_parser_run(plain_parser);

	GroEdParser *symantic_parser = groed_parser_new(scanner, priv->token_factory, priv->model);
	groed_parser_run(symantic_parser);


	/* tag lines */
	CatArrayWo *message_list = groed_parser_get_messages(symantic_parser);
	if (message_list) {
		long left_row, right_row;
		int left_column, right_column;
		CatIIterator *iter = cat_array_wo_iterator(message_list);
		while(cat_iiterator_has_next(iter)) {
			GroRunMessage *message = (GroRunMessage *) cat_iiterator_next(iter);
			cat_log_debug("message=%O", message);
			GroRunLocation *location = grorun_message_get_location(message);
			grorun_location_get_all(location, &left_column, &left_row, &right_column, &right_row);

			DraLineTagWo *line_tag = dra_line_tag_wo_new(left_row, DRA_TAG_TYPE_PARSER_ERROR);
			dra_line_tag_wo_set_color(line_tag, 0.9,0,0.7);
			dra_line_tag_wo_set_start_and_end_index(line_tag, left_column, right_column-1);
			dra_line_tag_wo_set_text(line_tag, grorun_message_get_text(message));
			dra_keyword_printer_print_line_tag(line_tag_printer, line_tag);
		}
		cat_unref_ptr(iter);
	}

	dra_keyword_printer_flush_line_tags(line_tag_printer);
	dra_keyword_printer_reset(line_tag_printer);

	/* highlight tokens */

	DraSpellHelper *spell_helper = dra_augment_request_get_spell_helper(request);

	CatArrayWo *token_cash = groed_parser_get_token_cash(symantic_parser);
	CatIIterator *iter = cat_array_wo_iterator(token_cash);
	while(cat_iiterator_has_next(iter)) {
		GroRunIToken *token = (GroRunIToken *) cat_iiterator_next(iter);

		cat_log_debug("token=%O", token);

		GroRunLocation *location = grorun_itoken_get_location(token);

		int left_column, right_column;
		long left_row, right_row;
		grorun_location_get_all(location, &left_column, &left_row, &right_column, &right_row);

		dra_keyword_printer_print_fg_color(keyword_printer, left_row, left_column, right_row, right_column, l_map_symbol(token));
//
		int sym_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
		if (sym_index==GRO_SYM_EOL_COMMENT) {
			GObject *val = grorun_itoken_get_value(token);
			if (CAT_IS_STRING_WO(val)) {
				dra_spell_helper_scan(spell_helper, line_tag_printer, (CatStringWo *) val, left_column, left_row);
			}
		}

//		if (token->is_error) {
//			cat_log_debug("error !!! row=%d, token->left=%d, token->right=%d", token->left_row, token->left, token->right);
//			DraLineTagWo *line_tag = dra_line_tag_wo_new(token->left_row);
//			dra_line_tag_wo_set_color(line_tag, 1,0,0);
//			dra_line_tag_wo_set_start_and_end_index(line_tag, token->left, token->right-1);
//			dra_keyword_printer_print_line_tag(priv->line_tag_printer, line_tag);
//		}
	}
	cat_unref_ptr(iter);

	cat_unref_ptr(utf8_scanner);
	cat_unref_ptr(symantic_parser);
	cat_unref_ptr(scanner);
	return TRUE;
}


static int l_map_symbol(GroRunIToken *token) {
	GroRunFullToken *full_token = GRORUN_FULL_TOKEN(token);
	int sym_index = grorun_full_token_get_user_index(full_token);
	switch(sym_index) {
		case GRO_SYM_END_OF_INPUT : return 3;
		case GRO_SYM_BAR : return 4;
		case GRO_SYM_COLON : return 4;
		case GRO_SYM_COMMA : return 4;
		case GRO_SYM_IDENTIFIER : return 1;
		case GRO_SYM_INDEX_MARK : return 11;

		case GRO_SYM_KW_CODE : return 9;
		case GRO_SYM_KW_IMPORTS : return 9;
		case GRO_SYM_KW_TERMINAL : return 9;
		case GRO_SYM_KW_TYPE : return 9;

		case GRO_SYM_LSQBRACK: return 4;
		case GRO_SYM_RSQBRACK: return 4;

		case GRO_SYM_CODE_TEXT : return 2;
		case GRO_SYM_NUMBER : return 5;
		case GRO_SYM_RULE_SEPERATOR : return 4;
		case GRO_SYM_SEMI : return 13;
		case GRO_SYM_EOL_COMMENT : return 15;

		case GROED_SYM_MARKED_TERMINAL : return 11;
		case GROED_SYM_MARKED_NON_TERMINAL : return 2;

		default : {} break;
	}
	return 1;
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
