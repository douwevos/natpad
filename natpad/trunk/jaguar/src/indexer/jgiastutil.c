/*
 File:    jgiastutil.c
 Project: jaguar
 Author:  Douwe Vos
 Date:    Jun 16, 2014
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

#include "jgiastutil.h"
#include "jgiastutil.h"
#include <jaguarparser.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JgiAstUtil"
#include <logging/catlog.h>

JgiTokenRange *jgi_ast_util_extract_name_root(CatArrayWo *e_token_array, int token_index) {

	int mr_index = token_index;
	int ml_index = token_index;

	JagPToken *cur_token = (JagPToken *) cat_array_wo_get(e_token_array, ml_index);

	gboolean do_run_parenting = TRUE;
	int hold_index = -1;
	int dot_at_index = ml_index-1;

	cat_log_debug("token_array=%p, index=%d, cur_token=%o", e_token_array, token_index, cur_token);
	while(hold_index!=ml_index && dot_at_index>=0) {
		hold_index = ml_index;

		cur_token = (JagPToken *) cat_array_wo_get(e_token_array, dot_at_index);
		switch(cur_token->kind) {
			case JAGP_KIND_DOT : {
				ml_index--;
//				cur_token = (JagPToken *) cat_array_wo_get(e_token_array, ml_index);
				do_run_parenting = TRUE;
			} break;
			case JAGP_KIND_IDENTIFIER : {
				ml_index--;
//				cur_token = (JagPToken *) cat_array_wo_get(e_token_array, ml_index);
				do_run_parenting = TRUE;
			} break;
		}

		cat_log_debug("cur_token=%o, hold=%d, ml=%d", cur_token, hold_index, ml_index);

//		if (do_run_parenting) {
//			JagPJCTree *par = cur_token->owner;
//			while(do_run_parenting) {
//				JagPJCTree *next_par = par->owner;
//				cat_log_trace("next_par=%o", next_par);
//				if (next_par==NULL) {
//					do_run_parenting = FALSE;
//					break;
//				}
//				switch(JAG_TOKEN(next_par)->sym) {
//					case JAG_SYMBOL_NONTERM_NAME :
//					case JAG_SYMBOL_NONTERM_QUALIFIED_NAME : {
//						par = next_par;
//					} break;
//					default :
//						do_run_parenting = FALSE;
//				}
//			}
//			if (par!=cur_token) {
//				JagAstToken *mlt = jag_ast_token_get_most_left_terminal(par);
//				ml_index = cat_array_wo_find_index(e_token_array, mlt, ml_index-1);
//			}
			dot_at_index = ml_index-1;
//		}
	}

	JgiTokenRange *result = jgi_token_range_new(e_token_array, ml_index, mr_index);
	return result;
}



//
//static int l_dump_reverse(JagAstToken *child_token, JagAstToken *token_root_opt) {
//	JagAstToken *parent_ast_token = child_token->parent_token;
//	int indent = 0;
//	CatStringWo *e_indenttext = cat_string_wo_new();
//	if ((parent_ast_token!=NULL) && (child_token!=token_root_opt)) {
//		indent += l_dump_reverse(parent_ast_token, token_root_opt);
//		int t;
//		for(t=0; t<indent; t++) {
//			cat_string_wo_append_chars(e_indenttext, "  ");
//		}
//		JagAstTokenClass *parent_token_class = JAG_AST_TOKEN_GET_CLASS(parent_ast_token);
//		int child_count = parent_token_class->childCount(parent_ast_token);
//		int idx;
//		for(idx=0; idx<child_count; idx++) {
//			JagAstToken *child_at_idx = (JagAstToken *) (parent_token_class->childAt(parent_ast_token, idx));
//			JagToken *token = JAG_TOKEN(child_at_idx);
//			cat_log_print("DUMP", "%s%s[%d], state=%d, text=%s, l/r=%d#%d/%d#%d", cat_string_wo_getchars(e_indenttext), jag_token_symbol_as_text(token), token->sym, token->parse_state, token->symbol_text, token->left, token->left_row, token->right, token->right_row);
//			if (child_at_idx==child_token) {
//				break;
//			}
//		}
//	} else {
//		JagToken *token = JAG_TOKEN(child_token);
//		cat_log_print("DUMP", "%s%s[%d], state=%d, text=%s, l/r=%d#%d/%d#%d", cat_string_wo_getchars(e_indenttext), jag_token_symbol_as_text(token), token->sym, token->parse_state, token->symbol_text, token->left, token->left_row, token->right, token->right_row);
//	}
//
//	return indent+2;
//}
//
//
//void jgi_ast_util_dump_reverse(JagAstToken *token, JagAstToken *token_root_opt) {
//	l_dump_reverse(token, token_root_opt);
//}
//


