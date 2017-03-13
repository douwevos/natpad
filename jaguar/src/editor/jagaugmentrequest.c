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
#include "../parser/jagscanner.h"
#include "../parser/jagplainparser.h"
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAugmentRequest"
#include <logging/catlog.h>

struct _JagAugmentRequestPrivate {
	GroRunModel *model;
	JagPScannerFactory *scanner_factory;
	GroRunITokenFactory *token_factory;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAugmentRequest, jag_augment_request, DRA_TYPE_AUGMENT_REQUEST,
		G_ADD_PRIVATE(JagAugmentRequest)
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
	JagAugmentRequest *instance = JAG_AUGMENT_REQUEST(object);
	JagAugmentRequestPrivate *priv = jag_augment_request_get_instance_private(instance);
	G_OBJECT_CLASS(jag_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagAugmentRequest *jag_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key,
		GroRunModel *model, GroRunITokenFactory *token_factory, JagPScannerFactory *scanner_factory) {
	JagAugmentRequest *result = g_object_new(JAG_TYPE_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	JagAugmentRequestPrivate *priv = jag_augment_request_get_instance_private(result);
	dra_augment_request_construct((DraAugmentRequest *) result, document, a_revision, slot_key);
	priv->scanner_factory = cat_ref_ptr(scanner_factory);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->model = cat_ref_ptr(model);
	return result;
}


static int l_map_symbol(GroRunIToken *token) {
	GroRunFullToken *full_token = GRORUN_FULL_TOKEN(token);
	int sym_index = grorun_full_token_get_user_index(full_token);
	switch(sym_index) {
		case JAGP_SYM_L_COLON : return 3;
		case JAGP_SYM_L_DOT : return 3;
		case JAGP_SYM_L_COMMA : return 3;
		case JAGP_SYM_L_SEMI: return 3;

		case JAGP_SYM_L_IS : return 1;

		/* conditions */
		case JAGP_SYM_L_EQ : return 1;
		case JAGP_SYM_L_NE : return 1;
		case JAGP_SYM_L_LT : return 1;
		case JAGP_SYM_L_GT : return 1;
		case JAGP_SYM_L_LE : return 1;
		case JAGP_SYM_L_GE : return 1;

		/* literals */
		case JAGP_SYM_LITERAL : return 5;
//		case JAGP_SYM_OCTAL : return 4;
//		case JAGP_SYM_DECIMAL : return 4;
//		case JAGP_SYM_STRING : return 5;
//		case JAGP_SYM_HEX : return 4;
		case JAGP_SYM_IDENTIFIER : return 1;

		/* unary operators */
		case JAGP_SYM_L_QUESTION_MARK : return 7;
		case JAGP_SYM_L_INC : return 7;
		case JAGP_SYM_L_DEC : return 7;
		case JAGP_SYM_L_NEG : return 7;
		case JAGP_SYM_L_NOT : return 7;

		case JAGP_SYM_L_MUL : return 7;
		case JAGP_SYM_L_DIV : return 7;
		case JAGP_SYM_L_MOD : return 7;
		case JAGP_SYM_L_ADD : return 7;
		case JAGP_SYM_L_SUB : return 7;

		case JAGP_SYM_L_BAR : return 6;
		case JAGP_SYM_L_AND : return 6;
		case JAGP_SYM_L_LOR : return 6;
		case JAGP_SYM_L_LAND : return 6;
		case JAGP_SYM_L_XOR : return 6;


//		case JAGP_SYM_L_DIM : return 6;

		case JAGP_SYM_L_LCUBRACE : return 9;
		case JAGP_SYM_L_RCUBRACE : return 9;
		case JAGP_SYM_L_LPAREN : return 9;
		case JAGP_SYM_L_RPAREN : return 9;
		case JAGP_SYM_L_LSQBRACE : return 4;
		case JAGP_SYM_L_RSQBRACE : return 4;


		case JAGP_SYM_L_MUL_IS : return 2;
		case JAGP_SYM_L_DIV_IS : return 2;
		case JAGP_SYM_L_MOD_IS : return 2;
		case JAGP_SYM_L_ADD_IS : return 2;
		case JAGP_SYM_L_SUB_IS : return 1;
		case JAGP_SYM_L_SHL_IS : return 2;
		case JAGP_SYM_L_SHR_IS : return 2;
		case JAGP_SYM_L_AND_IS : return 2;
		case JAGP_SYM_L_XOR_IS : return 2;
		case JAGP_SYM_L_OR_IS : return 2;



		case JAGP_SYM_L_SHRR_IS : return 1;
		case JAGP_SYM_EOL_COMMENT : return 15;
		case JAGP_SYM_FULL_COMMENT : return 3;

		case JAGP_SYM_K_PACKAGE : return 8;
		case JAGP_SYM_K_IMPORT : return 8;
		case JAGP_SYM_K_IMPLEMENTS : return 8;
		case JAGP_SYM_K_INSTANCEOF : return 8;
		case JAGP_SYM_K_EXTENDS : return 8;

		case JAGP_SYM_K_PUBLIC : return 4;
		case JAGP_SYM_K_PRIVATE : return 4;
		case JAGP_SYM_K_PROTECTED : return 4;
		case JAGP_SYM_K_STATIC : return 4;
		case JAGP_SYM_K_ABSTRACT : return 5;
		case JAGP_SYM_K_TRANSIENT : return 5;
		case JAGP_SYM_K_FINAL : return 5;
		case JAGP_SYM_K_VOLATILE : return 5;
		case JAGP_SYM_K_NATIVE : return 5;

		case JAGP_SYM_K_DO : return 9;
		case JAGP_SYM_K_TRY : return 9;
		case JAGP_SYM_K_THROW : return 9;
		case JAGP_SYM_K_THROWS : return 9;
		case JAGP_SYM_K_FINALLY : return 9;
		case JAGP_SYM_K_BREAK : return 9;
		case JAGP_SYM_K_ELSE : return 9;
//		case JAGP_SYM_K_INNER : return 9;
		case JAGP_SYM_K_CASE : return 9;
//		case JAGP_SYM_K_GOTO : return 9;
		case JAGP_SYM_K_RETURN : return 9;
		case JAGP_SYM_K_CATCH : return 9;
		case JAGP_SYM_K_NEW : return 9;
		case JAGP_SYM_K_FOR : return 9;
		case JAGP_SYM_K_SWITCH : return 9;
		case JAGP_SYM_K_CONTINUE : return 9;
		case JAGP_SYM_K_WHILE : return 9;
		case JAGP_SYM_K_SYNCHRONIZED : return 9;
		case JAGP_SYM_K_DEFAULT : return 9;
		case JAGP_SYM_K_IF : return 9;

		case JAGP_SYM_K_BOOLEAN : return 12;
		case JAGP_SYM_K_DOUBLE : return 12;
		case JAGP_SYM_K_BYTE : return 12;
		case JAGP_SYM_K_INT : return 12;
		case JAGP_SYM_K_VOID : return 12;
		case JAGP_SYM_K_FLOAT : return 12;
		case JAGP_SYM_K_LONG : return 12;
		case JAGP_SYM_K_SHORT : return 12;
		case JAGP_SYM_K_CHAR : return 1;

//		case JAGP_SYM_K_REST : return 15;
//		case JAGP_SYM_K_VAR : return 15;
//		case JAGP_SYM_K_CAST : return 15;
//		case JAGP_SYM_K_FUTURE : return 15;
//		case JAGP_SYM_K_CONST : return 15;
//		case JAGP_SYM_K_GENERIC : return 15;

		case JAGP_SYM_K_INTERFACE : return 10;
		case JAGP_SYM_K_CLASS : return 10;

		case JAGP_SYM_K_SUPER : return 8;
//		case JAGP_SYM_NULL : return 8;
		case JAGP_SYM_K_THIS : return 8;


		default : {} break;
	}
	return 1;
}



static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {
	JagAugmentRequestPrivate *priv = jag_augment_request_get_instance_private((JagAugmentRequest *) request);
	ChaRevisionReader *revision_reader = cha_revision_reader_new(a_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);

	JagPScanner *scanner = jagp_scanner_factory_create_scanner(priv->scanner_factory, utf8_scanner);
	JagPParser *parser = jagp_parser_new(scanner, priv->token_factory, priv->model);
	jagp_parser_run(parser);





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

	CatArrayWo *token_cash = jagp_parser_get_token_cash(parser);
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
		if (sym_index==JAGP_SYM_EOL_COMMENT) {
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


//
//
//    JagScanner *scanner = jag_scanner_new(utf8_scanner);
//
//    JagPlainParser *plain_parser = jag_plain_parser_new(JAG_ISCANNER(scanner), keyword_printer, line_tag_printer);
//    jag_plain_parser_run(plain_parser);
//
//	cat_unref_ptr(utf8_scanner);
//	cat_unref_ptr(plain_parser);
//	cat_unref_ptr(scanner);
	return TRUE;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagAugmentRequest *instance = JAG_AUGMENT_REQUEST(self);
	JagAugmentRequestPrivate *priv = jag_augment_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

