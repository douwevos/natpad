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
#include "../include/gvlcodeparsing/codes.h"
#include "../include/gvlcodeparsing/ExtraToken.h"
#include "../include/gvlcodeparsing/private/tokenPrivate.h"

DEFINE_PRIVATE_TOKEN_STRUCT(extraTokenFunctionsStruct)

static void* constructor(TypeData typeData, ...);
static TypeData rtType(void);

static const struct extraTokenFunctionsStruct extraTokenFunctions = {
  (void*)Token_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType
};

const struct typeDataStruct extraTokenTypeDataStruct = {
  "ExtraToken",
  constructor,
  ptrof(extraTokenFunctions),
  sizeof(struct private_tokenStruct),
  TOKEN_TYPE_DATA
};

ExtraToken newExtraToken(int line, int column, PushbackStream stream, int cr, StringBuilder clearedStringBuilder) {
  ExtraToken newInstance;

  ___BTPUSH;
#ifdef DEBUG_MODE
  if(clearedStringBuilder->length != 0) {
    Errors.printMessageAndExit("The specified string builder hasn't been cleared");
  }
#endif
#ifdef GVLCP_TEST_MODE
  newInstance = constructor(EXTRA_TOKEN_TYPE_DATA, line, column, stream, cr, clearedStringBuilder);
#else
  newInstance = constructor(EXTRA_TOKEN_TYPE_DATA, line, column, stream, cr);
#endif
  ___BTPOP;
  return newInstance;
}

#define ExtraToken struct private_tokenStruct*

static void* constructor(TypeData typeData, ...) {
  ExtraToken newInstance;
  PushbackStream stream;
  va_list argp;
  int cr, len;
  DECLARE_STRING_BUILDER(sb);

  ___BTPUSH;
  newInstance = Token_constructor(typeData);
  va_start(argp, typeData);
  newInstance->line = va_arg(argp, int);
  newInstance->column = va_arg(argp, int);
  newInstance->code = GVLCP_CODE_IGNORE | GVLCP_CODE_PREPROCESSOR;
  stream = va_arg(argp, void*);
  cr = va_arg(argp, int);
  GET_STRING_BUILDER(sb);
  va_end(argp);

  len = 0;
  while(!(cr==-1 || Char.isSpace(cr) || cr=='\\')) {
    ++len;
    SB_APPEND_WIDE_CHAR(sb, cr);
    cr = stream->call->read(stream);
  }
  stream->call->unread(stream, cr);
  Token_clearErrwarn(newInstance);

  SET_ELEMENT(newInstance, sb);
  newInstance->length = len;
  newInstance->code2 = 0;
  newInstance->value = 0;
  ___BTPOP;
  return newInstance;
}

static TypeData rtType(void) {
  return EXTRA_TOKEN_TYPE_DATA;
}
