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

#ifndef ____MAP_INCLUDED
#define ____MAP_INCLUDED

typedef struct mapStruct* Map;

#define MAP_TYPE_DATA ptrof(mapTypeDataStruct)

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"

struct mapFunctionsStruct {
  void (* dump)(Map this);
  gvboolean (* equals)(Map this, Object other);
  int (* hashCode)(Map this);
  void (* reserve)(Map this);
  TypeData (* rtType)(void);

  void (* add)(Map this, Object key, Object value);
  void (* addAndReserve)(Map this, Object key, Object value);
  void (* clear)(Map this);
  void (* delete)(Map this, Object key);
  Object (* get)(Map this, Object key);
};

struct mapStruct {
  readonly refCount_t refCount;
  const struct mapFunctionsStruct* const call;
  readonly int size;
};

extern const struct typeDataStruct mapTypeDataStruct;

/* The following conditions for the arguments should hold:
   initialCapacity > 1,
   1/initialCapacity  < loadFactor.
   If the conditions are not met, default values will be set. */
Map newMap(int initialCapacity, double loadFactor);

#endif
