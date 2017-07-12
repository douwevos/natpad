/*
   File:    jagplexerimpl.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 31, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "jagplexerimpl.h"
#include "jagpilexer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPLexerImpl"
#include <logging/catlog.h>

struct _JagPLexerImplPrivate {
	JagPTokenizer *tokenizer;
	CatArrayWo *tokens;
	CatArrayWo *parser_tokens;
	JagPCursor *error_pos;
	JagPToken *eof_token;
	int tokens_index;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_lexer_iface_init(JagPILexerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPLexerImpl, jagp_lexer_impl, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPLexerImpl)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(JAGP_TYPE_ILEXER, l_lexer_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_lexer_impl_class_init(JagPLexerImplClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_lexer_impl_init(JagPLexerImpl *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPLexerImpl *instance = JAGP_LEXER_IMPL(object);
	JagPLexerImplPrivate *priv = jagp_lexer_impl_get_instance_private(instance);
	cat_unref_ptr(priv->tokenizer);
	cat_unref_ptr(priv->tokens);
	cat_unref_ptr(priv->parser_tokens);
	cat_unref_ptr(priv->error_pos);
	G_OBJECT_CLASS(jagp_lexer_impl_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_lexer_impl_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static JagPToken *l_scan_real_token(JagPLexerImpl *lexer);
static void l_next_token(JagPILexer *lexer);


JagPLexerImpl *jagp_lexer_impl_new(JagPTokenizer *tokenizer) {
	JagPLexerImpl *result = g_object_new(JAGP_TYPE_LEXER_IMPL, NULL);
	cat_ref_anounce(result);
	JagPLexerImplPrivate *priv = jagp_lexer_impl_get_instance_private(result);
	priv->tokenizer = cat_ref_ptr(tokenizer);
	priv->tokens_index = -1;
	priv->tokens = cat_array_wo_new();
	priv->parser_tokens = cat_array_wo_new();
	priv->error_pos = NULL;
	priv->eof_token = jagp_token_new();
	priv->eof_token->kind = JAGP_KIND_EOF;
	priv->eof_token->cur_start =jagp_cursor_new(-1, -1);
	priv->eof_token->cur_end =jagp_cursor_new(-1, -1);
	return result;
}

CatArrayWo *jagp_lexer_impl_get_all_tokens(JagPLexerImpl *lexer) {
	JagPLexerImplPrivate *priv = jagp_lexer_impl_get_instance_private(lexer);
	return priv->tokens;
}


static JagPToken *l_scan_real_token(JagPLexerImpl *lexer) {
	JagPLexerImplPrivate *priv = jagp_lexer_impl_get_instance_private(lexer);
	
	JagPToken *token = jagp_tokenizer_next(priv->tokenizer);
	cat_log_debug("token=%O", token);
	if (token==NULL) {
		token = cat_ref_ptr(priv->eof_token);
	}
	cat_array_wo_append(priv->tokens, (GObject *) token);
	if (token->kind!=JAGP_KIND_COMMENT_EOL && token->kind!=JAGP_KIND_COMMENT_FULL) {
		cat_array_wo_append(priv->parser_tokens, (GObject *) token);
	}
	cat_unref(token);
	return token;

//
//	GroRunFullToken *full_token = (GroRunFullToken *) grorun_iscanner_next((GroRunIScanner *) priv->scanner);
//	cat_log_detail("full_token=%O", full_token);
//	JagPToken *token = jagp_token_new();
//	token->source = full_token;
//	int sym_index = grorun_full_token_get_user_index(full_token);
//	GroRunLocation *location = grorun_full_token_get_location(full_token);
//
//
//
//	switch(sym_index) {
//
//		// 0
//		case JAGP_SYM_END_OF_INPUT : token->kind = JAGP_KIND_EOF;  break;
////		case JAGP_SYM_ERROR : token->kind = JAGP_KIND_; break;
//		case JAGP_SYM_L_LSQBRACE : token->kind = JAGP_KIND_LBRACKET; break;
//		case JAGP_SYM_L_RSQBRACE : token->kind = JAGP_KIND_RBRACKET; break;
//		case JAGP_SYM_L_ARROW : token->kind = JAGP_KIND_ARROW; break;
//		case JAGP_SYM_L_AND : token->kind = JAGP_KIND_AMP; break;
//		case JAGP_SYM_L_LT : token->kind = JAGP_KIND_LT; break;
//		case JAGP_SYM_L_GT : token->kind = JAGP_KIND_GT; break;
//		case JAGP_SYM_L_LE : token->kind = JAGP_KIND_LTEQ; break;
//		case JAGP_SYM_L_GE : token->kind = JAGP_KIND_GTEQ; break;
//
//
//		// 10
//		case JAGP_SYM_L_EQ : token->kind = JAGP_KIND_EQEQ; break;
//		case JAGP_SYM_L_NE : token->kind = JAGP_KIND_BANGEQ; break;
//		case JAGP_SYM_L_COMMA : token->kind = JAGP_KIND_COMMA; break;
//		case JAGP_SYM_L_QUESTION_MARK : token->kind = JAGP_KIND_QUES; break;
//		case JAGP_SYM_L_DOT : token->kind = JAGP_KIND_DOT; break;
//		case JAGP_SYM_L_XOR : token->kind = JAGP_KIND_CARET; break;
//		case JAGP_SYM_L_MUL : token->kind = JAGP_KIND_STAR; break;
//		case JAGP_SYM_L_DIV : token->kind = JAGP_KIND_SLASH; break;
//		case JAGP_SYM_L_MOD : token->kind = JAGP_KIND_PERCENT; break;
//		case JAGP_SYM_L_SUB : token->kind = JAGP_KIND_SUB; break;
//
//		// 20
//		case JAGP_SYM_L_ADD : token->kind = JAGP_KIND_PLUS; break;
//		case JAGP_SYM_L_IS : token->kind = JAGP_KIND_EQ; break;
//		case JAGP_SYM_L_NOT : token->kind = JAGP_KIND_BANG; break;
//		case JAGP_SYM_L_NEG : token->kind = JAGP_KIND_TILDE; break;
//		case JAGP_SYM_L_LPAREN : token->kind = JAGP_KIND_LPAREN; break;
//		case JAGP_SYM_L_RPAREN : token->kind = JAGP_KIND_RPAREN; break;
//		case JAGP_SYM_L_LCUBRACE : token->kind = JAGP_KIND_LBRACE; break;
//		case JAGP_SYM_L_RCUBRACE : token->kind = JAGP_KIND_RBRACE; break;
//		case JAGP_SYM_L_VARARGS: token->kind = JAGP_KIND_ELLIPSIS; break;
//		case JAGP_SYM_L_AT : token->kind = JAGP_KIND_MONKEYS_AT; break;
//
//		// 30
//		case JAGP_SYM_L_INC : token->kind = JAGP_KIND_PLUSPLUS; break;
//		case JAGP_SYM_L_DEC : token->kind = JAGP_KIND_SUBSUB; break;
//		case JAGP_SYM_L_COLON : token->kind = JAGP_KIND_COLON; break;
//		case JAGP_SYM_L_SEMI : token->kind = JAGP_KIND_SEMI; break;
//		case JAGP_SYM_L_COLON_COLON : token->kind = JAGP_KIND_COLCOL; break;
//		case JAGP_SYM_L_BAR : token->kind = JAGP_KIND_BAR; break;
//		case JAGP_SYM_L_LOR : token->kind = JAGP_KIND_BARBAR; break;
//		case JAGP_SYM_L_LAND : token->kind = JAGP_KIND_AMPAMP; break;
//		case JAGP_SYM_L_MUL_IS : token->kind = JAGP_KIND_STAREQ; break;
//		case JAGP_SYM_L_DIV_IS : token->kind = JAGP_KIND_SLASHEQ; break;
//
//		//40
//		case JAGP_SYM_L_MOD_IS: token->kind = JAGP_KIND_PERCENTEQ; break;
//		case JAGP_SYM_L_ADD_IS: token->kind = JAGP_KIND_PLUSEQ; break;
//		case JAGP_SYM_L_SUB_IS: token->kind = JAGP_KIND_SUBEQ; break;
//		case JAGP_SYM_L_SHL_IS: token->kind = JAGP_KIND_LTEQ; break;
//		case JAGP_SYM_L_SHR_IS: token->kind = JAGP_KIND_GTEQ; break;
//		case JAGP_SYM_L_SHRR_IS: token->kind = JAGP_KIND_GTGTEQ; break;
//		case JAGP_SYM_L_AND_IS: token->kind = JAGP_KIND_AMPEQ; break;
//		case JAGP_SYM_L_XOR_IS: token->kind = JAGP_KIND_CARETEQ; break;
//		case JAGP_SYM_L_OR_IS: token->kind = JAGP_KIND_BAREQ; break;
//		case JAGP_SYM_K_BOOLEAN: token->kind = JAGP_KIND_BOOLEAN; break;
//
//
//		// 50
//		case JAGP_SYM_K_BYTE: token->kind = JAGP_KIND_BYTE; break;
//		case JAGP_SYM_K_SHORT: token->kind = JAGP_KIND_SHORT; break;
//		case JAGP_SYM_K_INT: token->kind = JAGP_KIND_INT; break;
//		case JAGP_SYM_K_LONG: token->kind = JAGP_KIND_LONG; break;
//		case JAGP_SYM_K_CHAR: token->kind = JAGP_KIND_CHAR; break;
//		case JAGP_SYM_K_DOUBLE: token->kind = JAGP_KIND_DOUBLE; break;
//		case JAGP_SYM_K_FLOAT: token->kind = JAGP_KIND_FLOAT; break;
//		case JAGP_SYM_IDENTIFIER : token->kind = JAGP_KIND_IDENTIFIER; token->value = (GObject *) jagp_name_new(); break;
//		case JAGP_SYM_LITERAL : token->kind = JAGP_KIND_INTLITERAL; token->value = cat_ref_ptr(grorun_itoken_get_value(full_token)); break;
//		case JAGP_SYM_K_EXTENDS : token->kind = JAGP_KIND_EXTENDS; break;
//
//		// 60
//		case JAGP_SYM_K_SUPER : token->kind = JAGP_KIND_SUPER; break;
//		case JAGP_SYM_K_THIS : token->kind = JAGP_KIND_THIS; break;
//		case JAGP_SYM_K_PACKAGE : token->kind = JAGP_KIND_PACKAGE; break;
//		case JAGP_SYM_K_PUBLIC: token->kind = JAGP_KIND_PUBLIC; break;
//		case JAGP_SYM_K_PRIVATE: token->kind = JAGP_KIND_PRIVATE; break;
//		case JAGP_SYM_K_PROTECTED: token->kind = JAGP_KIND_PROTECTED; break;
//		case JAGP_SYM_K_IMPORT: token->kind = JAGP_KIND_IMPORT; break;
//		case JAGP_SYM_K_CLASS : token->kind = JAGP_KIND_CLASS; break;
//		case JAGP_SYM_K_INTERFACE : token->kind = JAGP_KIND_INTERFACE; break;
//		case JAGP_SYM_K_ABSTRACT: token->kind = JAGP_KIND_ABSTRACT; break;
//
//		// 70
//		case JAGP_SYM_K_STATIC: token->kind = JAGP_KIND_STATIC; break;
//		case JAGP_SYM_K_FINAL : token->kind = JAGP_KIND_FINAL; break;
//		case JAGP_SYM_K_STRICTFP : token->kind = JAGP_KIND_STRICTFP; break;
//		case JAGP_SYM_K_IMPLEMENTS : token->kind = JAGP_KIND_IMPLEMENTS; break;
//		case JAGP_SYM_K_TRANSIENT: token->kind = JAGP_KIND_TRANSIENT; break;
//		case JAGP_SYM_K_VOLATILE: token->kind = JAGP_KIND_VOLATILE; break;
//		case JAGP_SYM_K_SYNCHRONIZED: token->kind = JAGP_KIND_SYNCHRONIZED; break;
//		case JAGP_SYM_K_NATIVE: token->kind = JAGP_KIND_NATIVE; break;
//		case JAGP_SYM_K_VOID: token->kind = JAGP_KIND_VOID; break;
//		case JAGP_SYM_K_THROWS : token->kind = JAGP_KIND_THROWS; break;
//
//		// 80
//		case JAGP_SYM_K_ENUM : token->kind = JAGP_KIND_ENUM; break;
//		case JAGP_SYM_K_DEFAULT : token->kind = JAGP_KIND_DEFAULT; break;
//		case JAGP_SYM_K_IF : token->kind = JAGP_KIND_IF; break;
//		case JAGP_SYM_K_ASSERT : token->kind = JAGP_KIND_ASSERT; break;
//		case JAGP_SYM_K_SWITCH : token->kind = JAGP_KIND_SWITCH; break;
//		case JAGP_SYM_K_CASE : token->kind = JAGP_KIND_CASE; break;
//		case JAGP_SYM_K_BREAK : token->kind = JAGP_KIND_BREAK; break;
//		case JAGP_SYM_K_ELSE : token->kind = JAGP_KIND_ELSE; break;
//		case JAGP_SYM_K_DO : token->kind = JAGP_KIND_DO; break;
//		case JAGP_SYM_K_WHILE : token->kind = JAGP_KIND_WHILE; break;
//
//		// 90
//		case JAGP_SYM_K_FOR: token->kind = JAGP_KIND_FOR; break;
//		case JAGP_SYM_K_NEW : token->kind = JAGP_KIND_NEW; break;
//		case JAGP_SYM_K_INSTANCEOF : token->kind = JAGP_KIND_INSTANCEOF; break;
//		case JAGP_SYM_K_TRY : token->kind = JAGP_KIND_TRY; break;
//		case JAGP_SYM_K_CATCH : token->kind = JAGP_KIND_CATCH; break;
//		case JAGP_SYM_K_FINALLY : token->kind = JAGP_KIND_FINALLY; break;
//		case JAGP_SYM_K_CONTINUE: token->kind = JAGP_KIND_CONTINUE; break;
//		case JAGP_SYM_K_THROW : token->kind = JAGP_KIND_THROW; break;
//
//		case JAGP_SYM_K_RETURN: token->kind = JAGP_KIND_RETURN; break;
//
//
//
//		case JAGP_SYM_EOL_COMMENT :
//		case JAGP_SYM_FULL_COMMENT : {
//			cat_unref(token);
//			return l_scan_real_token(lexer);
//		}
//
//		default : {
//			cat_log_error("token not defined:sym_index=%d", sym_index);
//			token->kind = JAGP_KIND_EOF;
//		}
//	}
//
//	int column;
//	long row;
//	grorun_location_get_begin(location, &column, &row);
//	token->cur_start = jagp_cursor_new(row, column);
//	grorun_location_get_end(location, &column, &row);
//	token->cur_end = jagp_cursor_new(row, column);
//
//	cat_array_wo_append(priv->tokens, (GObject *) token);
//	cat_unref(token);
//	return token;
}

/********************* start JagPILexer implementation *********************/

static void l_next_token(JagPILexer *lexer) {
	JagPLexerImplPrivate *priv = jagp_lexer_impl_get_instance_private((JagPLexerImpl *) lexer);
	JagPToken *cur_token = (JagPToken *) cat_array_wo_get(priv->parser_tokens, priv->tokens_index);
	if (cur_token!=NULL && cur_token->kind==JAGP_KIND_EOF) {
		return;
	}
	int mx = 1000;
	priv->tokens_index++;
	while(priv->tokens_index>=cat_array_wo_size(priv->parser_tokens)) {
		JagPToken *token = l_scan_real_token((JagPLexerImpl *) lexer);
		if (mx--<0) {
			break;
		}
		if (token->kind==JAGP_KIND_EOF) {
			break;
		}
	}
}

static JagPToken *l_token(JagPILexer *lexer) {
	JagPLexerImplPrivate *priv = jagp_lexer_impl_get_instance_private((JagPLexerImpl *) lexer);
	return (JagPToken *) cat_array_wo_get(priv->parser_tokens, priv->tokens_index);
}


static JagPToken *l_token_ahead(JagPILexer *lexer, int lookahead) {
	JagPLexerImplPrivate *priv = jagp_lexer_impl_get_instance_private((JagPLexerImpl *) lexer);
	int offset = priv->tokens_index + lookahead;
	while(offset>=cat_array_wo_size(priv->parser_tokens)) {
		JagPToken *token = l_scan_real_token((JagPLexerImpl *) lexer);
		if (token->kind==JAGP_KIND_EOF) {
			return token;
		}
	}
	return (JagPToken *) cat_array_wo_get(priv->parser_tokens, offset);
}

static JagPToken *l_prev_token(JagPILexer *lexer) {
	JagPLexerImplPrivate *priv = jagp_lexer_impl_get_instance_private((JagPLexerImpl *) lexer);
	return (JagPToken *) cat_array_wo_get(priv->parser_tokens, priv->tokens_index-1);
}


static JagPCursor *l_err_pos(JagPILexer *lexer) {
	JagPLexerImplPrivate *priv = jagp_lexer_impl_get_instance_private((JagPLexerImpl *) lexer);
	return priv->error_pos;
}

static void l_err_pos_set(JagPILexer *lexer, JagPCursor *cursor) {
	JagPLexerImplPrivate *priv = jagp_lexer_impl_get_instance_private((JagPLexerImpl *) lexer);
	cat_ref_swap(priv->error_pos, cursor);
}

static void l_lexer_iface_init(JagPILexerInterface *iface) {
	iface->nextToken = l_next_token;
	iface->token = l_token;
	iface->tokenAhead = l_token_ahead;
	iface->prevToken = l_prev_token;

	iface->errPos = l_err_pos;
	iface->errPosSet = l_err_pos_set;

}


/********************* end JagPILexer implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPLexerImpl *instance = JAGP_LEXER_IMPL(self);
	JagPLexerImplPrivate *priv = jagp_lexer_impl_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, tokens=%O]", iname, self, priv->tokens);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
