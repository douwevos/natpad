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
#include <gvlcodeparsing/PrimaryToken.h>
#include "cheContextRelativeTokenCode.h"
#include "chesetcolourcode.h"
#include "../../checodes.h"

static gvboolean longUsedAsType(const Token* tokens, int sOff) {
	gvboolean result;
  int code;

  ___BTPUSH;
  result = true;
  while(result  &&  tokens[sOff]->call->rtType() == PRIMARY_TOKEN_TYPE_DATA) {
    code = Token_getCode(tokens[sOff]);
    result = !(code == GVLCP_CODE_INT  ||  code == GVLCP_CODE_DOUBLE  ||  code == GVLCP_CODE_LONG);
    ++sOff;
  }
  ___BTPOP;
  return result;
}

static gvboolean shortUsedAsType(const Token* tokens, int sOff) {
	gvboolean result;

  ___BTPUSH;
  result = true;
  while(result  &&  tokens[sOff]->call->rtType() == PRIMARY_TOKEN_TYPE_DATA) {
    result = Token_getCode(tokens[sOff]) != GVLCP_CODE_INT;
    ++sOff;
  }
  ___BTPOP;
  return result;
}

void* che_set_colour_code__base(CheState state, const Token* tokens, int sOff) {
  void* (* result)(CheState, const Token*, int);
  int code, generalClassification;

  ___BTPUSH;
  code = CheContextRelativeTokenCode.get(state, tokens, sOff);
  generalClassification = code & GVLCP_MASK_GEN_CLASSIFICATION;
  switch(generalClassification) {

  case GVLCP_CODE_ADJECTIVE: {
    switch(code) {

    case GVLCP_CODE_LONG: {
      if(longUsedAsType(tokens, sOff+1)) {
        tokens[sOff]->code2 = CHE_CODE_TYPE;
        result = che_set_colour_code__type;
      } else {
        tokens[sOff]->code2 = CHE_CODE_ADJECTIVE;
        result = che_set_colour_code__base;
      }
    } break;

    case GVLCP_CODE_SHORT: {
      if(shortUsedAsType(tokens, sOff+1)) {
        tokens[sOff]->code2 = CHE_CODE_TYPE;
        result = che_set_colour_code__type;
      } else {
        tokens[sOff]->code2 = CHE_CODE_ADJECTIVE;
        result = che_set_colour_code__base;
      }
    } break;

    default: {
      tokens[sOff]->code2 = CHE_CODE_ADJECTIVE;
      result = che_set_colour_code__base;
    } break;

    }
  } break;

  case GVLCP_CODE_BUILT_IN_MACRO: {
    tokens[sOff]->code2 = CHE_CODE_FALSE_NULL_TRUE;
    result = che_set_colour_code__base;
  } break;

  case GVLCP_CODE_HALF_TYPE: {
    tokens[sOff]->code2 = CHE_CODE_HALFTYPE_PREFIX;
    result = che_set_colour_code__halftype;
  } break;

  case GVLCP_CODE_KEY_WORD: {
    tokens[sOff]->code2 = CHE_CODE_KEY_WORD;
    result = che_set_colour_code__base;
  } break;

  case GVLCP_CODE_NUMBER: {
    tokens[sOff]->code2 = CHE_CODE_NUMBER;
    result = che_set_colour_code__base;
  } break;

  case GVLCP_CODE_STATEMENT: {
    tokens[sOff]->code2 = CHE_CODE_STATEMENT;
    result = che_set_colour_code__base;
  } break;

  case GVLCP_CODE_STRING: {
    tokens[sOff]->code2 = CHE_CODE_STRING;
    result = che_set_colour_code__base;
  } break;

  case GVLCP_CODE_TYPE: {
    tokens[sOff]->code2 = CHE_CODE_TYPE;
    result = che_set_colour_code__type;
  } break;

  default: {
    tokens[sOff]->code2 = CHE_CODE_NONE;
    result = che_set_colour_code__base;
  } break;

  }
  ___BTPOP;
  return result;
}
