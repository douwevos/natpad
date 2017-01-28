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

#ifndef ____CHE_STATE_INFO_STACK_INCLUDED
#define ____CHE_STATE_INFO_STACK_INCLUDED

typedef struct cheStateInfoStackStruct* CheStateInfoStack;

#define CHE_STATE_INFO_STACK_TYPE_DATA ptrof(cheStateInfoStackTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/TypeData.h>
#include "cheStateInfo.h"

struct cheStateInfoStackFunctionsStruct {
  void (* dump)(CheStateInfoStack this);
  gvboolean (* equals)(CheStateInfoStack this, Object other);
  int (* hashCode)(CheStateInfoStack this);
  void (* reserve)(CheStateInfoStack this);
  TypeData (* rtType)(void);

  void (* clear)(CheStateInfoStack this);
  gvboolean (* isEmpty)(CheStateInfoStack this);
  cheStateInfo_t (* peek)(CheStateInfoStack this);
  cheStateInfo_t (* pop)(CheStateInfoStack this);
  void (* push)(CheStateInfoStack this, cheStateInfo_t val);
};

struct cheStateInfoStackStruct {
  readonly refCount_t refCount;
  const struct cheStateInfoStackFunctionsStruct* const call;
};

CheStateInfoStack newCheStateInfoStack(int initialCapacity);

const struct typeDataStruct cheStateInfoStackTypeDataStruct;

#endif
