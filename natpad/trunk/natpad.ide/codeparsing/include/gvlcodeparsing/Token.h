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

#ifndef ____TOKEN_INCLUDED
#define ____TOKEN_INCLUDED

typedef struct tokenStruct* Token;

#define TOKEN_TYPE_DATA ptrof(tokenTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/String.h>
#include <gvlib/TypeData.h>

#define Token_getCode(this) (this->code)
#define Token_getErrwarn(this) (this->errwarn)

struct tokenFunctionsStruct {
  void (* dump)(Token this);
  gvboolean (* equals)(Token this, Object other);
  int (* hashCode)(Token this);
  void (* reserve)(Token this);
  TypeData (* rtType)(void);
};

struct tokenStruct {
  readonly refCount_t refCount;
  const struct tokenFunctionsStruct* const call;
  const String element; /* Possibly null. */
  const uint64_t value;
  const int column;
  const int line;
  const int code;
  const int length;
  const int errwarn;
  int code2;
};

extern const struct typeDataStruct tokenTypeDataStruct;

#endif
