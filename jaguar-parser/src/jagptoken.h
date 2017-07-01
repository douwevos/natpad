/*
   File:    jagptoken.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 1, 2017
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

#ifndef JAGPTOKEN_H_
#define JAGPTOKEN_H_

#include "jagpcursor.h"
#include "code/jagpname.h"
#include "code/jagpcomment.h"
#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS


typedef enum _JagPTokenKind JagPTokenKind;

enum _JagPTokenKind {
    JAGP_KIND_EOF,
	JAGP_KIND_ERROR,
	JAGP_KIND_IDENTIFIER,
	JAGP_KIND_ABSTRACT,
	JAGP_KIND_ASSERT,
	JAGP_KIND_BOOLEAN,
	JAGP_KIND_BREAK,
	JAGP_KIND_BYTE,
	JAGP_KIND_CASE,
	JAGP_KIND_CATCH,
	JAGP_KIND_CHAR,
	JAGP_KIND_CLASS,
	JAGP_KIND_CONST,
	JAGP_KIND_CONTINUE,
	JAGP_KIND_DEFAULT,
	JAGP_KIND_DO,
	JAGP_KIND_DOUBLE,
	JAGP_KIND_ELSE,
	JAGP_KIND_ENUM,
	JAGP_KIND_EXTENDS,
	JAGP_KIND_FINAL,
	JAGP_KIND_FINALLY,
	JAGP_KIND_FLOAT,
	JAGP_KIND_FOR,
	JAGP_KIND_GOTO,
	JAGP_KIND_IF,
	JAGP_KIND_IMPLEMENTS,
	JAGP_KIND_IMPORT,
	JAGP_KIND_INSTANCEOF,
	JAGP_KIND_INT,
	JAGP_KIND_INTERFACE,
	JAGP_KIND_LONG,
	JAGP_KIND_NATIVE,
	JAGP_KIND_NEW,
	JAGP_KIND_PACKAGE,
	JAGP_KIND_PRIVATE,
	JAGP_KIND_PROTECTED,
	JAGP_KIND_PUBLIC,
	JAGP_KIND_RETURN,
	JAGP_KIND_SHORT,
	JAGP_KIND_STATIC,
	JAGP_KIND_STRICTFP,
	JAGP_KIND_SUPER,
	JAGP_KIND_SWITCH,
	JAGP_KIND_SYNCHRONIZED,
	JAGP_KIND_THIS,
	JAGP_KIND_THROW,
	JAGP_KIND_THROWS,
	JAGP_KIND_TRANSIENT,
	JAGP_KIND_TRY,
	JAGP_KIND_VOID,
	JAGP_KIND_VOLATILE,
	JAGP_KIND_WHILE,
	JAGP_KIND_INTLITERAL,
	JAGP_KIND_LONGLITERAL,
	JAGP_KIND_FLOATLITERAL,
	JAGP_KIND_DOUBLELITERAL,
	JAGP_KIND_CHARLITERAL,
	JAGP_KIND_STRINGLITERAL,
	JAGP_KIND_TRUE,
	JAGP_KIND_FALSE,
	JAGP_KIND_NULL,
	JAGP_KIND_UNDERSCORE,
	JAGP_KIND_ARROW,
	JAGP_KIND_COLCOL,
	JAGP_KIND_LPAREN,
	JAGP_KIND_RPAREN,
	JAGP_KIND_LBRACE,
	JAGP_KIND_RBRACE,
	JAGP_KIND_LBRACKET,
	JAGP_KIND_RBRACKET,
	JAGP_KIND_SEMI,
	JAGP_KIND_COMMA,
	JAGP_KIND_DOT,
	JAGP_KIND_ELLIPSIS,
	JAGP_KIND_EQ,
	JAGP_KIND_GT,
	JAGP_KIND_LT,
	JAGP_KIND_BANG,
	JAGP_KIND_TILDE,
	JAGP_KIND_QUES,
	JAGP_KIND_COLON,
	JAGP_KIND_EQEQ,
	JAGP_KIND_LTEQ,
	JAGP_KIND_GTEQ,
	JAGP_KIND_BANGEQ,
	JAGP_KIND_AMPAMP,
	JAGP_KIND_BARBAR,
	JAGP_KIND_PLUSPLUS,
	JAGP_KIND_SUBSUB,
	JAGP_KIND_PLUS,
	JAGP_KIND_SUB,
	JAGP_KIND_STAR,
	JAGP_KIND_SLASH,
	JAGP_KIND_AMP,
	JAGP_KIND_BAR,
	JAGP_KIND_CARET,
	JAGP_KIND_PERCENT,
	JAGP_KIND_LTLT,
	JAGP_KIND_GTGT,
	JAGP_KIND_GTGTGT,
	JAGP_KIND_PLUSEQ,
	JAGP_KIND_SUBEQ,
	JAGP_KIND_STAREQ,
	JAGP_KIND_SLASHEQ,
	JAGP_KIND_AMPEQ,
	JAGP_KIND_BAREQ,
	JAGP_KIND_CARETEQ,
	JAGP_KIND_PERCENTEQ,
	JAGP_KIND_LTLTEQ,
	JAGP_KIND_GTGTEQ,
	JAGP_KIND_GTGTGTEQ,
	JAGP_KIND_MONKEYS_AT,
	JAGP_KIND_CUSTOM

};


#define JAGP_TYPE_TOKEN              (jagp_token_get_type())
#define JAGP_TOKEN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_token_get_type(), JagPToken))
#define JAGP_TOKEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_TOKEN, JagPTokenClass))
#define JAGP_IS_TOKEN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_TOKEN))
#define JAGP_IS_TOKEN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_TOKEN))
#define JAGP_TOKEN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_TOKEN, JagPTokenClass))


typedef struct _JagPToken               JagPToken;
typedef struct _JagPTokenClass          JagPTokenClass;

struct _JagPToken {
	GObject parent;
	JagPTokenKind kind;
	int pos;
	int endPos;

	JagPCursor *cur_start;
	JagPCursor *cur_end;
	
	
	GObject *value;
	GroRunFullToken *source;

};

struct _JagPTokenClass {
	GObjectClass parent_class;
	JagPName *(*name)(JagPToken *token);
};


GType jagp_token_get_type();

JagPToken *jagp_token_new();

JagPName *jagp_token_name(JagPToken *token);


JagPComment *jagp_token_comment(JagPToken *token, JagPCommentStyle comment_style);

G_END_DECLS

#endif /* JAGPTOKEN_H_ */
