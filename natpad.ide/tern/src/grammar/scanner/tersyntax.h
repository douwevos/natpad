/*
   File:    tersyntax.h
   Project: tern
   Author:  Douwe Vos
   Date:    Jun 2, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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


#ifndef TERSYNTAX_H_
#define TERSYNTAX_H_
//
//#include <glib-object.h>
//#include "terunicharnode.h"
//
//G_BEGIN_DECLS
//
//#define TER_TYPE_SYNTAX            (ter_syntax_get_type())
//#define TER_SYNTAX(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_syntax_get_type(), TerSyntax))
//#define TER_SYNTAX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_SYNTAX, TerSyntaxClass))
//#define TER_IS_SYNTAX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_SYNTAX))
//#define TER_IS_SYNTAX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_SYNTAX))
//#define TER_SYNTAX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_SYNTAX, TerSyntaxClass))
//
//
//typedef struct _TerSyntax	      TerSyntax;
//typedef struct _TerSyntaxClass	  TerSyntaxClass;
//
//typedef enum _TerSyntaxMode    TerSyntaxMode;
//
//enum _TerSyntaxMode {
//	TER_MODE_TEXT,
//	TER_MODE_C,
//	TER_MODE_HTML,
//	TER_MODE_PERL,
//	TER_MODE_LATEX
//};
//
//struct _TerSyntax {
//	GObject parent;
//	CatStringWo *a_grammar_path;
//	TerUnicharNode *keyword_node;
//	CatHashSet *keyword_chars;
//
//	CatUnicharArray *hex_prefix;
//
//
//	CatUnicharArray *multi_line_comment_start;
//	CatUnicharArray *multi_line_comment_end;
//
//
//	CatUnicharArray *single_comment;
//	int single_comment_col;
//
//	CatUnicharArray *single_comment_alt;
//	int single_comment_col_alt;
//
//	CatHashSet *bracket_set;
//	CatHashSet *operator_char_set;
//
//	gunichar string_start, string_end;
//	TerSyntaxMode mode;
//};
//
//struct _TerSyntaxClass {
//	GObjectClass parent_class;
//};
//
//
//
//
//TerSyntax *ter_syntax_new(CatStringWo *a_grammar_path);
//TerUnicharNode *ter_syntax_create_keyword_end_node(TerSyntax *syntax, CatUnicharArray *key_array);
//
//
//
//void ter_syntax_set_mode(TerSyntax *syntax, TerSyntaxMode new_mode);
//TerSyntaxMode ter_syntax_get_mode(TerSyntax *syntax);
//
//CatStringWo *ter_syntax_get_grammar_path(TerSyntax *syntax);
//
//
//CatHashSet *ter_syntax_get_bracket_set(TerSyntax *syntax);
//CatHashSet *ter_syntax_get_operator_char_set(TerSyntax *syntax);
//
//
//G_END_DECLS
//
//
//
//
//
//
//








#include <caterpillar.h>
#include "terunicharnode.h"

G_BEGIN_DECLS

#define TER_TYPE_SYNTAX              (ter_syntax_get_type())
#define TER_SYNTAX(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_syntax_get_type(), TerSyntax))
#define TER_SYNTAX_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_SYNTAX, TerSyntaxClass))
#define TER_IS_SYNTAX(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_SYNTAX))
#define TER_IS_SYNTAX_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_SYNTAX))
#define TER_SYNTAX_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_SYNTAX, TerSyntaxClass))
#define TER_SYNTAX_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TER_TYPE_SYNTAX, TerSyntaxPrivate))

typedef struct _TerSyntax               TerSyntax;
typedef struct _TerSyntaxPrivate        TerSyntaxPrivate;
typedef struct _TerSyntaxClass          TerSyntaxClass;

typedef enum _TerSyntaxMode    TerSyntaxMode;

enum _TerSyntaxMode {
	TER_MODE_TEXT,
	TER_MODE_C,
	TER_MODE_HTML,
	TER_MODE_PERL,
	TER_MODE_LATEX
};

struct _TerSyntax {
	GObject parent;
	TerSyntaxPrivate *priv;
};

struct _TerSyntaxClass {
	GObjectClass parent_class;
};


GType ter_syntax_get_type();

TerSyntax *ter_syntax_new();


TerUnicharNode *ter_syntax_create_keyword_end_node(TerSyntax *syntax, CatUnicharArray *key_array);

TerUnicharNode *ter_syntax_get_keyword_node(TerSyntax *syntax);

gboolean ter_syntax_is_keyword_char(TerSyntax *syntax, CatUnichar *uchobj);


void ter_syntax_set_mode(TerSyntax *syntax, TerSyntaxMode new_mode);
TerSyntaxMode ter_syntax_get_mode(TerSyntax *syntax);

CatHashSet *ter_syntax_get_bracket_set(TerSyntax *syntax);
gboolean ter_syntax_is_bracket_char(TerSyntax *syntax, CatUnichar *uchobj);

CatHashSet *ter_syntax_get_operator_char_set(TerSyntax *syntax);
gboolean ter_syntax_is_operator_char(TerSyntax *syntax, CatUnichar *uchobj);


void ter_syntax_set_hex_prefix(TerSyntax *syntax, CatUnicharArray *nvalue);
CatUnicharArray *ter_syntax_get_hex_prefix(TerSyntax *syntax);



void ter_syntax_set_single_comment(TerSyntax *syntax, CatUnicharArray *new_single_comment);
CatUnicharArray *ter_syntax_get_single_comment(TerSyntax *syntax);

void ter_syntax_set_single_comment_column(TerSyntax *syntax, int new_column);
int ter_syntax_get_single_comment_column(TerSyntax *syntax);



void ter_syntax_set_single_comment_alt(TerSyntax *syntax, CatUnicharArray *new_single_comment);
CatUnicharArray *ter_syntax_get_single_comment_alt(TerSyntax *syntax);

void ter_syntax_set_single_comment_column_alt(TerSyntax *syntax, int new_column);
int ter_syntax_get_single_comment_column_alt(TerSyntax *syntax);



void ter_syntax_set_multi_line_comment_start(TerSyntax *syntax, CatUnicharArray *new_multi_line_comment_start);
CatUnicharArray *ter_syntax_get_multi_line_comment_start(TerSyntax *syntax);
void ter_syntax_set_multi_line_comment_end(TerSyntax *syntax, CatUnicharArray *new_multi_line_comment_end);
CatUnicharArray *ter_syntax_get_multi_line_comment_end(TerSyntax *syntax);


void ter_syntax_set_string_start(TerSyntax *syntax, gunichar string_start);
gunichar ter_syntax_get_string_start(TerSyntax *syntax);
void ter_syntax_set_string_end(TerSyntax *syntax, gunichar string_end);
gunichar ter_syntax_get_string_end(TerSyntax *syntax);


G_END_DECLS

#endif /* TERSYNTAX_H_ */
