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

#ifndef ____PRIMARY_TOKEN_INCLUDED
#define ____PRIMARY_TOKEN_INCLUDED

typedef struct primaryTokenStruct* PrimaryToken;

#define PRIMARY_TOKEN_TYPE_DATA ptrof(primaryTokenTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/String.h>
#include <gvlib/StringBuilder.h>
#include <gvlib/TypeData.h>
#include <gvlib/io/PushbackStream.h>
#include "StringElemInfoMap.h"

struct primaryTokenFunctionsStruct {
  void (* dump)(PrimaryToken this);
  gvboolean (* equals)(PrimaryToken this, Object other);
  int (* hashCode)(PrimaryToken this);
  void (* reserve)(PrimaryToken this);
  TypeData (* rtType)(void);
};

struct primaryTokenStruct {
  readonly refCount_t refCount;
  const struct primaryTokenFunctionsStruct* const call;
  const String element;
  const uint64_t value;
  const int column;
  const int line;
  const int code;
  const int length;
  const int errwarn;
  int code2;
};

extern const struct typeDataStruct primaryTokenTypeDataStruct;

PrimaryToken newPrimaryToken(int line, int column, PushbackStream stream, int cr,
                             StringBuilder clearedStringBuilder,
                             StringElemInfoMap map);

#endif
