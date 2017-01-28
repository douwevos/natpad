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
#include "../include/gvlcodeparsing/ElemInfo.h"
#include "../include/gvlcodeparsing/keyWords.h"
#include "../include/gvlcodeparsing/private/primaryTokenPrivate.h"

DEFINE_PRIVATE_TOKEN_STRUCT(primaryTokenFunctionsStruct)

static TypeData rtType(void);

static const struct primaryTokenFunctionsStruct primaryTokenFunctions = {
  (void*)Token_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType
};

const struct typeDataStruct primaryTokenTypeDataStruct = {
  "PrimaryToken",
  PrimaryToken_constructor,
  ptrof(primaryTokenFunctions),
  sizeof(struct private_tokenStruct),
  TOKEN_TYPE_DATA
};

PrimaryToken newPrimaryToken(int line, int column, PushbackStream stream, int cr, StringBuilder sb, StringElemInfoMap map) {
  PrimaryToken newInstance;

  ___BTPUSH;
  newInstance = PrimaryToken_constructor(PRIMARY_TOKEN_TYPE_DATA,
                                         ARGS_INT_INT_STREAM_INT_STRINGBUILDER_MAP,
                                         line, column, stream, cr, sb, map);
  ___BTPOP;
  return newInstance;
}

#define PrimaryToken struct private_tokenStruct*

/* There are two ways to call this constructor: the second argument, an int,
   tells what the remaining arguments are. One of the two macros
   ARGS_INT_INT_STREAM_INT_STRINGBUILDER_MAP or ARGS_STRING can be used for that
   purpose.
   1) void* PrimaryToken_constructor(TypeData, int, int, int, PushbackStream, int, StringBuilder)
      here, the second argument should be ARGS_INT_INT_STREAM_INT_STRINGBUILDER_MAP.
   2) void* PrimaryToken_constructor(TypeData, int, String)
      here, the second argument should be ARGS_STRING.          */
void* PrimaryToken_constructor(TypeData typeData, ...) {
  PrimaryToken newInstance;
  PushbackStream stream;
  StringElemInfoMap map;
  ElemInfo elemInfo;
  StringBuilder sb;
  va_list argp;
  int cr;

  ___BTPUSH;
  newInstance = Token_constructor(typeData);
  va_start(argp, typeData);
  if(va_arg(argp, int) == ARGS_INT_INT_STREAM_INT_STRINGBUILDER_MAP) {

    newInstance->line = va_arg(argp, int);
    newInstance->column = va_arg(argp, int);
    stream = va_arg(argp, void*);
    cr = va_arg(argp, int);
    sb = va_arg(argp, void*);
    map = va_arg(argp, void*);
#ifdef DEBUG_MODE
    if(sb->length != 0) {
      Errors.printMessageAndExit("The specified string builder hasn't been cleared");
    }
#endif

    while(IS_PRIMARY_CHAR(cr)) {
      sb->call->append(sb, (char)cr);
      cr = stream->call->read(stream);
    }
    stream->call->unread(stream, cr);

    elemInfo = map->call->get(map, (String)sb);
    if(elemInfo != NULL) {
      newInstance->element = elemInfo->element;
      newInstance->code = elemInfo->code;
      Token_setErrwarn(newInstance, elemInfo->errwarn);
    } else {
      newInstance->element = sb->call->toString(sb);
      newInstance->code = GVLCP_CODE_IDENTIFIER;
      Token_clearErrwarn(newInstance);
    }
    sb->call->clear(sb);

  } else {

    map = KeyWords.getReservedWords(); /* Might be risky... */
    newInstance->element = va_arg(argp, void*);
    newInstance->line = 0;
    newInstance->column = 0;
    elemInfo = map->call->get(map, newInstance->element);
    if(elemInfo != NULL) {
      newInstance->element->call->dump(newInstance->element);
      newInstance->element = elemInfo->element;
      newInstance->code = elemInfo->code;
      Token_setErrwarn(newInstance, elemInfo->errwarn);
    } else {
      cr = 0;
      newInstance->code = GVLCP_CODE_IDENTIFIER;
      while(IS_PRIMARY_CHAR(newInstance->element->buf[cr])) {
        ++cr;
      }
      if(cr < newInstance->element->length) {
        Token_setErrwarn(newInstance, GVLCP_ERROR_INVALID_TOKEN);
      } else {
        Token_clearErrwarn(newInstance);
      }
    }

  }
  va_end(argp);
  newInstance->length = newInstance->element->length;
  newInstance->value = 0;
  newInstance->code2 = 0;

  ___BTPOP;
  return newInstance;
}

static TypeData rtType(void) {
  return PRIMARY_TOKEN_TYPE_DATA;
}
