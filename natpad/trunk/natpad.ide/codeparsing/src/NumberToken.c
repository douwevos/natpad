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
#include "../include/gvlcodeparsing/elements.h"
#include "../include/gvlcodeparsing/private/numberTokenPrivate.h"

DEFINE_PRIVATE_TOKEN_STRUCT(numberTokenFunctionsStruct)

static TypeData rtType(void);

static const struct numberTokenFunctionsStruct numberTokenFunctions = {
  (void*)Token_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType
};

const struct typeDataStruct numberTokenTypeDataStruct = {
  "NumberToken",
  NumberToken_constructor,
  ptrof(numberTokenFunctions),
  sizeof(struct private_tokenStruct),
  TOKEN_TYPE_DATA
};

NumberToken newNumberToken(int line, int column, PushbackStream stream, int cr, StringBuilder clearedStringBuilder) {
  NumberToken newInstance;

  ___BTPUSH;
  newInstance = NumberToken_constructor(NUMBER_TOKEN_TYPE_DATA,
                                        ARGS_INT_INT_STREAM_INT_STRINGBUILDER,
                                        line, column, stream, cr, clearedStringBuilder);
  ___BTPOP;
  return newInstance;
}

#define NumberToken struct private_tokenStruct*

#define IS_E(x) (x=='e' || x=='E')

#define IS_F(x) (x=='f' || x=='F')

#define IS_L(x) (x=='L' || x=='l')

#define IS_MINUS_OR_PLUS(x) (x=='-' || x=='+')

#define IS_U(x) (x=='U' || x=='u')

#define IS_X(x) (x=='x' || x=='X')

#define SET_ERROR_AND_WARNING_GENERAL_CASE(skipSuffices, warn, isFloat) \
  if(sOff < this->length) { \
    sOff = skipSuffices(sPtr, sOff); \
    if(sOff < this->length) { \
      Token_setErrwarn(this, isFloat ? GVLCP_ERROR_INVALID_SUFFIX_ON_FLOAT : GVLCP_ERROR_INVALID_SUFFIX_ON_INT); \
    } else if(warn) { \
      Token_setErrwarn(this, GVLCP_WARN_INTEGER_TOO_LARGE); \
    } \
  } else if(warn) { \
    Token_setErrwarn(this, GVLCP_WARN_INTEGER_TOO_LARGE); \
  }

static void setErrorWarningAndValue_dec(NumberToken this, const char* sPtr);
static void setErrorWarningAndValue_float(NumberToken this, const char* sPtr);
static void setErrorWarningAndValue_hex(NumberToken this, const char* sPtr);
static void setErrorWarningAndValue_oct(NumberToken this, const char* sPtr);
static int skipFloatSuffix(const char* sPtr, int sOff);
static int skipIntSuffices(const char* sPtr, int sOff);
static int skipZeros(const char* sPtr, int sOff);

static gvboolean isFloat(const byte* sPtr) {
  int sOff;

  if(sPtr[0] == '.') {
    return true;
  }
  sOff = 1;
  while(Char.isDigit(sPtr[sOff])) {
    ++sOff;
  }
  return sPtr[sOff]=='.' || IS_E(sPtr[sOff]);
}

/* There are two ways to call this constructor: the second argument, an int,
   tells what the remaining arguments are. One of the two macros
   ARGS_INT_INT_STREAM_INT_STRINGBUILDER or ARGS_STRING can be used for that
   purpose.
   1) void* NumberToken_constructor(TypeData, int, int, int, PushbackStream, int, StringBuilder)
      here, the second argument should be ARGS_INT_INT_STREAM_INT_STRINGBUILDER.
   2) void* NumberToken_constructor(TypeData, int, String)
      here, the second argument should be ARGS_STRING.          */
void* NumberToken_constructor(TypeData typeData, ...) {
  NumberToken newInstance;
  gvboolean prev_0, prev_e;
  PushbackStream stream;
  const char* strBuf;
  StringBuilder sb;
  va_list argp;
  int argSpecifier, cr;

  ___BTPUSH;
  newInstance = Token_constructor(typeData);
  va_start(argp, typeData);
  argSpecifier = va_arg(argp, int);
  if(argSpecifier == ARGS_INT_INT_STREAM_INT_STRINGBUILDER) {

    newInstance->line = va_arg(argp, int);
    newInstance->column = va_arg(argp, int);
    stream = va_arg(argp, void*);
    cr = va_arg(argp, int);
    sb = va_arg(argp, void*);
#ifdef DEBUG_MODE
    if(sb->length != 0) {
      Errors.printMessageAndExit("The specified string builder hasn't been cleared");
    }
#endif

    prev_e = false;

#ifndef GCC_PARSING_MODE
    prev_0 = cr == '0';
    sb->call->append(sb, (char)cr);
    cr = stream->call->read(stream);
    if(prev_0 && IS_X(cr)) {

      while(IS_PRIMARY_CHAR(cr) || cr=='.') {
        sb->call->append(sb, (char)cr);
        cr = stream->call->read(stream);
      }

    } else {
#endif

      while(IS_PRIMARY_CHAR(cr) || cr=='.' || (prev_e && IS_MINUS_OR_PLUS(cr))) {
        prev_e = IS_E(cr);
        sb->call->append(sb, (char)cr);
        cr = stream->call->read(stream);
      }

#ifndef GCC_PARSING_MODE
    }
#endif

    stream->call->unread(stream, cr);
    if(sb->call->equals(sb, (Object)elem_30)) {
      newInstance->element = elem_30;
    } else if(sb->call->equals(sb, (Object)elem_31)) {
      newInstance->element = elem_31;
    } else {
      newInstance->element = sb->call->toString(sb);
    }
    sb->call->clear(sb);

  } else {

    newInstance->line = 0;
    newInstance->column = 0;
    newInstance->element = va_arg(argp, void*);

  }
  va_end(argp);
  newInstance->length = newInstance->element->length;

  newInstance->code2 = 0;
  newInstance->value = 0;
  Token_clearErrwarn(newInstance);
  newInstance->code = GVLCP_CODE_NUMBER;
  strBuf = newInstance->element->buf;
  if(strBuf[0]=='0' && IS_X(strBuf[1])) {

    newInstance->code |= GVLCP_CODE_INTEGER;
    setErrorWarningAndValue_hex(newInstance, strBuf);

  } else if(isFloat((const byte *) strBuf)) {

    newInstance->code |= GVLCP_CODE_FLOATING_POINT;
    setErrorWarningAndValue_float(newInstance, strBuf);

  } else if(strBuf[0] != '0') {

    newInstance->code |= GVLCP_CODE_INTEGER;
    setErrorWarningAndValue_dec(newInstance, strBuf);

  } else {

    newInstance->code |= GVLCP_CODE_INTEGER;
    setErrorWarningAndValue_oct(newInstance, strBuf);

  }

  ___BTPOP;
  return newInstance;
}

static TypeData rtType(void) {
  return NUMBER_TOKEN_TYPE_DATA;
}

static void setErrorWarningAndValue_dec(NumberToken this, const char* sPtr) {
	gvboolean firstDigitIsOne, warn;
  int len, sOff;
  char cr;

  len = 0;
  sOff = 0;
  cr = sPtr[sOff];
  firstDigitIsOne = cr == '1';
  while(Char.isDigit(cr)) {
    this->value *= 10;
    this->value += Char.valueOf(cr);
    ++sOff;
    ++len;
    cr = sPtr[sOff];
  }
  if(len==20 && firstDigitIsOne) {
    warn = (this->value & 0x8000000000000000) == 0;
  } else {
    warn = len > 19;
  }
  SET_ERROR_AND_WARNING_GENERAL_CASE(skipIntSuffices, warn, false);
}

static void setErrorWarningAndValue_float(NumberToken this, const char* sPtr) {
  int sOff;

  sOff = 0;
  while(Char.isDigit(sPtr[sOff])) {
    ++sOff;
  }
  if(sPtr[sOff] == '.') {
    ++sOff;
    while(Char.isDigit(sPtr[sOff])) {
      ++sOff;
    }
  }
  if(IS_E(sPtr[sOff])) {
    ++sOff;
    if(IS_MINUS_OR_PLUS(sPtr[sOff])) {
      ++sOff;
    }
    if(Char.isDigit(sPtr[sOff])) {
      while(Char.isDigit(sPtr[sOff])) {
        ++sOff;
      }
    } else {
      Token_setErrwarn(this, GVLCP_ERROR_EXPONENT_HAS_NO_DIGITS);
    }
  }
  if(Token_getErrwarn(this) == 0) {
    SET_ERROR_AND_WARNING_GENERAL_CASE(skipFloatSuffix, false, true);
  }
}

static void setErrorWarningAndValue_hex(NumberToken this, const char* sPtr) {
  int len, sOff;
  gvboolean warn;
  char cr;

  len = 0;
  sOff = skipZeros(sPtr, 2);
  cr = sPtr[sOff];
  while(Char.isHexDigit(cr)) {
    this->value <<= 4;
    this->value |= Char.valueOf(cr);
    ++sOff;
    ++len;
    cr = sPtr[sOff];
  }
  if(sOff > 2) {
    warn = len > 16;
    SET_ERROR_AND_WARNING_GENERAL_CASE(skipIntSuffices, warn, false);
  } else {
    Token_setErrwarn(this, GVLCP_ERROR_INVALID_SUFFIX_ON_INT);
  }
}

static void setErrorWarningAndValue_oct(NumberToken this, const char* sPtr) {
  int len, sOff;
  gvboolean warn;
  char cr;

  if(this->length > 1) {

    len = 0;
    sOff = skipZeros(sPtr, 1);
    if(sPtr[sOff] == 0x31) {
      this->value = 1;
      ++sOff;
    }
    cr = sPtr[sOff];
    while(Char.isOctDigit(cr)) {
      this->value <<= 3;
      this->value |= Char.valueOf(cr);
      ++sOff;
      ++len;
      cr = sPtr[sOff];
    }
    if(!Char.isDigit(cr)) {
      warn = len > 21;
      SET_ERROR_AND_WARNING_GENERAL_CASE(skipIntSuffices, warn, false);
    } else {
      Token_setErrwarn(this, GVLCP_ERROR_INVALID_DIGIT_IN_OCTAL);
    }

  }
}

static int skipFloatSuffix(const char* sPtr, int sOff) {
  if(IS_F(sPtr[sOff])) {/* F */
    ++sOff;
  } else if(IS_L(sPtr[sOff])) {/* L */
    ++sOff;
  }
  return sOff;
}

static int skipIntSuffices(const char* sPtr, int sOff) {
  if(IS_L(sPtr[sOff])) {/* L */

    ++sOff;
    if(IS_L(sPtr[sOff])) {/* LL */
      ++sOff;
      if(IS_U(sPtr[sOff])) {/* LLU */
        ++sOff;
      }
    } else if(IS_U(sPtr[sOff])) { /* LU */
      ++sOff;
    }

  } else if(IS_U(sPtr[sOff])) {/* U */

    ++sOff;
    if(IS_L(sPtr[sOff])) {/* UL */
      ++sOff;
      if(IS_L(sPtr[sOff])) {/* ULL */
        ++sOff;
      }
    }

  }
  return sOff;
}

static int skipZeros(const char* sPtr, int sOff) {
  while(sPtr[sOff] == 0x30) {
    ++sOff;
  }
  return sOff;
}
