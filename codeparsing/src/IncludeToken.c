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
#include "../include/gvlcodeparsing/IncludeToken.h"
#include "../include/gvlcodeparsing/private/tokenPrivate.h"

DEFINE_PRIVATE_TOKEN_STRUCT(includeTokenFunctionsStruct)

static void* constructor(TypeData typeData, ...);
static TypeData rtType(void);

static const struct includeTokenFunctionsStruct includeTokenFunctions = {
  (void*)Token_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType
};

const struct typeDataStruct includeTokenTypeDataStruct = {
  "IncludeToken",
  constructor,
  ptrof(includeTokenFunctions),
  sizeof(struct private_tokenStruct),
  TOKEN_TYPE_DATA
};

IncludeToken newIncludeToken(int line, int column, PushbackStream stream, int cr, StringBuilder clearedStringBuilder) {
  IncludeToken newInstance;

  ___BTPUSH;
#ifdef DEBUG_MODE
  if(clearedStringBuilder->length != 0) {
    Errors.printMessageAndExit("The specified string builder hasn't been cleared");
  }
#endif
  newInstance = constructor(INCLUDE_TOKEN_TYPE_DATA, line, column, stream, cr, clearedStringBuilder);
  ___BTPOP;
  return newInstance;
}

#define IncludeToken struct private_tokenStruct*

static void* constructor(TypeData typeData, ...) {
  IncludeToken newInstance;
  PushbackStream stream;
  StringBuilder sb;
  int64_t utf8code;
  va_list argp;
  int cr, finalChar, len;

  ___BTPUSH;
  newInstance = Token_constructor(typeData);
  va_start(argp, typeData);
  newInstance->line = va_arg(argp, int);
  newInstance->column = va_arg(argp, int);
  stream = va_arg(argp, void*);
  cr = va_arg(argp, int);
  sb = va_arg(argp, void*);
  va_end(argp);

  len = 1;
  switch(cr) {

  case '"': {
    finalChar = '"';
    newInstance->code = GVLCP_CODE_FILE_NAME | GVLCP_CODE_PREPROCESSOR | GVLCP_CODE_RELATIVE;
  } break;

  case '<': {
    finalChar = '>';
    newInstance->code = GVLCP_CODE_FILE_NAME | GVLCP_CODE_PREPROCESSOR;
  } break;

  default: {
    finalChar = -1;
    newInstance->code = GVLCP_CODE_FILE_NAME | GVLCP_CODE_PREPROCESSOR;
    utf8code = Char.toUtf8(cr);
    while(utf8code > 0) {
      sb->call->append(sb, (char)utf8code);
      utf8code >>= 8;
    }
  } break;

  }
  cr = stream->call->read(stream);
  while(!(cr==-1 || cr==0x0A || cr==finalChar || cr=='\\')) {
    if(cr > 0x7F) {
      utf8code = Char.toUtf8(cr);
      while(utf8code > 0) {
        sb->call->append(sb, (char)utf8code);
        utf8code >>= 8;
      }
    } else {
      sb->call->append(sb, (char)cr);
    }
    cr = stream->call->read(stream);
    ++len;
  }

  if(finalChar == -1) {
    stream->call->unread(stream, cr);
    Token_setErrwarn(newInstance, GVLCP_ERROR_INCLUDE);
  } else if(cr != finalChar) {
    stream->call->unread(stream, cr);
    Token_setErrwarn(newInstance, GVLCP_ERROR_MISSING_TERMINATING_CHARACTER);
  } else {
    ++len;
    Token_clearErrwarn(newInstance);
  }

  newInstance->element = sb->call->toString(sb);
  newInstance->length = len;
  newInstance->code2 = 0;
  newInstance->value = 0;
  sb->call->clear(sb);
  ___BTPOP;
  return newInstance;
}

static TypeData rtType(void) {
  return INCLUDE_TOKEN_TYPE_DATA;
}
