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

#include <gvlib/defs.h>
#include <gvlib/errors.h>
#include <gvlib/String.h>
#include <gvlcodeparsing/elements.h>
#include <gvlcodeparsing/ElemInfo.h>
#include <gvlcodeparsing/StringElemInfoMap.h>
#include "../../checodes.h"
#include "../cheExtraKeyWords.h"
#include "cheContextRelativeTokenCode.h"

static int get(CheState state, const Token* tokens, int sOff);
static gvboolean hasCapitalsOnly(String str);
static gvboolean hasTypeEnding(String str);
static gvboolean isFuncPtrDeclar(const Token* tokens, int sOff);
static gvboolean isType(CheState state, const Token* tokens, int sOff);

const struct cheContextRelativeTokenCodeStruct CheContextRelativeTokenCode = {
  get
};

static int get(CheState state, const Token* tokens, int sOff) {
  StringElemInfoMap map;
  ElemInfo elemInfo;
  int code;

  ___BTPUSH;
  map = CheExtraKeyWords.get();
#ifdef DEBUG_MODE
  if(map == NULL) {
    Errors.printMessageAndExit("The extra key words are not available");
  }
#endif
  code = Token_getCode(tokens[sOff]);
  if(code != GVLCP_CODE_IDENTIFIER) {
    ___BTPOP;
    return code;
  }
  elemInfo = map->call->get(map, tokens[sOff]->element);
  if(elemInfo != NULL) {
    ___BTPOP;
    return elemInfo->code;
  }
  if(isType(state, tokens, sOff)) {
    ___BTPOP;
    return GVLCP_CODE_USER_TYPE;
  }
  ___BTPOP;
  return GVLCP_CODE_IDENTIFIER;
}

static gvboolean hasCapitalsOnly(String str) {
	gvboolean result;
  int i, len;

  ___BTPUSH;
  i = 0;
  len = str->length;
  while(i < len  &&  str->buf[i] < 0x60) {
    ++i;
  }
  result = i == len;
  ___BTPOP;
  return result;
}

static gvboolean hasTypeEnding(String str) {
	gvboolean result;
  int off;

  ___BTPUSH;
  off = str->length - 2;
  result = str->length > 2  &&  str->buf[off] == '_'  &&  str->buf[off+1] == 't';
  ___BTPOP;
  return result;
}

/* PRECONDITION: tokens[sOff-2] might represent a type,
                 Token_getCode(tokens[sOff-1]) == GVLCP_CODE_PARENTHESIS_OPEN.
   Returns whether tokens[sOff] is an asterisk, tokens[sOff+1] is an identifier,
   tokens[sOff+2] is a ')' and tokens[sOff+3] is a '('. */
static gvboolean isFuncPtrDeclar(const Token* tokens, int sOff) {
	gvboolean result;

  ___BTPUSH;
  result = Token_getCode(tokens[sOff]) == GVLCP_CODE_ASTERISK            &&
           Token_getCode(tokens[sOff+1]) == GVLCP_CODE_IDENTIFIER        &&
           Token_getCode(tokens[sOff+2]) == GVLCP_CODE_PARENTHESIS_CLOSE &&
           Token_getCode(tokens[sOff+3]) == GVLCP_CODE_PARENTHESIS_OPEN;
  ___BTPOP;
  return result;
}

static gvboolean isType(CheState state, const Token* tokens, int sOff) {
	gvboolean result;

  ___BTPUSH;
  if(hasTypeEnding(tokens[sOff]->element)) {
    ___BTPOP;
    return true;
  }
  if(tokens[sOff]->element->buf[0] > 0x40  &&  tokens[sOff]->element->buf[0] < 0x5B) {
    result = !(hasCapitalsOnly(tokens[sOff]->element) ||
               (tokens[sOff+1]->element == elem_28 && !isFuncPtrDeclar(tokens, sOff+2)) ||
               (tokens[sOff+1]->element == elem_2C && !(gvboolean)state->parenthesesOpen) ||
               (tokens[sOff+1]->element == elem_3B && !state->typeDef) ||
               tokens[sOff+1]->element == elem_2E ||
               tokens[sOff+1]->element == elem_3A ||
               tokens[sOff+1]->element == elem_3D ||
               tokens[sOff+1]->element == elem_7D);
  } else {
    result = false;
  }
  ___BTPOP;
  return result;
}
