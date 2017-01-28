/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2010 Gerard Visser.

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

#include <gvlib/errors.h>
#include <gvlcodeparsing/codes.h>
#include "cheHighlightFilter.h"

static TokenList filter(const Token* tokens, int length);

const struct cheHighlightFilterStruct CheHighlightFilter = {
  filter
};

static TokenList filter(const Token* tokens, int length) {
  TokenList list;
  int i;

  ___BTPUSH;
  i = 0;
  list = newTokenList(length);
  while(i < length) {

    switch(Token_getCode(tokens[i])) {

    case GVLCP_CODE_COMMENT: {
      ++i;
    } break;

    case GVLCP_CODE_PREPROCESSOR_START: {
      ++i;
      while((tokens[i]->code & GVLCP_MASK_PREPROCESSOR) == GVLCP_CODE_PREPROCESSOR  ||
            Token_getCode(tokens[i]) == GVLCP_CODE_COMMENT) {
        ++i;
      }
    } break;

    default: {
      list->call->append(list, tokens[i]);
      ++i;
    } break;

    }

  }
  ___BTPOP;
  return list;
}
