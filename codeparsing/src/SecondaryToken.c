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
#include <gvlib/StringBuilder.h>
#include "../include/gvlcodeparsing/codes.h"
#include "../include/gvlcodeparsing/elements.h"
#include "../include/gvlcodeparsing/SecondaryToken.h"
#include "../include/gvlcodeparsing/private/tokenPrivate.h"

DEFINE_PRIVATE_TOKEN_STRUCT(secondaryTokenFunctionsStruct)

static void* constructor(TypeData typeData, ...);
static TypeData rtType(void);

static const struct secondaryTokenFunctionsStruct secondaryTokenFunctions = {
  (void*)Token_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType
};

const struct typeDataStruct secondaryTokenTypeDataStruct = {
  "SecondaryToken",
  constructor,
  ptrof(secondaryTokenFunctions),
  sizeof(struct private_tokenStruct),
  TOKEN_TYPE_DATA
};

SecondaryToken newSecondaryToken(int line, int column, PushbackStream stream, int cr) {
  SecondaryToken newInstance;

  ___BTPUSH;
  newInstance = constructor(SECONDARY_TOKEN_TYPE_DATA, line, column, stream, cr);
  ___BTPOP;
  return newInstance;
}

#define SecondaryToken struct private_tokenStruct*

static void* constructor(TypeData typeData, ...) {
  SecondaryToken newInstance;
  PushbackStream stream;
  StringBuilder sb;
  int64_t utf8code;
  va_list argp;
  int cr;

  ___BTPUSH;
  newInstance = Token_constructor(typeData);
  va_start(argp, typeData);
  newInstance->line = va_arg(argp, int);
  newInstance->column = va_arg(argp, int);
  stream = va_arg(argp, void*);
  cr = va_arg(argp, int);
  va_end(argp);

  switch(cr) {

  case 0x21: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x3D: {
      newInstance->element = elem_213D;
      newInstance->code = GVLCP_CODE_COMPARISON;
    } break;

    default: {
      newInstance->element = elem_21;
      newInstance->code = GVLCP_CODE_ARITHMIC_PRE;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x23: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x23: {
      newInstance->element = elem_2323;
      newInstance->code = GVLCP_CODE_TWO_OPERANDS;
    } break;

    default: {
      newInstance->element = elem_23;
      newInstance->code = 0;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x25: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x3D: {
      newInstance->element = elem_253D;
      newInstance->code = GVLCP_CODE_ASSIGNMENT;
    } break;

    default: {
      newInstance->element = elem_25;
      newInstance->code = GVLCP_CODE_ARITHMIC_IN;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x26: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x26: {
      newInstance->element = elem_2626;
      newInstance->code = GVLCP_CODE_ARITHMIC_IN;
    } break;

    case 0x3D: {
      newInstance->element = elem_263D;
      newInstance->code = GVLCP_CODE_ASSIGNMENT;
    } break;

    default: {
      newInstance->element = elem_26;
      newInstance->code = GVLCP_CODE_ARITHMIC_IN | GVLCP_CODE_POINTER_REF;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x28: {
    newInstance->element = elem_28;
    newInstance->code = GVLCP_CODE_PARENTHESIS_OPEN;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x29: {
    newInstance->element = elem_29;
    newInstance->code = GVLCP_CODE_PARENTHESIS_CLOSE;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x2A: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x3D: {
      newInstance->element = elem_2A3D;
      newInstance->code = GVLCP_CODE_ASSIGNMENT;
    } break;

    default: {
      newInstance->element = elem_2A;
      newInstance->code = GVLCP_CODE_ASTERISK;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x2B: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x2B: {
      newInstance->element = elem_2B2B;
      newInstance->code = GVLCP_CODE_ARITHMIC_PRE_OR_POST;
    } break;

    case 0x3D: {
      newInstance->element = elem_2B3D;
      newInstance->code = GVLCP_CODE_ASSIGNMENT;
    } break;

    default: {
      newInstance->element = elem_2B;
      newInstance->code = GVLCP_CODE_ARITHMIC_IN;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x2C: {
    newInstance->element = elem_2C;
    newInstance->code = GVLCP_CODE_COMMA;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x2D: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x2D: {
      newInstance->element = elem_2D2D;
      newInstance->code = GVLCP_CODE_ARITHMIC_PRE_OR_POST;
    } break;

    case 0x3D: {
      newInstance->element = elem_2D3D;
      newInstance->code = GVLCP_CODE_ASSIGNMENT;
    } break;

    case 0x3E: {
      newInstance->element = elem_2D3E;
      newInstance->code = GVLCP_CODE_STRUCT_RELATED | GVLCP_CODE_POINTER_RELATED;
    } break;

    default: {
      newInstance->element = elem_2D;
      newInstance->code = GVLCP_CODE_ARITHMIC_IN;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x2E: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x2E: {
      cr = stream->call->read(stream);
      if(cr == 0x2E) {
        newInstance->element = elem_2E2E2E;
        newInstance->code = GVLCP_CODE_ELLIPSIS;
        Token_clearErrwarn(newInstance);
      } else {
        newInstance->code = 0;
        newInstance->element = elem_2E2E;
        Token_setErrwarn(newInstance, GVLCP_ERROR_INVALID_TOKEN);
        stream->call->unread(stream, cr);
      }
    } break;

    default: {
      newInstance->element = elem_2E;
      newInstance->code = GVLCP_CODE_STRUCT_RELATED;
      Token_clearErrwarn(newInstance);
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
  } break;

  case 0x2F: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x3D: {
      newInstance->element = elem_2F3D;
      newInstance->code = GVLCP_CODE_ASSIGNMENT;
    } break;

    default: {
      newInstance->element = elem_2F;
      newInstance->code = GVLCP_CODE_ARITHMIC_IN;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x3A: {
    newInstance->element = elem_3A;
    newInstance->code = GVLCP_CODE_COLON;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x3B: {
    newInstance->element = elem_3B;
    newInstance->code = GVLCP_CODE_SEMICOLON;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x3C: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x3C: {
      cr = stream->call->read(stream);
      if(cr == 0x3D) {
        newInstance->element = elem_3C3C3D;
        newInstance->code = GVLCP_CODE_ASSIGNMENT;
      } else {
        newInstance->element = elem_3C3C;
        newInstance->code = GVLCP_CODE_ARITHMIC_IN;
        stream->call->unread(stream, cr);
      }
    } break;

    case 0x3D: {
      newInstance->element = elem_3C3D;
      newInstance->code = GVLCP_CODE_COMPARISON;
    } break;

    default: {
      newInstance->element = elem_3C;
      newInstance->code = GVLCP_CODE_COMPARISON;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x3D: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x3D: {
      newInstance->element = elem_3D3D;
      newInstance->code = GVLCP_CODE_COMPARISON;
    } break;

    default: {
      newInstance->element = elem_3D;
      newInstance->code = GVLCP_CODE_ASSIGNMENT;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x3E: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x3E: {
      cr = stream->call->read(stream);
      if(cr == 0x3D) {
        newInstance->element = elem_3E3E3D;
        newInstance->code = GVLCP_CODE_ASSIGNMENT;
      } else {
        newInstance->element = elem_3E3E;
        newInstance->code = GVLCP_CODE_ARITHMIC_IN;
        stream->call->unread(stream, cr);
      }
    } break;

    case 0x3D: {
      newInstance->element = elem_3E3D;
      newInstance->code = GVLCP_CODE_COMPARISON;
    } break;

    default: {
      newInstance->element = elem_3E;
      newInstance->code = GVLCP_CODE_COMPARISON;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x3F: {
    newInstance->element = elem_3F;
    newInstance->code = GVLCP_CODE_QUESTION_MARK;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x5B: {
    newInstance->element = elem_5B;
    newInstance->code = GVLCP_CODE_BRACKET_OPEN;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x5C: {
    newInstance->element = elem_5C;
    newInstance->code = GVLCP_CODE_BACKSLASH;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x5D: {
    newInstance->element = elem_5D;
    newInstance->code = GVLCP_CODE_BRACKET_CLOSE;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x5E: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x3D: {
      newInstance->element = elem_5E3D;
      newInstance->code = GVLCP_CODE_ASSIGNMENT;
    } break;

    default: {
      newInstance->element = elem_5E;
      newInstance->code = GVLCP_CODE_ARITHMIC_IN;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x7B: {
    newInstance->element = elem_7B;
    newInstance->code = GVLCP_CODE_BRACE_OPEN;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x7C: {
    cr = stream->call->read(stream);
    switch(cr) {

    case 0x3D: {
      newInstance->element = elem_7C3D;
      newInstance->code = GVLCP_CODE_ASSIGNMENT;
    } break;

    case 0x7C: {
      newInstance->element = elem_7C7C;
      newInstance->code = GVLCP_CODE_ARITHMIC_IN;
    } break;

    default: {
      newInstance->element = elem_7C;
      newInstance->code = GVLCP_CODE_ARITHMIC_IN;
      stream->call->unread(stream, cr);
    } break;

    }
    newInstance->length = newInstance->element->length;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x7D: {
    newInstance->element = elem_7D;
    newInstance->code = GVLCP_CODE_BRACE_CLOSE;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  case 0x7E: {
    newInstance->element = elem_7E;
    newInstance->code = GVLCP_CODE_ARITHMIC_PRE;
    newInstance->length = 1;
    Token_clearErrwarn(newInstance);
  } break;

  default: {
    sb = newStringBuilder(0);
    utf8code = Char.toUtf8(cr);
    while(utf8code > 0) {
      sb->call->append(sb, (char)utf8code);
      utf8code >>= 8;
    }
    newInstance->element = sb->call->toString(sb);
    sb->call->dump(sb);
    newInstance->code = 0;
    newInstance->length = 1;
    Token_setErrwarn(newInstance, GVLCP_ERROR_INVALID_TOKEN);
  } break;

  }
  newInstance->code2 = 0;
  newInstance->value = 0;
  ___BTPOP;
  return newInstance;
}

static TypeData rtType(void) {
  return SECONDARY_TOKEN_TYPE_DATA;
}
