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
#include "cheContextRelativeTokenCode.h"
#include "chesetcolourcode.h"
#include "../../checodes.h"

void* che_set_colour_code__declarvars(CheState state, const Token* tokens, int sOff) {
  void* (* result)(CheState, const Token*, int);
  int code;

  ___BTPUSH;
  code = CheContextRelativeTokenCode.get(state, tokens, sOff);
  switch(code) {

  case GVLCP_CODE_COMMA: {
    tokens[sOff]->code2 = CHE_CODE_NONE;
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
