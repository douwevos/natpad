/*
   File:    jagpilexer.c
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

#include "jagpilexer.h"

G_DEFINE_INTERFACE(JagPILexer, jagp_ilexer, G_TYPE_OBJECT);

static void jagp_ilexer_default_init (JagPILexerInterface *iface) {
}

void jagp_ilexer_next_token(JagPILexer *lexer) {
	JAGP_ILEXER_GET_INTERFACE(lexer)->nextToken(lexer);
}

JagPToken *jagp_ilexer_token(JagPILexer *lexer) {
	return JAGP_ILEXER_GET_INTERFACE(lexer)->token(lexer);
}

JagPToken *jagp_ilexer_split(JagPILexer *lexer) {
	// TODO
	return NULL;
}

JagPToken *jagp_ilexer_token_lah(JagPILexer *lexer, int lookahead) {
	return JAGP_ILEXER_GET_INTERFACE(lexer)->tokenAhead(lexer, lookahead);
}



