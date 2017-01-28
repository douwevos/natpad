/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2009, 2010, 2011 Gerard Visser.

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

#ifndef ____TOKENISER_INCLUDED
#define ____TOKENISER_INCLUDED

typedef struct tokeniserStruct* Tokeniser;

#define TOKENISER_TYPE_DATA ptrof(tokeniserTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/TypeData.h>
#include <gvlib/io/PushbackStream.h>
#include "TokenList.h"

struct tokeniserFunctionsStruct {
  void (* dump)(Tokeniser this);
  gvboolean (* equals)(Tokeniser this, Object other);
  int (* hashCode)(Tokeniser this);
  void (* reserve)(Tokeniser this);
  TypeData (* rtType)(void);

  TokenList (* getTokens)(Tokeniser this, PushbackStream stream);
};

struct tokeniserStruct {
  readonly refCount_t refCount;
  const struct tokeniserFunctionsStruct* const call;
};

extern const struct typeDataStruct tokeniserTypeDataStruct;

#endif
