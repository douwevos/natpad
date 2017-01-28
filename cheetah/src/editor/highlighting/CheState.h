/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

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

#ifndef ____CHE_STATE_INCLUDED
#define ____CHE_STATE_INCLUDED

typedef struct cheStateStruct* CheState;

#define CHE_STATE_TYPE_DATA ptrof(cheStateTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/TypeData.h>

struct cheStateFunctionsStruct {
  void (* dump)(CheState this);
  gvboolean (* equals)(CheState this, Object other);
  int (* hashCode)(CheState this);
  void (* reserve)(CheState this);
  TypeData (* rtType)(void);

  gvboolean (* decParenthesesOpen)(CheState this);
  void (* incParenthesesOpen)(CheState this);
  void (* reset)(CheState this);
};

struct cheStateStruct {
  readonly refCount_t refCount;
  const struct cheStateFunctionsStruct* const call;
  readonly int parenthesesOpen;
  gvboolean typeDef;
};

const struct typeDataStruct cheStateTypeDataStruct;

CheState newCheState(void);

#endif
