/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

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

#ifndef ___TOKEN_PRIVATE_INCLUDED
#define ___TOKEN_PRIVATE_INCLUDED

#include <gvlib/char.h>
#include <gvlib/private/objectPrivate.h>
#include "../Token.h"

#define DEFINE_PRIVATE_TOKEN_STRUCT(functionsStructIdentifier) \
  struct private_tokenStruct { \
    readonly refCount_t refCount; \
    const struct functionsStructIdentifier* const call; \
    String element; \
    uint64_t value; \
    int column; \
    int line; \
    int code; \
    int length; \
    int errwarn; \
    int code2; \
  };

#define IS_PRIMARY_CHAR(x) (Char.isAsciiLetter(x) || Char.isDigit(x) || x=='_' || x=='$')

#ifdef GVLCP_TEST_MODE

# define DECLARE_STRING_BUILDER(name) \
  StringBuilder name; \
  int64_t utf8code;

# define GET_STRING_BUILDER(sb) sb = va_arg(argp, void*)

# define SB_APPEND_CHAR(sb, cr) sb->call->append(sb, (char)cr)

# define SB_APPEND_WIDE_CHAR(sb, cr) \
  utf8code = Char.toUtf8(cr); \
  while(utf8code > 0) { \
    sb->call->append(sb, (char)utf8code); \
    utf8code >>= 8; \
  }

# define SET_ELEMENT(this, sb) this->element = sb->call->toString(sb); sb->call->clear(sb)

#else

# define DECLARE_STRING_BUILDER(name)
# define GET_STRING_BUILDER(sb)
# define SB_APPEND_CHAR(sb, cr)
# define SB_APPEND_WIDE_CHAR(sb, cr)
# define SET_ELEMENT(this, sb) this->element = NULL

#endif

#define Token_clearErrwarn(this) this->errwarn = 0
#define Token_constructor Object_constructor
#define Token_setErrwarn(this, errwarnCode) this->errwarn = errwarnCode

void Token_dump(Token this);

#endif
