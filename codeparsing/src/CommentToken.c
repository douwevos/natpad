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
#include "../include/gvlcodeparsing/CommentToken.h"
#include "../include/gvlcodeparsing/private/tokenPrivate.h"
#ifdef GVLCP_TEST_MODE
# include <gvlib/StringBuilder.h>
#endif

#undef newCommentToken

DEFINE_PRIVATE_TOKEN_STRUCT(commentTokenFunctionsStruct)

#define BLOCK_COMMENT_TOKEN     1
#define CONTINUED_COMMENT_TOKEN 2
#define LINE_COMMENT_TOKEN      3

static void* constructor(TypeData typeData, ...);
static TypeData rtType(void);

static const struct commentTokenFunctionsStruct commentTokenFunctions = {
  (void*)Token_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType
};

const struct typeDataStruct commentTokenTypeDataStruct = {
  "CommentToken",
  constructor,
  ptrof(commentTokenFunctions),
  sizeof(struct private_tokenStruct),
  TOKEN_TYPE_DATA
};

CommentToken newCommentToken(int line, int column, PushbackStream stream, ...) {
  CommentToken newInstance;
  gvboolean* openPtr;
  va_list argp;
#ifdef GVLCP_TEST_MODE
  StringBuilder sb;
#endif

  ___BTPUSH;
  va_start(argp, stream);
#ifdef GVLCP_TEST_MODE
  sb = va_arg(argp, void*);
# ifdef DEBUG_MODE
  if(sb->length != 0) {
    Errors.printMessageAndExit("The specified string builder hasn't been cleared");
  }
# endif
#endif
  openPtr = va_arg(argp, void*);
  va_end(argp);
  if(openPtr == NULL) {

#ifdef GVLCP_TEST_MODE
    sb->call->appendRange(sb, "//");
    newInstance = constructor(COMMENT_TOKEN_TYPE_DATA, line, column, stream, LINE_COMMENT_TOKEN, sb);
#else
    newInstance = constructor(COMMENT_TOKEN_TYPE_DATA, line, column, stream, LINE_COMMENT_TOKEN);
#endif

  } else if(openPtr[0]) {

#ifdef GVLCP_TEST_MODE
    newInstance = constructor(COMMENT_TOKEN_TYPE_DATA, line, column, stream, CONTINUED_COMMENT_TOKEN, openPtr, sb);
#else
    newInstance = constructor(COMMENT_TOKEN_TYPE_DATA, line, column, stream, CONTINUED_COMMENT_TOKEN, openPtr);
#endif

  } else {

#ifdef GVLCP_TEST_MODE
    sb->call->appendRange(sb, "/*");
    newInstance = constructor(COMMENT_TOKEN_TYPE_DATA, line, column, stream, BLOCK_COMMENT_TOKEN, openPtr, sb);
#else
    newInstance = constructor(COMMENT_TOKEN_TYPE_DATA, line, column, stream, BLOCK_COMMENT_TOKEN, openPtr);
#endif

  }
  ___BTPOP;
  return newInstance;
}

#define CommentToken struct private_tokenStruct*

#define PARSE_BLOCK_COMMENT_TOKEN() \
  Token_clearErrwarn(newInstance); \
  openPtr = va_arg(argp, void*); \
  GET_STRING_BUILDER(sb); \
  done = false; \
  asterisk = false; \
  c = stream->call->read(stream); \
  while(!(c == -1  ||  c == 0x0A  ||  done)) { \
    if(c == '*') { \
      asterisk = true; \
    } else { \
      done = asterisk  &&  c == '/'; \
      asterisk = false; \
    } \
    SB_APPEND_WIDE_CHAR(sb, c); \
    c = stream->call->read(stream); \
    ++len; \
  } \
  stream->call->unread(stream, c); \
  openPtr[0] = !done;

static void* constructor(TypeData typeData, ...) {
  CommentToken newInstance;
  gvboolean asterisk, done;
  PushbackStream stream;
  gvboolean* openPtr;
  va_list argp;
  int c, len;
  len=0;
  DECLARE_STRING_BUILDER(sb);

  ___BTPUSH;
  newInstance = Token_constructor(typeData);
  va_start(argp, typeData);
  newInstance->line = va_arg(argp, int);
  newInstance->column = va_arg(argp, int);
  newInstance->code = GVLCP_CODE_COMMENT;
  stream = va_arg(argp, void*);
  c = va_arg(argp, int);
  switch(c) {

  case BLOCK_COMMENT_TOKEN: {
    len = 2;
    PARSE_BLOCK_COMMENT_TOKEN();
  } break;

  case CONTINUED_COMMENT_TOKEN: {
    len = 0;
    PARSE_BLOCK_COMMENT_TOKEN();
  } break;

  case LINE_COMMENT_TOKEN: {
    Token_setErrwarn(newInstance, GVLCP_WARN_NOT_ANSI);
    GET_STRING_BUILDER(sb);
    len = 2;
    c = stream->call->read(stream);
    while(!(c == -1  ||  c == 0x0A)) {
      SB_APPEND_WIDE_CHAR(sb, c);
      c = stream->call->read(stream);
      ++len;
    }
    stream->call->unread(stream, c);
  } break;

#ifdef DEBUG_MODE
  default: {
    Errors.printMessageAndExit("No such case");
  } break;
#endif

  }
  SET_ELEMENT(newInstance, sb);
  newInstance->length = len;
  newInstance->code2 = 0;
  newInstance->value = 0;
  va_end(argp);
  ___BTPOP;
  return newInstance;
}

static TypeData rtType(void) {
  return COMMENT_TOKEN_TYPE_DATA;
}
