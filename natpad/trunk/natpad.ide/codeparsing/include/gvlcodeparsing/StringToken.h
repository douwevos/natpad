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

#ifndef ____STRING_TOKEN_INCLUDED
#define ____STRING_TOKEN_INCLUDED

typedef struct stringTokenStruct* StringToken;

#define STRING_TOKEN_TYPE_DATA ptrof(stringTokenTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/String.h>
#include <gvlib/StringBuilder.h>
#include <gvlib/TypeData.h>
#include <gvlib/io/PushbackStream.h>

struct stringTokenFunctionsStruct {
  void (* dump)(StringToken this);
  gvboolean (* equals)(StringToken this, Object other);
  int (* hashCode)(StringToken this);
  void (* reserve)(StringToken this);
  TypeData (* rtType)(void);
};

struct stringTokenStruct {
  readonly refCount_t refCount;
  const struct stringTokenFunctionsStruct* const call;
  const String element; /* Is null if not in GVLCP_TEST_MODE. */
  const uint64_t value;
  const int column;
  const int line;
  const int code;
  const int length;
  const int errwarn;
  int code2;
};

extern const struct typeDataStruct stringTokenTypeDataStruct;

StringToken newStringToken(int line, int column, PushbackStream stream, StringBuilder clearedStringBuilder);

#endif
