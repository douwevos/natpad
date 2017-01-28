/*
   File:    jagasttoken.h
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


#ifndef JAGASTTOKEN_H_
#define JAGASTTOKEN_H_

#include <caterpillar.h>
#include "generated/runtime/jagtoken.h"

G_BEGIN_DECLS

#define JAG_TYPE_AST_TOKEN              (jag_ast_token_get_type())
#define JAG_AST_TOKEN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_token_get_type(), JagAstToken))
#define JAG_AST_TOKEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_AST_TOKEN, JagAstTokenClass))
#define JAG_IS_AST_TOKEN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_AST_TOKEN))
#define JAG_IS_AST_TOKEN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_AST_TOKEN))
#define JAG_AST_TOKEN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_AST_TOKEN, JagAstTokenClass))

typedef struct _JagAstToken               JagAstToken;
typedef struct _JagAstTokenClass          JagAstTokenClass;


struct _JagAstToken {
	JagToken parent;
	JagAstToken *parent_token;
	CatArrayWo *e_children;
};

struct _JagAstTokenClass {
	JagTokenClass parent_class;
	void (*enlistTerminals)(JagAstToken *ast_token, CatArrayWo *e_enlist);
	int (*countTerminals)(JagAstToken *ast_token);
	JagToken *(*getTerminalAt)(JagAstToken *ast_token, int idx);

	int (*childCount)(JagAstToken *ast_token);
	JagToken *(*childAt)(JagAstToken *ast_token, int idx);
	void (*childAdd)(JagAstToken *ast_token, JagAstToken *child_token);


	JagAstToken *(*getParent)(JagAstToken *ast_token);
};


GType jag_ast_token_get_type();


JagAstToken *jag_ast_token_new(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);

void jag_ast_token_construct(JagAstToken *ast_token, int sym);

JagAstToken *jag_ast_token_get_parent_real(JagAstToken *ast_token);


void jag_ast_token_add_child(JagAstToken *ast_token, JagAstToken *child_token);


void jag_ast_token_remove_all_children(JagAstToken *ast_token);


CatStringWo *jag_ast_token_full_qualified_name(JagAstToken *ast_token, int row, int column);

CatArrayWo *jag_ast_token_flat_qualified_name(JagAstToken *ast_token, CatArrayWo *e_enlist, int row, int column);

CatStringWo *jag_ast_token_as_string(JagAstToken *ast_token);
void jag_ast_token_tree_dump(JagAstToken *ast_token);


JagAstToken *jag_ast_token_get_most_left_terminal(JagAstToken *ast_token);
JagAstToken *jag_ast_token_get_most_right_terminal(JagAstToken *ast_token);


G_END_DECLS

#endif /* JAGASTTOKEN_H_ */
