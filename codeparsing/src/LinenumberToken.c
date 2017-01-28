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
#include "../include/gvlcodeparsing/LinenumberToken.h"
#include "../include/gvlcodeparsing/private/tokenPrivate.h"

DEFINE_PRIVATE_TOKEN_STRUCT(linenumberTokenFunctionsStruct)

static void* constructor(TypeData typeData, ...);
static TypeData rtType(void);

static const struct linenumberTokenFunctionsStruct linenumberTokenFunctions = {
  (void*)Token_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType
};

const struct typeDataStruct linenumberTokenTypeDataStruct = {
  "LinenumberToken",
  constructor,
  ptrof(linenumberTokenFunctions),
  sizeof(struct private_tokenStruct),
  TOKEN_TYPE_DATA
};

LinenumberToken newLinenumberToken(int line, int column, PushbackStream stream, int cr,
                                   int code, StringBuilder clearedStringBuilder) {
  LinenumberToken newInstance;

  ___BTPUSH;
#ifdef DEBUG_MODE
  if(clearedStringBuilder->length != 0) {
    Errors.printMessageAndExit("The specified string builder hasn't been cleared");
  }
#endif
#ifdef GVLCP_TEST_MODE
  newInstance = constructor(LINENUMBER_TOKEN_TYPE_DATA, line, column, stream, cr, code, clearedStringBuilder);
#else
  newInstance = constructor(LINENUMBER_TOKEN_TYPE_DATA, line, column, stream, cr, code);
#endif
  ___BTPOP;
  return newInstance;
}

#define LinenumberToken struct private_tokenStruct*

#define LINE_NUMBER_MAX 0xFFFFFFFFul

static void* constructor(TypeData typeData, ...) {
  LinenumberToken newInstance;
  PushbackStream stream;
  va_list argp;
  int cr, len, startOff;
  DECLARE_STRING_BUILDER(sb);

  ___BTPUSH;
  newInstance = Token_constructor(typeData);
  va_start(argp, typeData);
  newInstance->line = va_arg(argp, int);
  newInstance->column = va_arg(argp, int);
  newInstance->value = 0;
  stream = va_arg(argp, void*);
  cr = va_arg(argp, int);
  newInstance->code = va_arg(argp, int);
  GET_STRING_BUILDER(sb);
  va_end(argp);

  len = 0;
  while(cr == '0') {
    ++len;
    SB_APPEND_CHAR(sb, cr);
    cr = stream->call->read(stream);
  }
  startOff = len;
  while(Char.isDigit(cr)) {
    ++len;
    SB_APPEND_CHAR(sb, cr);
    newInstance->value *= 10;
    newInstance->value += Char.valueOf(cr);
    cr = stream->call->read(stream);
  }
  stream->call->unread(stream, cr);
  if(newInstance->value > LINE_NUMBER_MAX  ||  len-startOff > 10) {
    Token_setErrwarn(newInstance, GVLCP_WARN_LINE_NUMBER_OUT_OF_RANGE);
  } else {
    Token_clearErrwarn(newInstance);
  }

  SET_ELEMENT(newInstance, sb);
  newInstance->length = len;
  newInstance->code2 = 0;
  ___BTPOP;
  return newInstance;
}

static TypeData rtType(void) {
  return LINENUMBER_TOKEN_TYPE_DATA;
}
