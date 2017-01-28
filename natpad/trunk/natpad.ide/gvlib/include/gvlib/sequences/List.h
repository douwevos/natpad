/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2008, 2009, 2010 Gerard Visser.

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

#ifndef ____LIST_INCLUDED
#define ____LIST_INCLUDED

typedef struct listStruct* List;

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"

struct listFunctionsStruct {
  void (* dump)(List this);
  gvboolean (* equals)(List this, Object other);
  int (* hashCode)(List this);
  void (* reserve)(List this);
  TypeData (* rtType)(void);

  void (* append)(List this, Object val);
  void (* appendNew)(List this, Object val);
  void (* appendRange)(List this, const Object* sPtr, int sOff, int len);
  void (* clear)(List this);
  gvboolean (* contains)(List this, Object val);
  void (* delete)(List this, int dOff);
  void (* deleteLast)(List this);
  void (* ensureCapacity)(List this, int additionalSize);
  int (* indexOf)(List this, Object val);
  void (* insert)(List this, int dOff, Object val);
  void (* insertNew)(List this, int dOff, Object val);
  Object (* remove)(List this, int dOff);
  Object (* removeLast)(List this);
  void (* setCapacityQuantum)(List this, int val);
  void (* sort)(List this, int (* compare)(Object,Object));
  void (* trimToSize)(List this);
};

struct listStruct {
  readonly refCount_t refCount;
  const struct listFunctionsStruct* const call;
  readonly Object* readonly buf;
  readonly int capacityQuantum;
  readonly int capacity;
  readonly int size;
};

List newList(int initialCapacity);

#endif
