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

#ifndef ____C_TOKENISER_INCLUDED
#define ____C_TOKENISER_INCLUDED

typedef struct cTokeniserStruct* CTokeniser;

#define C_TOKENISER_TYPE_DATA ptrof(cTokeniserTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/TypeData.h>
#include <gvlib/io/PushbackStream.h>
#include "TokenList.h"

struct cTokeniserFunctionsStruct {
  void (* dump)(CTokeniser this);
  gvboolean (* equals)(CTokeniser this, Object other);
  int (* hashCode)(CTokeniser this);
  void (* reserve)(CTokeniser this);
  TypeData (* rtType)(void);

  TokenList (* getTokens)(CTokeniser this, PushbackStream stream);
};

struct cTokeniserStruct {
  readonly refCount_t refCount;
  const struct cTokeniserFunctionsStruct* const call;
};

extern const struct typeDataStruct cTokeniserTypeDataStruct;

CTokeniser newCTokeniser(void);

#endif
