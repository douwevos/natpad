/*
   File:    armaugmentrequest.c
   Project: armadillo
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

#include "armaugmentrequest.h"
#include "../parser/armpparser.h"
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ArmAugmentRequest"
#include <logging/catlog.h>

struct _ArmAugmentRequestPrivate {
	GroRunModel *model;
	ArmPScannerFactory *scanner_factory;
	GroRunITokenFactory *token_factory;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ArmAugmentRequest, arm_augment_request, DRA_TYPE_AUGMENT_REQUEST,
		G_ADD_PRIVATE(ArmAugmentRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);

static void arm_augment_request_class_init(ArmAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAugmentRequestClass *aug_class = DRA_AUGMENT_REQUEST_CLASS(clazz);
	aug_class->runAugment = l_run_augment;
}

static void arm_augment_request_init(ArmAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ArmAugmentRequest *instance = ARM_AUGMENT_REQUEST(object);
	ArmAugmentRequestPrivate *priv = arm_augment_request_get_instance_private(instance);
	G_OBJECT_CLASS(arm_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(arm_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ArmAugmentRequest *arm_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key,
		GroRunModel *model, GroRunITokenFactory *token_factory, ArmPScannerFactory *scanner_factory) {
	ArmAugmentRequest *result = g_object_new(ARM_TYPE_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	ArmAugmentRequestPrivate *priv = arm_augment_request_get_instance_private(result);
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
		case ARMP_SYM_ELLIPSIS :
		case ARMP_SYM_PTR_OP :
		case ARMP_SYM_L_COMMA :
		case ARMP_SYM_L_DOT :
		case ARMP_SYM_L_COLON :
		case ARMP_SYM_L_SEMI : return 3;

		/* conditions */
		case ARMP_SYM_L_IS :
		case ARMP_SYM_EQ_OP :
		case ARMP_SYM_NE_OP :
		case ARMP_SYM_L_LT :
		case ARMP_SYM_L_GT :
		case ARMP_SYM_LE_OP :
		case ARMP_SYM_GE_OP : return 1;

		/* unary operators */
		case ARMP_SYM_L_QUESTIONMARK :
		case ARMP_SYM_INC_OP :
		case ARMP_SYM_DEC_OP :
		case ARMP_SYM_L_NEG :
		case ARMP_SYM_L_NOT : return 7;

		/* infix operators */
		case ARMP_SYM_LEFT_OP :
		case ARMP_SYM_RIGHT_OP :
		case ARMP_SYM_L_MUL :
		case ARMP_SYM_L_DIV :
		case ARMP_SYM_L_MOD :
		case ARMP_SYM_L_PLUS :
		case ARMP_SYM_L_MINUS : return 7;

		/* infix logical and bitwise operators */
		case ARMP_SYM_L_BAR :
		case ARMP_SYM_L_AND :
		case ARMP_SYM_L_XOR :
		case ARMP_SYM_AND_OP :
		case ARMP_SYM_OR_OP : return 6;

		/* assignments */
		case ARMP_SYM_MUL_ASSIGN :
		case ARMP_SYM_DIV_ASSIGN :
		case ARMP_SYM_MOD_ASSIGN :
		case ARMP_SYM_ADD_ASSIGN :
		case ARMP_SYM_SUB_ASSIGN :
		case ARMP_SYM_LEFT_ASSIGN :
		case ARMP_SYM_RIGHT_ASSIGN :
		case ARMP_SYM_AND_ASSIGN :
		case ARMP_SYM_XOR_ASSIGN :
		case ARMP_SYM_OR_ASSIGN : return 2;

		/* literals */
		case ARMP_SYM_IDENTIFIER : return 1;
		case ARMP_SYM_I_CONSTANT :
		case ARMP_SYM_F_CONSTANT :
		case ARMP_SYM_STRING_LITERAL : return 5;

		/* braces */
		case ARMP_SYM_L_LCUBRACE :
		case ARMP_SYM_L_RCUBRACE : return 9;
		case ARMP_SYM_L_LSQBRACE :
		case ARMP_SYM_L_RSQBRACE : return 4;
		case ARMP_SYM_L_LPAREN :
		case ARMP_SYM_L_RPAREN : return 9;

		/* comments */
		case ARMP_SYM_EOL_COMMENT : return 15;
		case ARMP_SYM_FULL_COMMENT : return 3;

		/* primitive types */
		case ARMP_SYM_BOOL :
		case ARMP_SYM_CHAR :
		case ARMP_SYM_SHORT :
		case ARMP_SYM_INT :
		case ARMP_SYM_LONG :
		case ARMP_SYM_SIGNED :
		case ARMP_SYM_UNSIGNED :
		case ARMP_SYM_FLOAT :
		case ARMP_SYM_DOUBLE :
		case ARMP_SYM_VOID : return 12;

		/* statement basics */
		case ARMP_SYM_IF :
		case ARMP_SYM_ELSE :
		case ARMP_SYM_SWITCH :
		case ARMP_SYM_WHILE :
		case ARMP_SYM_DO :
		case ARMP_SYM_FOR : return 9;

		/* jumps */
		case ARMP_SYM_CASE :
		case ARMP_SYM_DEFAULT :
		case ARMP_SYM_GOTO :
		case ARMP_SYM_CONTINUE :
		case ARMP_SYM_BREAK :
		case ARMP_SYM_RETURN : return 9;

		/* modifiers and type constructors */
		case ARMP_SYM_SIZEOF :
		case ARMP_SYM_TYPEDEF :
		case ARMP_SYM_EXTERN :
		case ARMP_SYM_STATIC :
		case ARMP_SYM_REGISTER :
		case ARMP_SYM_INLINE :
		case ARMP_SYM_CONST :
		case ARMP_SYM_VOLATILE :
		case ARMP_SYM_STRUCT :
		case ARMP_SYM_UNION :
		case ARMP_SYM_ENUM : return 4 ;

		/* special */
		case ARMP_SYM_FUNC_NAME :
		case ARMP_SYM_TYPEDEF_NAME :
		case ARMP_SYM_ENUMERATION_CONSTANT :

		case ARMP_SYM_AUTO :
		case ARMP_SYM_RESTRICT :
		case ARMP_SYM_COMPLEX :
		case ARMP_SYM_IMAGINARY :
		case ARMP_SYM_ALIGNAS :
		case ARMP_SYM_ALIGNOF :
		case ARMP_SYM_ATOMIC :
		case ARMP_SYM_GENERIC :
		case ARMP_SYM_NORETURN :
		case ARMP_SYM_STATIC_ASSERT :
		case ARMP_SYM_THREAD_LOCAL : return 4 ;

		default : {} break;
	}
	return 1;
}



static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {
	ArmAugmentRequestPrivate *priv = arm_augment_request_get_instance_private((ArmAugmentRequest *) request);
	ChaRevisionReader *revision_reader = cha_revision_reader_new(a_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);

	ArmPScanner *scanner = armp_scanner_factory_create_scanner(priv->scanner_factory, utf8_scanner);
	ArmPParser *parser = armp_parser_new(scanner, priv->token_factory, priv->model);
	armp_parser_run(parser);


	/* tag lines */
	CatArrayWo *message_list = armp_parser_get_messages(parser);
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

	CatArrayWo *token_cash = armp_parser_get_token_cash(parser);
	CatIIterator *iter = cat_array_wo_iterator(token_cash);
	while(cat_iiterator_has_next(iter)) {
		GroRunIToken *token = (GroRunIToken *) cat_iiterator_next(iter);

		cat_log_debug("token=%O", token);

		GroRunLocation *location = grorun_itoken_get_location(token);

		int left_column, right_column;
		long left_row, right_row;
		grorun_location_get_all(location, &left_column, &left_row, &right_column, &right_row);

		dra_keyword_printer_print_fg_color(keyword_printer, left_row, left_column, right_row, right_column, l_map_symbol(token));
		int sym_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
		if (sym_index==ARMP_SYM_EOL_COMMENT) {
			GObject *val = grorun_itoken_get_value(token);
			if (CAT_IS_STRING_WO(val)) {
				dra_spell_helper_scan(spell_helper, line_tag_printer, (CatStringWo *) val, left_column, left_row);
			}
		}
	}
	cat_unref_ptr(iter);

	return TRUE;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ArmAugmentRequest *instance = ARM_AUGMENT_REQUEST(self);
	ArmAugmentRequestPrivate *priv = arm_augment_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

