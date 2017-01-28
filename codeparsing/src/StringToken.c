/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2009, 2010 Gerard Visser.

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
#include "../include/gvlcodeparsing/codes.h"
#include "../include/gvlcodeparsing/StringToken.h"
#include "../include/gvlcodeparsing/private/tokenPrivate.h"

DEFINE_PRIVATE_TOKEN_STRUCT(stringTokenFunctionsStruct)

static void* constructor(TypeData typeData, ...);
static TypeData rtType(void);

static const struct stringTokenFunctionsStruct stringTokenFunctions = {
  (void*)Token_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType
};

const struct typeDataStruct stringTokenTypeDataStruct = {
  "StringToken",
  constructor,
  ptrof(stringTokenFunctions),
  sizeof(struct private_tokenStruct),
  TOKEN_TYPE_DATA
};

StringToken newStringToken(int line, int column, PushbackStream stream, StringBuilder clearedStringBuilder) {
  StringToken newInstance;

  ___BTPUSH;
#ifdef DEBUG_MODE
  if(clearedStringBuilder->length != 0) {
    Errors.printMessageAndExit("The specified string builder hasn't been cleared");
  }
#endif
#ifdef GVLCP_TEST_MODE
  clearedStringBuilder->call->append(clearedStringBuilder, '"');
  newInstance = constructor(STRING_TOKEN_TYPE_DATA, line, column, stream, clearedStringBuilder);
#else
  newInstance = constructor(STRING_TOKEN_TYPE_DATA, line, column, stream);
#endif
  ___BTPOP;
  return newInstance;
}

#define StringToken struct private_tokenStruct*

static void* constructor(TypeData typeData, ...) {
  StringToken newInstance;
  PushbackStream stream;
  va_list argp;
  gvboolean bsl;
  int cr, len;
  DECLARE_STRING_BUILDER(sb);

  ___BTPUSH;
  newInstance = Token_constructor(typeData);
  va_start(argp, typeData);
  newInstance->line = va_arg(argp, int);
  newInstance->column = va_arg(argp, int);
  newInstance->code = GVLCP_CODE_STRING;
  stream = va_arg(argp, void*);
  GET_STRING_BUILDER(sb);
  va_end(argp);
  len = 1;
  bsl = false;
  cr = stream->call->read(stream);
  while(!(cr==-1 || cr=='\n' || (cr=='"' && !bsl))) {
    SB_APPEND_WIDE_CHAR(sb, cr);
    bsl = cr == '\\' && !bsl;
    cr = stream->call->read(stream);
    ++len;
  }
  if(cr != '"') {
    Token_setErrwarn(newInstance, GVLCP_ERROR_MISSING_TERMINATING_CHARACTER);
    stream->call->unread(stream, cr);
  } else {
    Token_clearErrwarn(newInstance);
    SB_APPEND_CHAR(sb, cr);
    ++len;
  }
  SET_ELEMENT(newInstance, sb);
  newInstance->length = len;
  newInstance->code2 = 0;
  newInstance->value = 0;
  ___BTPOP;
  return newInstance;
}

static TypeData rtType(void) {
  return STRING_TOKEN_TYPE_DATA;
}
