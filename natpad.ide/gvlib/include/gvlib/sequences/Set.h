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

#ifndef ____SET_INCLUDED
#define ____SET_INCLUDED

typedef struct setStruct* Set;

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"

struct setFunctionsStruct {
  void (* dump)(Set this);
  gvboolean (* equals)(Set this, Object other);
  int (* hashCode)(Set this);
  void (* reserve)(Set this);
  TypeData (* rtType)(void);

  void (* append)(Set this, Object val);
  void (* appendNew)(Set this, Object val);
  void (* appendRange)(Set this, const Object* sPtr, int sOff, int len);
  void (* clear)(Set this);
  gvboolean (* contains)(Set this, Object val);
  void (* delete)(Set this, int dOff);
  void (* deleteLast)(Set this);
  void (* ensureCapacity)(Set this, int additionalSize);
  int (* indexOf)(Set this, Object val);
  void (* insert)(Set this, int dOff, Object val);
  void (* insertNew)(Set this, int dOff, Object val);
  Object (* remove)(Set this, int dOff);
  Object (* removeLast)(Set this);
  void (* setCapacityQuantum)(Set this, int val);
  void (* sort)(Set this, int (* compare)(Object,Object));
  void (* trimToSize)(Set this);
};

struct setStruct {
  readonly refCount_t refCount;
  const struct setFunctionsStruct* const call;
  readonly Object* readonly buf;
  readonly int capacityQuantum;
  readonly int capacity;
  readonly int size;
};

Set newSet(int initialCapacity);

#endif
