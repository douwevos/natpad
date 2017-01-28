/*
   File:    jgiparsercontext.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 10, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "jgiparsercontext.h"
#include "../model/jagastplaintype.h"
#include "../model/jagastgenericstype.h"
#include "../model/jagasttype.h"
#include "../../parser/jagasttoken.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JgiParserContext"
#include <logging/catlog.h>

struct _JgiParserContextPrivate {
	VipISequence *moo_sequence;
	MooNodeWo *e_main;


	/* next fields are for auto-completion only */
	int row;
	int column;
	CatArrayWo *e_token_array;
	int focus_index;

};


G_DEFINE_TYPE(JgiParserContext, jgi_parser_context, JAG_TYPE_PARSER_CONTEXT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_reduce(JagParserContext *parser_context, int replace_count, JagToken *replace_with_symbol);
static JagToken *l_scan_next(JagParserContext *parser_context, int eofSymbol);

static void jgi_parser_context_class_init(JgiParserContextClass *clazz) {
	g_type_class_add_private(clazz, sizeof(JgiParserContextPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagParserContextClass *par_clazz = JAG_PARSER_CONTEXT_CLASS(clazz);
	par_clazz->reduce = l_reduce;
	par_clazz->scanNext = l_scan_next;
}

static void jgi_parser_context_init(JgiParserContext *instance) {
	JgiParserContextPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JGI_TYPE_PARSER_CONTEXT, JgiParserContextPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JgiParserContext *instance = JGI_PARSER_CONTEXT(object);
	JgiParserContextPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_token_array);
	cat_unref_ptr(priv->moo_sequence);
	cat_unref_ptr(priv->e_main);
	G_OBJECT_CLASS(jgi_parser_context_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jgi_parser_context_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JgiParserContext *jgi_parser_context_new(JagIScanner *scanner, VipISequence *moo_sequence) {
	JgiParserContext *result = g_object_new(JGI_TYPE_PARSER_CONTEXT, NULL);
	cat_ref_anounce(result);
	JgiParserContextPrivate *priv = result->priv;
	jag_parser_context_construct((JagParserContext *) result, scanner);
	priv->moo_sequence = cat_ref_ptr(moo_sequence);
	priv->e_main = moo_node_wo_new(vip_isequence_next(moo_sequence), NULL, NULL, NULL, 0);

	priv->e_token_array = NULL;
	return result;
}

void jgi_parser_context_enlist_tokens(JgiParserContext *parser_context, int row, int column) {
	JgiParserContextPrivate *priv = JGI_PARSER_CONTEXT_GET_PRIVATE(parser_context);
	cat_unref_ptr(priv->e_token_array);
	priv->e_token_array = cat_array_wo_new();
	priv->focus_index = -1;
	priv->row = row;
	priv->column = column;
}




MooNodeWo *jgi_parser_context_get_root_node(JgiParserContext *parser_context) {
	return JGI_PARSER_CONTEXT_GET_PRIVATE(parser_context)->e_main;
}

int jgi_parser_context_get_focus_index(JgiParserContext *parser_context) {
	return JGI_PARSER_CONTEXT_GET_PRIVATE(parser_context)->focus_index;
}

CatArrayWo *jgi_parser_context_get_token_array(JgiParserContext *parser_context) {
	return JGI_PARSER_CONTEXT_GET_PRIVATE(parser_context)->e_token_array;
}


static int _ds(JagAstToken *parent_ast_token, JagAstToken *child_token) {
	int indent = 0;
	cat_log_on_debug({
		CatStringWo *e_indenttext = cat_string_wo_new();
		if (parent_ast_token!=NULL) {
			indent += _ds(parent_ast_token->parent_token, parent_ast_token);
			int t;
			for(t=0; t<indent; t++) {
				cat_string_wo_append_chars(e_indenttext, "  ");
			}
			JagAstTokenClass *parent_token_class = JAG_AST_TOKEN_GET_CLASS(parent_ast_token);
			int child_count = parent_token_class->childCount(parent_ast_token);
			int idx;
			for(idx=0; idx<child_count; idx++) {
				JagAstToken *child_at_idx = (JagAstToken *) (parent_token_class->childAt(parent_ast_token, idx));
				JagToken *token = JAG_TOKEN(child_at_idx);
				cat_log_debug("%s%s[%d], state=%d, text=%s, l/r=%d#%d/%d#%d", cat_string_wo_getchars(e_indenttext), jag_token_symbol_as_text(token), token->sym, token->parse_state, token->symbol_text, token->left, token->left_row, token->right, token->right_row);
				if (child_at_idx==child_token) {
					break;
				}
			}
		} else {
			JagToken *token = JAG_TOKEN(child_token);
			cat_log_debug("%s%s[%d], state=%d, text=%s, l/r=%d#%d/%d#%d", cat_string_wo_getchars(e_indenttext), jag_token_symbol_as_text(token), token->sym, token->parse_state, token->symbol_text, token->left, token->left_row, token->right, token->right_row);
		}

	});
	return indent+2;
}



void jgi_parser_context_dump_stack_with_focus(JgiParserContext *parser_context) {
	JgiParserContextPrivate *priv = JGI_PARSER_CONTEXT_GET_PRIVATE(parser_context);
	if (priv->focus_index==-1) {
		cat_log_debug("focus_index=-1, token_array.size=%d", cat_array_wo_size(priv->e_token_array));
	} else {
		JagAstToken *focus_token = (JagAstToken *) cat_array_wo_get(priv->e_token_array, priv->focus_index);
		_ds(focus_token->parent_token, focus_token);
	}
}



static void l_populate_model(JgiParserContext *parser_context, JagAstToken *ast_token) {
	JgiParserContextPrivate *priv = JGI_PARSER_CONTEXT_GET_PRIVATE(parser_context);

	switch(JAG_TOKEN(ast_token)->sym) {
	case JAG_SYMBOL_NONTERM_CLASS_HEADER : {
		JagAstToken *ast_class_name_token = JAG_AST_TOKEN(cat_array_wo_get(ast_token->e_children, 2));
		JagToken *token = JAG_TOKEN(ast_class_name_token);
		if (token->is_terminal && (CAT_IS_STRING_WO(token->value))) {
			CatStringWo *token_text = CAT_STRING_WO(token->value);
			MooNodeWo *e_class_node = moo_node_wo_new(vip_isequence_next(priv->moo_sequence), cat_ref_ptr(token_text), NULL, NULL, 0);
			moo_node_wo_append_child(priv->e_main, e_class_node);
			cat_unref_ptr(e_class_node);
		}
	} break;
	}

}



static void l_reduce(JagParserContext *parser_context, int replace_count, JagToken *replace_with_symbol) {
	JagAstToken *parent_ast = NULL;
	if (JAG_IS_AST_TOKEN(replace_with_symbol)) {
		parent_ast = JAG_AST_TOKEN(replace_with_symbol);
		int child_count = cat_array_wo_size(parser_context->e_stack);
		int child_idx;
		for(child_idx = child_count-replace_count; child_idx<child_count; child_idx++) {
			GObject *child = cat_array_wo_get(parser_context->e_stack, child_idx);
			if (JAG_IS_AST_TOKEN(child)) {
				JagAstToken *ch = JAG_AST_TOKEN(child);
				jag_ast_token_add_child(parent_ast, ch);
			} else {
				cat_log_error("token not an AST-token.");
				jag_token_dump(JAG_TOKEN(child));
				return;
			}
		}
	} else {
		cat_log_error("parent token not an AST-token.");
		jag_token_dump(JAG_TOKEN(replace_with_symbol));
	}

	JAG_PARSER_CONTEXT_CLASS(jgi_parser_context_parent_class)->reduce(parser_context, replace_count, replace_with_symbol);

	if (parent_ast) {
		l_populate_model(JGI_PARSER_CONTEXT(parser_context), parent_ast);
	}
}

static JagToken *l_scan_next(JagParserContext *parser_context, int eofSymbol) {
	JgiParserContextPrivate *priv = JGI_PARSER_CONTEXT_GET_PRIVATE(parser_context);
	JagToken *token = JAG_PARSER_CONTEXT_CLASS(jgi_parser_context_parent_class)->scanNext(parser_context, eofSymbol);
	if (priv->e_token_array==NULL) {
		return token;
	}
	if (priv->focus_index==-1) {
		if (token->right_row>priv->row) {
			priv->focus_index = cat_array_wo_size(priv->e_token_array);

		} else if (token->right_row==priv->row) {
			if (token->right >= priv->column) {
				priv->focus_index = cat_array_wo_size(priv->e_token_array);
			}
		} else if (token->right_row == -1 && token->sym==0) {
			priv->focus_index = cat_array_wo_size(priv->e_token_array);
		}
	}

	if (token!=NULL) {
		if (!token->is_terminal || token->sym != eofSymbol) {
			cat_array_wo_append(priv->e_token_array, (GObject *) token);
		}
	}
	return token;
}
