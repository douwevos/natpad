/*
   File:    jagaugmentrequest.c
   Project: jaguar
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

#include "jagaugmentrequest.h"
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAugmentRequest"
#include <logging/catlog.h>

struct _JagAugmentRequestPrivate {
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAugmentRequest, jag_augment_request, DRA_TYPE_AUGMENT_REQUEST, // @suppress("Unused static function")
//		G_ADD_PRIVATE(JagAugmentRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);

static void jag_augment_request_class_init(JagAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAugmentRequestClass *aug_class = DRA_AUGMENT_REQUEST_CLASS(clazz);
	aug_class->runAugment = l_run_augment;
}

static void jag_augment_request_init(JagAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagAugmentRequest *jag_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key) {
	JagAugmentRequest *result = g_object_new(JAG_TYPE_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	dra_augment_request_construct((DraAugmentRequest *) result, document, a_revision, slot_key);
	return result;
}


static int l_map_symbol(JagPToken *token) {
	switch(token->kind) {
		case JAGP_KIND_COLON : return 3;
		case JAGP_KIND_DOT : return 3;
		case JAGP_KIND_COMMA : return 3;
		case JAGP_KIND_SEMI: return 3;

		case JAGP_KIND_EQ : return 1;

		/* conditions */
		case JAGP_KIND_EQEQ : return 1;
		case JAGP_KIND_BANGEQ : return 1;
		case JAGP_KIND_LT : return 1;
		case JAGP_KIND_GT : return 1;
		case JAGP_KIND_LTEQ : return 1;
		case JAGP_KIND_GTEQ : return 1;

		/* literals */
		case JAGP_KIND_STRINGLITERAL : return 5;
//		case JAGP_KIND_OCTAL : return 4;
//		case JAGP_KIND_DECIMAL : return 4;
//		case JAGP_KIND_STRING : return 5;
//		case JAGP_KIND_HEX : return 4;
		case JAGP_KIND_IDENTIFIER : return 1;

		/* unary operators */
		case JAGP_KIND_QUES : return 7;
		case JAGP_KIND_PLUSPLUS : return 7;
		case JAGP_KIND_SUBSUB : return 7;
		case JAGP_KIND_BANG : return 7;
		case JAGP_KIND_TILDE : return 7;

		case JAGP_KIND_STAR : return 7;
		case JAGP_KIND_SLASH : return 7;
		case JAGP_KIND_PERCENT : return 7;
		case JAGP_KIND_PLUS : return 7;
		case JAGP_KIND_SUB : return 7;

		case JAGP_KIND_BAR : return 6;
		case JAGP_KIND_AMP : return 6;
		case JAGP_KIND_BARBAR : return 6;
		case JAGP_KIND_AMPAMP : return 6;
		case JAGP_KIND_CARET : return 6;


//		case JAGP_KIND_DIM : return 6;

		case JAGP_KIND_LBRACE : return 9;
		case JAGP_KIND_RBRACE : return 9;
		case JAGP_KIND_LPAREN : return 9;
		case JAGP_KIND_RPAREN : return 9;
		case JAGP_KIND_LBRACKET : return 4;
		case JAGP_KIND_RBRACKET : return 4;


		case JAGP_KIND_STAREQ : return 2;
		case JAGP_KIND_SLASHEQ : return 2;
		case JAGP_KIND_PERCENTEQ : return 2;
		case JAGP_KIND_PLUSEQ : return 2;
		case JAGP_KIND_SUBEQ : return 1;
		case JAGP_KIND_LTLTEQ : return 2;
		case JAGP_KIND_GTGTEQ : return 2;
		case JAGP_KIND_AMPEQ : return 2;
		case JAGP_KIND_CARETEQ : return 2;
		case JAGP_KIND_BAREQ : return 2;



		case JAGP_KIND_GTGTGTEQ : return 1;
		case JAGP_KIND_COMMENT_EOL : return 15;
		case JAGP_KIND_COMMENT_FULL : return 3;

		case JAGP_KIND_PACKAGE : return 8;
		case JAGP_KIND_IMPORT : return 8;
		case JAGP_KIND_IMPLEMENTS : return 8;
		case JAGP_KIND_INSTANCEOF : return 8;
		case JAGP_KIND_EXTENDS : return 8;

		case JAGP_KIND_PUBLIC : return 4;
		case JAGP_KIND_PRIVATE : return 4;
		case JAGP_KIND_PROTECTED : return 4;
		case JAGP_KIND_STATIC : return 4;
		case JAGP_KIND_ABSTRACT : return 5;
		case JAGP_KIND_TRANSIENT : return 5;
		case JAGP_KIND_FINAL : return 5;
		case JAGP_KIND_VOLATILE : return 5;
		case JAGP_KIND_NATIVE : return 5;

		case JAGP_KIND_DO : return 9;
		case JAGP_KIND_TRY : return 9;
		case JAGP_KIND_THROW : return 9;
		case JAGP_KIND_THROWS : return 9;
		case JAGP_KIND_FINALLY : return 9;
		case JAGP_KIND_BREAK : return 9;
		case JAGP_KIND_ELSE : return 9;
//		case JAGP_KIND_INNER : return 9;
		case JAGP_KIND_CASE : return 9;
//		case JAGP_KIND_GOTO : return 9;
		case JAGP_KIND_RETURN : return 9;
		case JAGP_KIND_CATCH : return 9;
		case JAGP_KIND_NEW : return 9;
		case JAGP_KIND_FOR : return 9;
		case JAGP_KIND_SWITCH : return 9;
		case JAGP_KIND_CONTINUE : return 9;
		case JAGP_KIND_WHILE : return 9;
		case JAGP_KIND_SYNCHRONIZED : return 9;
		case JAGP_KIND_DEFAULT : return 9;
		case JAGP_KIND_IF : return 9;

		case JAGP_KIND_BOOLEAN : return 12;
		case JAGP_KIND_DOUBLE : return 12;
		case JAGP_KIND_BYTE : return 12;
		case JAGP_KIND_INT : return 12;
		case JAGP_KIND_VOID : return 12;
		case JAGP_KIND_FLOAT : return 12;
		case JAGP_KIND_LONG : return 12;
		case JAGP_KIND_SHORT : return 12;
		case JAGP_KIND_CHAR : return 1;

//		case JAGP_KIND_REST : return 15;
//		case JAGP_KIND_VAR : return 15;
//		case JAGP_KIND_CAST : return 15;
//		case JAGP_KIND_FUTURE : return 15;
//		case JAGP_KIND_CONST : return 15;
//		case JAGP_KIND_GENERIC : return 15;

		case JAGP_KIND_INTERFACE : return 10;
		case JAGP_KIND_CLASS : return 10;

		case JAGP_KIND_SUPER : return 8;
//		case JAGP_KIND_NULL : return 8;
		case JAGP_KIND_THIS : return 8;


		default : {} break;
	}
	return 1;
}



static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {
	ChaRevisionReader *revision_reader = cha_revision_reader_new(a_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);

//	JagPScanner *scanner = jagp_scanner_factory_create_scanner(priv->scanner_factory, utf8_scanner);

	JagPNames *names = jagp_names_new();
	JagPTokenizer *tokenizer = jagp_tokenizer_new(utf8_scanner, names);

	JagPLexerImpl *lexer_impl = jagp_lexer_impl_new(tokenizer);
	cat_log_debug("lexer_impl=%O", lexer_impl);
	JagPParser *parser = jagp_parser_new((JagPILexer *) lexer_impl, names);
	jagp_parser_run(parser);
	cat_unref(lexer_impl);

	/* tag lines */
	CatArrayWo *message_list = jagp_parser_get_messages(parser);
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

	CatArrayWo *token_cash = jagp_lexer_impl_get_all_tokens(lexer_impl);
	CatIIterator *iter = cat_array_wo_iterator(token_cash);
	while(cat_iiterator_has_next(iter)) {
		JagPToken *token = (JagPToken *) cat_iiterator_next(iter);
		cat_log_debug("token=%O", token);

		int left_column, right_column;
		long long left_row, right_row;
		jagp_cursor_values(token->cur_start, &left_row, &left_column);
		jagp_cursor_values(token->cur_end, &right_row, &right_column);

		dra_keyword_printer_print_fg_color(keyword_printer, left_row, left_column, right_row, right_column, l_map_symbol(token));
//
//		int sym_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
		if (token->kind==JAGP_KIND_COMMENT_FULL || token->kind==JAGP_KIND_COMMENT_EOL) {
			GObject *val = token->value;
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


//
//
//    JagScanner *scanner = jag_scanner_new(utf8_scanner);
//
//    JagPlainParser *plain_parser = jag_plain_parser_new(JAG_ISCANNER(scanner), keyword_printer, line_tag_printer);
//    jag_plain_parser_run(plain_parser);
//
	cat_unref_ptr(utf8_scanner);
	cat_unref_ptr(names);
	cat_unref_ptr(tokenizer);
	cat_unref_ptr(parser);
	return TRUE;
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

