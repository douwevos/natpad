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

#ifndef ____INCLUDE_TOKEN_INCLUDED
#define ____INCLUDE_TOKEN_INCLUDED

typedef struct includeTokenStruct* IncludeToken;

#define INCLUDE_TOKEN_TYPE_DATA ptrof(includeTokenTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/String.h>
#include <gvlib/StringBuilder.h>
#include <gvlib/TypeData.h>
#include <gvlib/io/PushbackStream.h>

struct includeTokenFunctionsStruct {
  void (* dump)(IncludeToken this);
  gvboolean (* equals)(IncludeToken this, Object other);
  int (* hashCode)(IncludeToken this);
  void (* reserve)(IncludeToken this);
  TypeData (* rtType)(void);
};

struct includeTokenStruct {
  readonly refCount_t refCount;
  const struct includeTokenFunctionsStruct* const call;
  const String fileName;
  const uint64_t value;
  const int column;
  const int line;
  const int code;
  const int length;
  const int errwarn;
  int code2;
};

extern const struct typeDataStruct includeTokenTypeDataStruct;

IncludeToken newIncludeToken(int line, int column, PushbackStream stream, int cr, StringBuilder clearedStringBuilder);

#endif
