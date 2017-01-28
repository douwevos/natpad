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

#ifndef ____LINENUMBER_TOKEN_INCLUDED
#define ____LINENUMBER_TOKEN_INCLUDED

typedef struct linenumberTokenStruct* LinenumberToken;

#define LINENUMBER_TOKEN_TYPE_DATA ptrof(linenumberTokenTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/String.h>
#include <gvlib/StringBuilder.h>
#include <gvlib/TypeData.h>
#include <gvlib/io/PushbackStream.h>

struct linenumberTokenFunctionsStruct {
  void (* dump)(LinenumberToken this);
  gvboolean (* equals)(LinenumberToken this, Object other);
  int (* hashCode)(LinenumberToken this);
  void (* reserve)(LinenumberToken this);
  TypeData (* rtType)(void);
};

struct linenumberTokenStruct {
  readonly refCount_t refCount;
  const struct linenumberTokenFunctionsStruct* const call;
  const String element; /* Is null if not in GVLCP_TEST_MODE. */
  const uint64_t value;
  const int column;
  const int line;
  const int code;
  const int length;
  const int errwarn;
  int code2;
};

extern const struct typeDataStruct linenumberTokenTypeDataStruct;

LinenumberToken newLinenumberToken(int line, int column, PushbackStream stream, int cr,
                                   int code, StringBuilder clearedStringBuilder);

#endif
