/*
   File:    jagpilexer.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 13, 2017
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

#ifndef JAGPILEXER_H_
#define JAGPILEXER_H_

#include "jagptoken.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_ILEXER                 (jagp_ilexer_get_type())
#define JAGP_ILEXER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAGP_TYPE_ILEXER, JagPILexer))
#define JAGP_IS_ILEXER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAGP_TYPE_ILEXER))
#define JAGP_ILEXER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAGP_TYPE_ILEXER, JagPILexerInterface))

typedef struct _JagPILexer               JagPILexer; /* dummy object */
typedef struct _JagPILexerInterface      JagPILexerInterface;

struct _JagPILexerInterface {
	GTypeInterface parent_iface;

    /**
     * Consume the next token.
     */
	void (*nextToken)(JagPILexer *lexer);

    /**
     * Return current token.
     */
    JagPToken *(*token)(JagPILexer *lexer);

    /**
     * Return token with given lookahead.
     */
    JagPToken *(*tokenAhead)(JagPILexer *lexer, int lookahead);

    /**
     * Return the last character position of the previous token.
     */
    JagPToken *(*prevToken)(JagPILexer *lexer);

    /**
     * Splits the current token in two and return the first (splitted) token.
     * For instance {@literal '<<<'} is split into two tokens
     * {@literal '<'} and {@literal '<<'} respectively,
     * and the latter is returned.
     */
    JagPToken *(*split)(JagPILexer *lexer);

    /**
     * Return the position where a lexical error occurred;
     */
    int (*errPos)(JagPILexer *lexer);

    /**
     * Set the position where a lexical error occurred;
     */
    void (*errPosSet)(JagPILexer *lexer, int pos);

//    /**
//     * Build a map for translating between line numbers and
//     * positions in the input.
//     *
//     * @return a LineMap
//     */
//    JagPLineMap *(*getLineMap)(JagPILexer *lexer);

};

GType jagp_ilexer_get_type(void);

void jagp_ilexer_next_token(JagPILexer *lexer);

JagPToken *jagp_ilexer_token(JagPILexer *lexer);

JagPToken *jagp_ilexer_split(JagPILexer *lexer);

JagPToken *jagp_ilexer_token_lah(JagPILexer *lexer, int lookahead);

JagPToken *jagp_ilexer_prev_token(JagPILexer *lexer);


G_END_DECLS

#endif /* JAGPILEXER_H_ */
