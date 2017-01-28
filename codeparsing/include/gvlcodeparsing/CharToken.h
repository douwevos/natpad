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

#ifndef ____CHAR_TOKEN_INCLUDED
#define ____CHAR_TOKEN_INCLUDED

typedef struct charTokenStruct* CharToken;

#define CHAR_TOKEN_TYPE_DATA ptrof(charTokenTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/String.h>
#include <gvlib/StringBuilder.h>
#include <gvlib/TypeData.h>
#include <gvlib/io/PushbackStream.h>

struct charTokenFunctionsStruct {
  void (* dump)(CharToken this);
  gvboolean (* equals)(CharToken this, Object other);
  int (* hashCode)(CharToken this);
  void (* reserve)(CharToken this);
  TypeData (* rtType)(void);
};

struct charTokenStruct {
  readonly refCount_t refCount;
  const struct charTokenFunctionsStruct* const call;
  const String element; /* Is null if not in GVLCP_TEST_MODE. */
  const uint64_t value;
  const int column;
  const int line;
  const int code;
  const int length;
  const int errwarn;
  int code2;
};

extern const struct typeDataStruct charTokenTypeDataStruct;

CharToken newCharToken(int line, int column, PushbackStream stream, gvboolean wide, StringBuilder clearedStringBuilder);

#endif
