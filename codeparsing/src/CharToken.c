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
#include "../include/gvlcodeparsing/CharToken.h"
#include "../include/gvlcodeparsing/private/tokenPrivate.h"

DEFINE_PRIVATE_TOKEN_STRUCT(charTokenFunctionsStruct)

static void* constructor(TypeData typeData, ...);
/* getAdditionalLength also sets errors/warnings. */
static int getAdditionalLength(struct private_tokenStruct* this, PushbackStream stream, int cr, int errcode, ...);
static int64_t getValueAsUtf8(int val);
static TypeData rtType(void);
static void setErrwarn(struct private_tokenStruct* this, gvboolean wide, int warncode);

static const struct charTokenFunctionsStruct charTokenFunctions = {
  (void*)Token_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType
};

const struct typeDataStruct charTokenTypeDataStruct = {
  "CharToken",
  constructor,
  ptrof(charTokenFunctions),
  sizeof(struct private_tokenStruct),
  TOKEN_TYPE_DATA
};

CharToken newCharToken(int line, int column, PushbackStream stream, gvboolean wide, StringBuilder clearedStringBuilder) {
  CharToken newInstance;

  ___BTPUSH;
#ifdef DEBUG_MODE
  if(clearedStringBuilder->length != 0) {
    Errors.printMessageAndExit("The specified string builder hasn't been cleared");
  }
  if(wide < 0  ||  wide > 1) {
    Errors.printMessageAndExit("Parameter 'wide' not boolean");
  }
#endif
#ifdef GVLCP_TEST_MODE
  if(wide) {
    clearedStringBuilder->call->append(clearedStringBuilder, 'L');
  }
  clearedStringBuilder->call->append(clearedStringBuilder, '\'');
  newInstance = constructor(CHAR_TOKEN_TYPE_DATA, line, column, stream, (int)wide, clearedStringBuilder);
#else
  newInstance = constructor(CHAR_TOKEN_TYPE_DATA, line, column, stream, (int)wide);
#endif
  ___BTPOP;
  return newInstance;
}

#define CharToken struct private_tokenStruct*

#ifndef GVLCP_TEST_MODE
# define getAdditionalLength(this, stream, cr, errcode, sb) getAdditionalLength(this, stream, cr, errcode)
#endif

static void* constructor(TypeData typeData, ...) {
  CharToken newInstance;
  PushbackStream stream;
  gvboolean warn, wide;
  va_list argp;
  int cr, len;
  DECLARE_STRING_BUILDER(sb);

  ___BTPUSH;
  newInstance = Token_constructor(typeData);
  va_start(argp, typeData);
  newInstance->line = va_arg(argp, int);
  newInstance->column = va_arg(argp, int);
  newInstance->code = GVLCP_CODE_NUMBER | GVLCP_CODE_INTEGER;
  newInstance->value = 0;
  stream = va_arg(argp, void*);
  wide = va_arg(argp, int);
  GET_STRING_BUILDER(sb);
  va_end(argp);

  len = (int)wide + 1;
  cr = stream->call->read(stream);
  if(cr == '\\') { /* Escape sequence character. */

    ++len;
    SB_APPEND_CHAR(sb, cr);
    cr = stream->call->read(stream);
    if(cr==-1 || cr==0x0A) {

      Token_setErrwarn(newInstance, GVLCP_ERROR_MISSING_TERMINATING_CHARACTER);
      stream->call->unread(stream, cr);

    } else if(cr == 'x') {

      ++len;
      SB_APPEND_CHAR(sb, cr);
      cr = stream->call->read(stream);
      while(Char.isHexDigit(cr)) {
        newInstance->value <<= 4;
        newInstance->value |= Char.valueOf(cr);
        ++len;
        SB_APPEND_CHAR(sb, cr);
        cr = stream->call->read(stream);
      }
      ++len;
      if(cr == '\'') {
        SB_APPEND_CHAR(sb, cr);
        if(len > 4 + wide) {
          setErrwarn(newInstance, wide, GVLCP_WARN_ESCAPE_SEQUENCE_OUT_OF_RANGE);
        } else {
          Token_setErrwarn(newInstance, GVLCP_ERROR_ESCAPE_SEQUENCE_NO_HEX_DIGITS);
        }
      } else if(len > 4 + wide) {
        len += getAdditionalLength(newInstance, stream, cr, 0, sb);
      } else {
        len += getAdditionalLength(newInstance, stream, cr, GVLCP_ERROR_ESCAPE_SEQUENCE_NO_HEX_DIGITS, sb);
      }

    } else if(Char.isOctDigit(cr)) {

      while(Char.isOctDigit(cr)) {
        newInstance->value <<= 3;
        newInstance->value |= Char.valueOf(cr);
        ++len;
        SB_APPEND_CHAR(sb, cr);
        cr = stream->call->read(stream);
      }
      ++len;
      if(cr == '\'') {
        SB_APPEND_CHAR(sb, cr);
        setErrwarn(newInstance, wide, GVLCP_WARN_ESCAPE_SEQUENCE_OUT_OF_RANGE);
      } else {
        len += getAdditionalLength(newInstance, stream, cr, 0, sb);
      }

    } else {

      ++len;
      SB_APPEND_WIDE_CHAR(sb, cr);
      switch(cr) {

      case '"': {
        warn = false;
        newInstance->value = cr;
      } break;

      case '\'': {
        warn = false;
        newInstance->value = cr;
      } break;

      case '?': {
        warn = false;
        newInstance->value = cr;
      } break;

      case '\\': {
        warn = false;
        newInstance->value = cr;
      } break;

      case 'a': {
        warn = false;
        newInstance->value = '\a';
      } break;

      case 'b': {
        warn = false;
        newInstance->value = '\b';
      } break;

      case 'f': {
        warn = false;
        newInstance->value = '\f';
      } break;

      case 'n': {
        warn = false;
        newInstance->value = '\n';
      } break;

      case 'r': {
        warn = false;
        newInstance->value = '\r';
      } break;

      case 't': {
        warn = false;
        newInstance->value = '\t';
      } break;

      case 'v': {
        warn = false;
        newInstance->value = '\v';
      } break;

      default: {
        warn = true;
        newInstance->value = cr;
      } break;

      }
      ++len;
      cr = stream->call->read(stream);
      if(cr == '\'') {
        SB_APPEND_CHAR(sb, cr);
        if(warn) {
          Token_setErrwarn(newInstance, GVLCP_WARN_UNKNOWN_ESCAPE_SEQUENCE);
        } else {
          Token_clearErrwarn(newInstance);
        }
      } else {
        len += getAdditionalLength(newInstance, stream, cr, 0, sb);
      }

    }

  } else if(!(cr==-1 || cr==0x0A || cr=='\'')) { /* Ordinary character. */

    len += 2;
    SB_APPEND_WIDE_CHAR(sb, cr);
    if(!wide  &&  cr > 0x7F) {
      newInstance->value = getValueAsUtf8(cr);
    } else {
      newInstance->value = cr;
    }
    cr = stream->call->read(stream);
    if(cr == '\'') {
      SB_APPEND_CHAR(sb, cr);
      setErrwarn(newInstance, wide, GVLCP_WARN_MULTICHARACTER_CHARACTER_CONSTANT);
    } else {
      len += getAdditionalLength(newInstance, stream, cr, 0, sb);
    }

  } else if(cr == '\'') { /* Empty character. */

    ++len;
    SB_APPEND_CHAR(sb, cr);
    Token_setErrwarn(newInstance, GVLCP_ERROR_EMPTY_CHARACTER_CONSTANT);

  } else { /* Just ' */

    Token_setErrwarn(newInstance, GVLCP_ERROR_MISSING_TERMINATING_CHARACTER);
    stream->call->unread(stream, cr);

  }

  SET_ELEMENT(newInstance, sb);
  newInstance->length = len;
  newInstance->code2 = 0;
  ___BTPOP;
  return newInstance;
}

#ifndef GVLCP_TEST_MODE
# undef getAdditionalLength
#endif

static int getAdditionalLength(CharToken this, PushbackStream stream, int cr, int errcode, ...) {
	gvboolean bsl;
  int result;
#ifdef GVLCP_TEST_MODE
  va_list argp;
  DECLARE_STRING_BUILDER(sb);
#endif

  ___BTPUSH;
#ifdef GVLCP_TEST_MODE
  va_start(argp, errcode);
  GET_STRING_BUILDER(sb);
  va_end(argp);
#endif
  result = 0;
  bsl = false;
  while(!(cr==0 || cr==0x0A || (cr=='\'' && !bsl))) {
    bsl = cr == '\\';
    SB_APPEND_WIDE_CHAR(sb, cr);
    cr = stream->call->read(stream);
    ++result;
  }
  if(cr != '\'') {
    Token_setErrwarn(this, GVLCP_ERROR_MISSING_TERMINATING_CHARACTER);
    stream->call->unread(stream, cr);
    --result;
  } else {
    if(errcode == 0) {
      Token_setErrwarn(this, GVLCP_WARN_MULTICHARACTER_CHARACTER_CONSTANT);
    } else {
      Token_setErrwarn(this, errcode);
    }
    SB_APPEND_CHAR(sb, cr);
  }
  ___BTPOP;
  return result;
}

static int64_t getValueAsUtf8(int val) {
  int64_t result, utf8code;

  ___BTPUSH;
  result = 0;
  utf8code = Char.toUtf8(val);
  while(utf8code > 0) {
    result <<= 8;
    result |= utf8code & 0xFF;
    utf8code >>= 8;
  }
  ___BTPOP;
  return result;
}

static TypeData rtType(void) {
  return CHAR_TOKEN_TYPE_DATA;
}

static void setErrwarn(CharToken this, gvboolean wide, int warncode) {
  ___BTPUSH;
  if(wide) {
    if(this->value > 0x7FFFFFFF) {
      Token_setErrwarn(this, warncode);
    } else {
      Token_clearErrwarn(this);
    }
  } else if(this->value > 0x7F) {
    Token_setErrwarn(this, warncode);
  } else {
    Token_clearErrwarn(this);
  }
  ___BTPOP;
}
