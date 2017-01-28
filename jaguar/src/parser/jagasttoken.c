/*
   File:    jagasttoken.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jan 3, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "jagasttoken.h"
#include "../indexer/model/jagasttype.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstToken"
#include <logging/catlog.h>

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstToken, jag_ast_token, JAG_TYPE_TOKEN, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);



static void l_enlist_terminals(JagAstToken *ast_token, CatArrayWo *e_enlist);
static int l_count_terminals(JagAstToken *ast_token);
static JagToken *l_get_terminal_at(JagAstToken *ast_token, int idx);

static int l_child_count(JagAstToken *ast_token);
static JagToken *l_child_at(JagAstToken *ast_token, int idx);
static void l_child_add(JagAstToken *ast_token, JagAstToken *child_token);



static void jag_ast_token_class_init(JagAstTokenClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);

	clazz->enlistTerminals = l_enlist_terminals;
	clazz->countTerminals = l_count_terminals;
	clazz->getTerminalAt = l_get_terminal_at;

	clazz->childCount = l_child_count;
	clazz->childAt = l_child_at;
	clazz->childAdd = l_child_add;

	clazz->getParent = jag_ast_token_get_parent_real;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;

}

static void jag_ast_token_init(JagAstToken *instance) {
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstToken *instance = JAG_AST_TOKEN(object);
//	JagAstTokenPrivate *priv = instance->priv;
	jag_ast_token_remove_all_children(instance);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagAstToken *jag_ast_token_new(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	JagAstToken *result = g_object_new(JAG_TYPE_AST_TOKEN, NULL);
	cat_ref_anounce(result);
	jag_ast_token_construct(result, symbol);
	JagToken *token = JAG_TOKEN(result);
	token->parse_state = parse_state;
	token->is_terminal = is_terminal;
	token->is_error = is_error;
	token->used_by_parser = used_by_parser;
	token->left = left;
	token->left_row = left_row;
	token->right = right;
	token->right_row = right_row;
	token->symbol_text = is_terminal ? jag_symbol_terminal_as_string(symbol) : jag_symbol_non_terminal_as_string(symbol);
	token->value = cat_ref_ptr(val);
	return result;
}


void jag_ast_token_construct(JagAstToken *ast_token, int sym) {
	JagToken *token = JAG_TOKEN(ast_token);
	token->is_error = FALSE;
	token->is_terminal = FALSE;
	token->left = -1;
	token->left_row = -1;
	token->right = -1;
	token->right_row = -1;
	token->parse_state = -1;
	token->used_by_parser = FALSE;
	token->sym = sym;
	token->symbol_text = jag_symbol_non_terminal_as_string(sym);
	token->value = NULL;

	ast_token->parent_token = NULL;
	ast_token->e_children = NULL;
}



JagAstToken *jag_ast_token_get_parent_real(JagAstToken *ast_token) {
	return ast_token->parent_token;
}


static int l_child_count(JagAstToken *ast_token) {
	return ast_token->e_children==NULL ? 0 : cat_array_wo_size(ast_token->e_children);
}

static JagToken *l_child_at(JagAstToken *ast_token, int idx) {
	if (ast_token->e_children==NULL) {
		return NULL;
	}
	if (idx<0 || idx>=cat_array_wo_size(ast_token->e_children)) {
		return NULL;
	}
	return (JagToken *) cat_array_wo_get(ast_token->e_children, idx);
}


static void l_update_left_right(JagAstToken *ast_token) {
	JagToken *token = JAG_TOKEN(ast_token);
	if (ast_token->e_children!=NULL) {
		int count = cat_array_wo_size(ast_token->e_children);
		if (count>0) {
			JagToken *t = (JagToken *) cat_array_wo_get(ast_token->e_children,0);
			token->left = t->left;
			token->left_row = t->left_row;
			t = (JagToken *) cat_array_wo_get(ast_token->e_children,count-1);
			token->right = t->right;
			token->right_row = t->right_row;
			return;
		}
	}
	token->left = -1;
	token->left_row = -1;
	token->right = -1;
	token->right_row = -1;
}

static void l_child_add(JagAstToken *ast_token, JagAstToken *child_token) {
	if (ast_token->e_children==NULL) {
		ast_token->e_children = cat_array_wo_new();
	}
	cat_array_wo_append(ast_token->e_children, (GObject *) child_token);
	child_token->parent_token = ast_token;
	l_update_left_right(ast_token);

}


void jag_ast_token_add_child(JagAstToken *ast_token, JagAstToken *child_token) {
	if (ast_token->e_children==NULL) {
		ast_token->e_children = cat_array_wo_new();
	}
	cat_array_wo_append(ast_token->e_children, (GObject *) child_token);
	child_token->parent_token = ast_token;
	l_update_left_right(ast_token);
}


void jag_ast_token_remove_all_children(JagAstToken *ast_token) {
	if (ast_token->e_children!=NULL) {
		CatIIterator *iter = cat_array_wo_iterator(ast_token->e_children);
		while(cat_iiterator_has_next(iter)) {
			JagAstToken *child = (JagAstToken *) cat_iiterator_next(iter);
			child->parent_token = NULL;
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(ast_token->e_children);
		l_update_left_right(ast_token);
	}
}


static void l_enlist_terminals(JagAstToken *ast_token, CatArrayWo *e_enlist) {
	JagAstTokenClass *ast_token_class = JAG_AST_TOKEN_GET_CLASS(ast_token);
	int childCount = ast_token_class->childCount(ast_token);
	int childIdx;
	for(childIdx=0; childIdx<childCount; childIdx++) {
		JagToken *child_token = ast_token_class->getTerminalAt(ast_token, childIdx);
		if (JAG_IS_AST_TOKEN(child_token)) {
			JAG_AST_TOKEN_GET_CLASS(child_token)->enlistTerminals(JAG_AST_TOKEN(child_token), e_enlist);
		} else {
			cat_array_wo_append(e_enlist, (GObject *) child_token);
		}
	}
}

static int l_count_terminals(JagAstToken *ast_token) {
	JagAstTokenClass *ast_token_class = JAG_AST_TOKEN_GET_CLASS(ast_token);
	int childCount = ast_token_class->childCount(ast_token);
	int childIdx;
	int result = 0;
	for(childIdx=0; childIdx<childCount; childIdx++) {
		JagToken *child_token = ast_token_class->getTerminalAt(ast_token, childIdx);
		if (JAG_IS_AST_TOKEN(child_token)) {
			result += JAG_AST_TOKEN_GET_CLASS(child_token)->countTerminals(JAG_AST_TOKEN(child_token));
		} else {
			result++;
		}
	}
	return result;
}

static JagToken *l_get_terminal_at(JagAstToken *ast_token, int idx) {
	JagAstTokenClass *ast_token_class = JAG_AST_TOKEN_GET_CLASS(ast_token);
	int childCount = ast_token_class->childCount(ast_token);
	int offset = 0;
	JagToken *result = NULL;
	int childIdx;
	for(childIdx=0; childIdx<childCount; childIdx++) {
		JagToken *child_token = ast_token_class->getTerminalAt(ast_token, childIdx);
		if (JAG_IS_AST_TOKEN(child_token)) {
			int child_token_count = JAG_AST_TOKEN_GET_CLASS(child_token)->countTerminals(JAG_AST_TOKEN(child_token));
			if (idx>=offset && idx<offset+child_token_count) {
				result = JAG_AST_TOKEN_GET_CLASS(child_token)->getTerminalAt(JAG_AST_TOKEN(child_token), idx-offset);
				break;
			}
			offset+=child_token_count;
		} else {
			if (offset==idx) {
				result = child_token;
				break;
			}
			offset++;
		}
	}
	return result;
}


CatStringWo *jag_ast_token_full_qualified_name(JagAstToken *ast_token, int row, int column) {
	CatStringWo *a_result = NULL;
	JagToken *token = JAG_TOKEN(ast_token);
	if (token->is_terminal && (CAT_IS_STRING_WO(token->value))) {
		CatStringWo *a_token_text = CAT_STRING_WO(token->value);
		if (a_token_text!=NULL) {
			if (row<0 || row>token->right_row || (row==token->right_row && column>=token->right)) {
				a_result = cat_ref_ptr(a_token_text);
			} else {
				int sublen = column - token->left;
				a_result = cat_string_wo_new_sub(a_token_text,0,sublen);
				a_result = cat_string_wo_anchor(a_result, 0);
			}
		}
		if (a_result == NULL) {
			a_result = cat_string_wo_new_with("");
		}
	} else if (ast_token->e_children) {
		a_result = cat_string_wo_new_with("");
		int idx=0;
		for(idx=0; idx<cat_array_wo_size(ast_token->e_children); idx++) {
			JagToken *token = (JagToken *) cat_array_wo_get(ast_token->e_children, idx);
			if (row>=0 && (row<token->left_row || (row==token->left_row && column<=token->left))) {
				break;
			}
			CatStringWo *a_token_text = NULL;
			if (CAT_IS_STRING_WO(token->value)) {
				a_token_text = cat_ref_ptr(CAT_STRING_WO(token->value));
			} else if (token->sym==JAG_SYMBOL_TERM_DOT && token->is_terminal) {
				a_token_text = cat_ref_ptr(cat_string_wo_new_with("."));
			} else {
				a_token_text = jag_ast_token_full_qualified_name(JAG_AST_TOKEN(token), row, column);
			}

			if (a_token_text!=NULL) {
				if (row<0 || row>token->right_row || (row==token->right_row && column>=token->right)) {
					cat_string_wo_append(a_result, a_token_text);
				} else {
					int sublen = column - token->left;
					CatStringWo *a_subpart = cat_string_wo_new_sub(a_token_text,0,sublen);
					cat_string_wo_append(a_result, a_subpart);
					cat_unref_ptr(a_subpart);
				}
				cat_unref_ptr(a_token_text);
			}
		}
	} else {
		a_result = cat_string_wo_new_with("");
	}
	return a_result;
}


CatArrayWo *jag_ast_token_flat_qualified_name(JagAstToken *ast_token, CatArrayWo *e_enlist, int row, int column) {
	if (e_enlist==NULL) {
		e_enlist = cat_array_wo_new();
	}
	if (ast_token->e_children) {
		gboolean last_was_dot_token = FALSE;
		int idx=0;
		for(idx=0; idx<cat_array_wo_size(ast_token->e_children); idx++) {
			JagToken *token = (JagToken *) cat_array_wo_get(ast_token->e_children, idx);
			if (row>=0 && (row<token->left_row || (row==token->left_row && column<=token->left))) {
				break;
			}
			last_was_dot_token = FALSE;
			CatStringWo *a_token_text = NULL;
			if (CAT_IS_STRING_WO(token->value)) {
				a_token_text = cat_ref_ptr(CAT_STRING_WO(token->value));
			} else if (token->sym==JAG_SYMBOL_TERM_DOT && token->is_terminal) {
				last_was_dot_token = TRUE;
			} else {
				jag_ast_token_flat_qualified_name(JAG_AST_TOKEN(token), e_enlist, row, column);
			}

			if (a_token_text!=NULL) {
				if (row<0 || row>token->right_row || (row==token->right_row && column>=token->right)) {
					cat_array_wo_append(e_enlist, (GObject *) a_token_text);
				} else {
					int sublen = column - token->left;
					CatStringWo *a_sub = cat_string_wo_new_sub(a_token_text,0,sublen);
					cat_array_wo_append(e_enlist, (GObject *) a_sub);
					cat_unref_ptr(a_sub);
				}
				cat_unref_ptr(a_token_text);
			}
		}
		if (last_was_dot_token) {
			CatStringWo *a_sub = cat_string_wo_new_with("");
			cat_array_wo_append(e_enlist, (GObject *) a_sub);
			cat_unref_ptr(a_sub);
		}
	}

	return e_enlist;
}

void jag_ast_token_tree_dump(JagAstToken *ast_token) {
	if (ast_token==NULL) {
		cat_log_print("DUMP", "JagAstToken.tree=null");
		return;
	}
	CatStringWo *a_indent = cat_string_wo_new();
	CatArrayWo *e_stack = cat_array_wo_new();
	cat_array_wo_append(e_stack, (GObject *) ast_token);
	while(cat_array_wo_size(e_stack)>0) {
		ast_token = NULL;
		GObject *top_item = NULL;
		cat_array_wo_remove_last(e_stack, &top_item);
		if (CAT_IS_STRING_WO(top_item)) {
			cat_unref_ptr(a_indent);
			a_indent = CAT_STRING_WO(top_item);
		} else if (top_item==NULL) {
			cat_log_print("DUMP", "%s[null]", cat_string_wo_getchars(a_indent));
		} else if (JAG_IS_AST_TOKEN(top_item)) {
			ast_token = JAG_AST_TOKEN(top_item);
			CatStringWo *a_token_as_str = jag_ast_token_as_string(ast_token);
			cat_log_print("DUMP", "%s%s", cat_string_wo_getchars(a_indent), cat_string_wo_getchars(a_token_as_str));
			cat_unref_ptr(a_token_as_str);
			JagAstTokenClass *token_class = JAG_AST_TOKEN_GET_CLASS(ast_token);
			int child_count = token_class->childCount(ast_token);
			if (child_count>0) {
				cat_array_wo_append(e_stack, (GObject *) a_indent);
//				cat_unref(indent);
				CatStringWo *e_indent = cat_string_wo_clone(a_indent, CAT_CLONE_DEPTH_MAIN);
				cat_string_wo_append_chars(e_indent, "  ");
				a_indent = cat_string_wo_anchor(e_indent, 0);
				for(child_count--; child_count>=0; child_count--) {
					JagToken *child = token_class->childAt(ast_token, child_count);
					cat_array_wo_append(e_stack, (GObject *) child);
				}
			}
//			cat_unref_ptr(ast_token);
		}
	}

	cat_unref_ptr(e_stack);
	cat_unref_ptr(a_indent);
}

JagAstToken *jag_ast_token_get_most_left_terminal(JagAstToken *ast_token) {
	while(ast_token!=NULL) {
		JagAstTokenClass *tclass = JAG_AST_TOKEN_GET_CLASS(ast_token);
		if (tclass->childCount(ast_token)>0) {
			ast_token = JAG_AST_TOKEN(tclass->childAt(ast_token, 0));
		} else {
			break;
		}
	}
	return ast_token;
}


JagAstToken *jag_ast_token_get_most_right_terminal(JagAstToken *ast_token) {
	while(ast_token!=NULL) {
		JagAstTokenClass *tclass = JAG_AST_TOKEN_GET_CLASS(ast_token);
		int cc = tclass->childCount(ast_token);
		if (cc>0) {
			ast_token = JAG_AST_TOKEN(tclass->childAt(ast_token, cc-1));
		} else {
			break;
		}
	}
	return ast_token;
}





CatStringWo *jag_ast_token_as_string(JagAstToken *ast_token) {
	JagToken *token = JAG_TOKEN(ast_token);
	CatStringWo *e_result = cat_string_wo_new_with("JagAstToken[");
	if (ast_token==NULL) {
		cat_string_wo_append_chars(e_result, "NULL");
	} else {
		char buf[500];
		sprintf(buf, "[%d], state=%d, text=%s, left=%d:%d, right=%d:%d", token->sym, token->parse_state, token->symbol_text, token->left, token->left_row, token->right, token->right_row);
		cat_string_wo_append_chars(e_result, buf);
		cat_string_wo_append_chars(e_result, ", value=");
		if (token->value==NULL) {
			cat_string_wo_append_chars(e_result, "NULL");
		} else if (CAT_IS_STRING_WO(token->value)) {
			cat_string_wo_append(e_result, CAT_STRING_WO(token->value));
		} else if (JAG_IS_AST_TYPE(token->value)) {	// TODO dirty reference to AstType ... ast-token should not have any knowledge of JagAstType
			JagAstType *ast_type = JAG_AST_TYPE(token->value);
			cat_string_wo_append(e_result, JAG_AST_TYPE_GET_CLASS(ast_type)->toString(ast_type));
		} else {
			cat_string_wo_append_chars(e_result, g_type_name_from_instance((GTypeInstance *) token->value));
		}
	}
	cat_string_wo_append_chars(e_result, "]");
	return e_result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
	JagToken *token = JAG_TOKEN(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(e_append_to, "%s[%p:sym=[%d]/%s, state=%d, first=%d:%d, last=%d:%d, value=%o]", iname, self, token->sym, token->symbol_text, token->parse_state, token->left, token->left_row, token->right, token->right_row, token->value);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
