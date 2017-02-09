/*
   File:    jagplainparser.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Dec 18, 2011
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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

#include "jagplainparser.h"
#include "jagsymbolext.h"
#include "generated/jagparser.h"
#include "generated/runtime/jagparserbase.h"
#include "generated/runtime/jagtoken.h"
#include "generated/runtime/jagparsercontext.h"
#include <caterpillar.h>
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPlainParser"
#include <logging/catlog.h>

struct _JagPlainParserPrivate {
	JagIScanner *delagate;
	DraKeywordPrinter *keyword_printer;
	DraKeywordPrinter *line_tag_printer;
	JagToken **token_cash;
	int token_cash_out;
	int token_cash_length;
	int token_cash_size;
	DraSpellHelper *spell_helper;

};

static void l_scanner_iface_init(JagIScannerInterface *iface);


G_DEFINE_TYPE_WITH_CODE(JagPlainParser, jag_plain_parser, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_TYPE_ISCANNER, l_scanner_iface_init);
});

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_plain_parser_class_init(JagPlainParserClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagPlainParserPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void jag_plain_parser_init(JagPlainParser *instance) {
	JagPlainParserPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_PLAIN_PARSER, JagPlainParserPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPlainParser *instance = JAG_PLAIN_PARSER(object);
	JagPlainParserPrivate *priv = instance->priv;
	cat_unref_ptr(priv->keyword_printer);
	cat_unref_ptr(priv->line_tag_printer);
	cat_unref_ptr(priv->delagate);
	cat_unref_ptr(priv->spell_helper);
	cat_free_ptr(priv->token_cash);
	priv->token_cash_size = 0;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPlainParser *jag_plain_parser_new(JagIScanner *delagate, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer) {
	JagPlainParser *result = g_object_new(JAG_TYPE_PLAIN_PARSER, NULL);
	cat_ref_anounce(result);
	JagPlainParserPrivate *priv = result->priv;
	priv->delagate = cat_ref_ptr(delagate);
	priv->keyword_printer = cat_ref_ptr(token_printer);
	priv->line_tag_printer = cat_ref_ptr(line_tag_printer);
	priv->token_cash_size = 32;
	priv->token_cash = (JagToken **) g_malloc(sizeof(JagToken *)*priv->token_cash_size);
	priv->token_cash_out = 0;
	priv->token_cash_length = 0;
	priv->spell_helper = dra_spell_helper_new();
	return result;
}


static void l_print_token(JagPlainParser *parser, JagToken *token);


void jag_plain_parser_run(JagPlainParser *parser) {
	JagPlainParserPrivate *priv = JAG_PLAIN_PARSER_GET_PRIVATE(parser);

	priv->token_cash_out = 0;
	priv->token_cash_length = 0;

	JagIScanner *scanner = JAG_ISCANNER(parser);
	JagParserContext *context = jag_parser_context_new(scanner);
	JagParser *cupParser = jag_parser_new(scanner);
	JagParserBase *cupBase = JAG_PARSER_BASE(cupParser);
	jag_parser_base_parse(cupBase, context);


	while(priv->token_cash_length>0) {
		int idx = priv->token_cash_out % priv->token_cash_size;
		JagToken *out_token = priv->token_cash[idx];
		if (out_token) {
			priv->token_cash_out++;
			priv->token_cash_length--;
			l_print_token(parser, out_token);
			cat_unref_ptr(priv->token_cash[idx]);
		}
	}


	while(TRUE) {
		JagToken *token = jag_iscanner_next_token(priv->delagate);

		if (token) {
			if (token->sym==JAG_SYMBOL_TERM_EOF) {
				cat_unref_ptr(token);
				break;
			}
			l_print_token(parser, token);
			cat_unref_ptr(token);
		} else {
			break;
		}
	}


	cat_unref_ptr(cupParser);
	cat_unref_ptr(context);
}





static int l_map_symbol(JagToken *token) {
	switch(token->sym) {
		case JAG_SYMBOL_TERM_COLON : return 3;
		case JAG_SYMBOL_TERM_DOT : return 3;
		case JAG_SYMBOL_TERM_COMMA : return 3;
		case JAG_SYMBOL_TERM_SEMICOLON: return 3;

		case JAG_SYMBOL_TERM_ASS_IS : return 1;

		/* conditions */
		case JAG_SYMBOL_TERM_OP_EQ : return 1;
		case JAG_SYMBOL_TERM_OP_NE : return 1;
		case JAG_SYMBOL_TERM_OP_LT : return 1;
		case JAG_SYMBOL_TERM_OP_GT : return 1;
		case JAG_SYMBOL_TERM_OP_LE : return 1;
		case JAG_SYMBOL_TERM_OP_GE : return 1;

		/* literals */
		case JAG_SYMBOL_TERM_STRING_LITERAL : return 5;
//		case JAG_SYMBOL_TERM_OCTAL : return 4;
//		case JAG_SYMBOL_TERM_DECIMAL : return 4;
//		case JAG_SYMBOL_TERM_STRING : return 5;
//		case JAG_SYMBOL_TERM_HEX : return 4;
		case JAG_SYMBOL_TERM_IDENTIFIER : return 1;

		/* unary operators */
		case JAG_SYMBOL_TERM_QUESTION : return 7;
		case JAG_SYMBOL_TERM_OP_INC : return 7;
		case JAG_SYMBOL_TERM_OP_DEC : return 7;
		case JAG_SYMBOL_TERM_OP_NEG : return 7;
		case JAG_SYMBOL_TERM_OP_NOT : return 7;

		case JAG_SYMBOL_TERM_OP_MUL : return 7;
		case JAG_SYMBOL_TERM_OP_DIV : return 7;
		case JAG_SYMBOL_TERM_OP_MOD : return 7;
		case JAG_SYMBOL_TERM_OP_ADD : return 7;
		case JAG_SYMBOL_TERM_OP_SUB : return 7;

		case JAG_SYMBOL_TERM_OP_OR : return 6;
		case JAG_SYMBOL_TERM_OP_AND : return 6;
		case JAG_SYMBOL_TERM_OP_LOR : return 6;
		case JAG_SYMBOL_TERM_OP_LAND : return 6;
		case JAG_SYMBOL_TERM_OP_XOR : return 6;

		case JAG_SYMBOL_TERM_OP_SHRR : return 6;
		case JAG_SYMBOL_TERM_OP_SHL : return 6;
		case JAG_SYMBOL_TERM_OP_SHR : return 6;

//		case JAG_SYMBOL_TERM_OP_DIM : return 6;

		case JAG_SYMBOL_TERM_LBRACE : return 9;
		case JAG_SYMBOL_TERM_RBRACE : return 9;
		case JAG_SYMBOL_TERM_LPAREN : return 9;
		case JAG_SYMBOL_TERM_RPAREN : return 9;
		case JAG_SYMBOL_TERM_LBRACK : return 4;
		case JAG_SYMBOL_TERM_RBRACK : return 4;


		case JAG_SYMBOL_TERM_ASS_MUL : return 2;
		case JAG_SYMBOL_TERM_ASS_DIV : return 2;
		case JAG_SYMBOL_TERM_ASS_MOD : return 2;
		case JAG_SYMBOL_TERM_ASS_ADD : return 2;
		case JAG_SYMBOL_TERM_ASS_SUB : return 1;
		case JAG_SYMBOL_TERM_ASS_SHL : return 2;
		case JAG_SYMBOL_TERM_ASS_SHR : return 2;
		case JAG_SYMBOL_TERM_ASS_AND : return 2;
		case JAG_SYMBOL_TERM_ASS_XOR : return 2;
		case JAG_SYMBOL_TERM_ASS_OR : return 2;



		case JAG_SYMBOL_TERM_ASS_SHRR : return 1;
		case JAG_SYMBOL_TERM_EOL_COMMENT : return 15;
		case JAG_SYMBOL_TERM_FULL_COMMENT : return 3;

		case JAG_SYMBOL_TERM_PACKAGE : return 8;
		case JAG_SYMBOL_TERM_IMPORT : return 8;
		case JAG_SYMBOL_TERM_IMPLEMENTS : return 8;
		case JAG_SYMBOL_TERM_INSTANCEOF : return 8;
		case JAG_SYMBOL_TERM_EXTENDS : return 8;

		case JAG_SYMBOL_TERM_BOOLEAN_LITERAL : return 14;

		case JAG_SYMBOL_TERM_PUBLIC : return 4;
		case JAG_SYMBOL_TERM_PRIVATE : return 4;
		case JAG_SYMBOL_TERM_PROTECTED : return 4;
		case JAG_SYMBOL_TERM_STATIC : return 4;
		case JAG_SYMBOL_TERM_ABSTRACT : return 5;
		case JAG_SYMBOL_TERM_TRANSIENT : return 5;
		case JAG_SYMBOL_TERM_FINAL : return 5;
		case JAG_SYMBOL_TERM_VOLATILE : return 5;
		case JAG_SYMBOL_TERM_NATIVE : return 5;

		case JAG_SYMBOL_TERM_DO : return 9;
		case JAG_SYMBOL_TERM_TRY : return 9;
		case JAG_SYMBOL_TERM_THROW : return 9;
		case JAG_SYMBOL_TERM_THROWS : return 9;
		case JAG_SYMBOL_TERM_FINALLY : return 9;
		case JAG_SYMBOL_TERM_BREAK : return 9;
		case JAG_SYMBOL_TERM_ELSE : return 9;
//		case JAG_SYMBOL_TERM_INNER : return 9;
		case JAG_SYMBOL_TERM_CASE : return 9;
		case JAG_SYMBOL_TERM_GOTO : return 9;
		case JAG_SYMBOL_TERM_RETURN : return 9;
		case JAG_SYMBOL_TERM_CATCH : return 9;
		case JAG_SYMBOL_TERM_NEW : return 9;
		case JAG_SYMBOL_TERM_FOR : return 9;
		case JAG_SYMBOL_TERM_SWITCH : return 9;
		case JAG_SYMBOL_TERM_CONTINUE : return 9;
		case JAG_SYMBOL_TERM_WHILE : return 9;
		case JAG_SYMBOL_TERM_SYNCHRONIZED : return 9;
		case JAG_SYMBOL_TERM_DEFAULT : return 9;
		case JAG_SYMBOL_TERM_IF : return 9;

		case JAG_SYMBOL_TERM_BOOLEAN : return 12;
		case JAG_SYMBOL_TERM_DOUBLE : return 12;
		case JAG_SYMBOL_TERM_BYTE : return 12;
		case JAG_SYMBOL_TERM_INT : return 12;
		case JAG_SYMBOL_TERM_VOID : return 12;
		case JAG_SYMBOL_TERM_FLOAT : return 12;
		case JAG_SYMBOL_TERM_LONG : return 12;
		case JAG_SYMBOL_TERM_SHORT : return 12;
		case JAG_SYMBOL_TERM_CHAR : return 1;

//		case JAG_SYMBOL_TERM_REST : return 15;
//		case JAG_SYMBOL_TERM_VAR : return 15;
//		case JAG_SYMBOL_TERM_CAST : return 15;
//		case JAG_SYMBOL_TERM_FUTURE : return 15;
		case JAG_SYMBOL_TERM_CONST : return 15;
//		case JAG_SYMBOL_TERM_GENERIC : return 15;

		case JAG_SYMBOL_TERM_INTERFACE : return 10;
		case JAG_SYMBOL_TERM_CLASS : return 10;

		case JAG_SYMBOL_TERM_SUPER : return 8;
		case JAG_SYMBOL_TERM_NULL : return 8;
		case JAG_SYMBOL_TERM_THIS : return 8;

//		case JAG_SYMBOL_TERM_OPERATOR : return 1;
//		case JAG_SYMBOL_TERM_OUTER : return 1;

		default : {} break;
	}
	return 1;
}




static void l_print_token(JagPlainParser *parser, JagToken *token) {
	JagPlainParserPrivate *priv = JAG_PLAIN_PARSER_GET_PRIVATE(parser);
//
////	AstToken *astToken = ast_token_new_full(token->row, token->left, token->right, l_map_symbol(token), 0);
//	AstToken *astToken = ast_token_new_full(token->left_row, token->left, token->right, l_map_symbol(token), 0);
//	if (token->is_error) {
//		astToken->error = 3;
//	}
//	cat_log_on_debug({
//		ast_token_dump(astToken);
//	});
//	ast_token_print(priv->token_printer, astToken);


	dra_keyword_printer_print_fg_color(priv->keyword_printer, token->left_row, token->left, token->right_row, token->right, l_map_symbol(token));

	if (token->is_error) {
		cat_log_debug("error !!! token->left=%d, token->right=%d", token->left, token->right);
		DraLineTagWo *line_tag = dra_line_tag_wo_new(token->left_row, DRA_TAG_TYPE_PARSER_ERROR);
		dra_line_tag_wo_set_color(line_tag, 1,0,0);
		dra_line_tag_wo_set_start_and_end_index(line_tag, token->left, token->right);
		dra_keyword_printer_print_line_tag(priv->line_tag_printer, line_tag);
	}

	if (token->sym == JAG_SYMBOL_TERM_FULL_COMMENT) {
		CatStringWo *text = CAT_STRING_WO(token->value);
		cat_log_trace("text=%O", text);
		if (text) {
			dra_spell_helper_scan(priv->spell_helper, priv->line_tag_printer, text, token->left, token->left_row);
		}
	}
}




static JagToken *l_scanner_next_token(JagIScanner *self) {
	JagPlainParser *parser = JAG_PLAIN_PARSER(self);
	JagPlainParserPrivate *priv = JAG_PLAIN_PARSER_GET_PRIVATE(parser);

	JagToken *token = NULL;
	while(TRUE) {
		token = jag_iscanner_next_token(priv->delagate);

		if (token!=NULL) {
			cat_log_debug("token=%d", token->sym);
			if (token->sym==JAG_SYMBOL_TERM_EOF) {
				return token;
			}

			if (priv->token_cash_length+1>=priv->token_cash_size) {
				int idx = priv->token_cash_out % priv->token_cash_size;
				JagToken *out_token = priv->token_cash[idx];
				if (out_token) {
					priv->token_cash_out++;
					priv->token_cash_length--;
					l_print_token(parser, out_token);
					cat_unref_ptr(priv->token_cash[idx]);
				}
			}

			int idx = (priv->token_cash_out+priv->token_cash_length) % priv->token_cash_size;
			priv->token_cash[idx] = cat_ref_ptr(token);
			priv->token_cash_length++;

			if (token->sym == JAG_SYMBOL_TERM_FULL_COMMENT || token->sym == JAG_SYMBOL_TERM_EOL_COMMENT) {
				cat_unref_ptr(token);
				continue;
			}
		}
		break;
	}
	return token;
}

static void l_scanner_iface_init(JagIScannerInterface *iface) {
	iface->next_token = l_scanner_next_token;
}

