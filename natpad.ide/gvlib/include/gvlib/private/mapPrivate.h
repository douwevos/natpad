/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

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

#ifndef ___MAP_PRIVATE_INCLUDED
#define ___MAP_PRIVATE_INCLUDED

#include "../sequences/Map.h"
#include "objectPrivate.h"

/* Definition of a MapEntry. */

typedef struct mapEntryStruct* MapEntry;

struct mapEntryFunctionsStruct {
  void (* dump)(MapEntry this);
  gvboolean (* equals)(MapEntry this, Object other);
  int (* hashCode)(MapEntry this);
  void (* reserve)(MapEntry this);
  TypeData (* rtType)(void);
};

struct mapEntryStruct {
  readonly refCount_t refCount;
  const struct mapEntryFunctionsStruct* const call;
  Object key;
  Object value;
  MapEntry next;
};

MapEntry newMapEntry(Object key, Object value);

/* End of Definition of a MapEntry. */

struct private_mapStruct {
  readonly refCount_t refCount;
  const struct mapFunctionsStruct* const call;
  int size;
  int rehashSize;
  MapEntry* buckets;
  double loadFactor;
  int capacityQuantum;
  int capacity;
};

    void Map_add(struct private_mapStruct* this, Object key, Object value);
    void Map_addAndReserve(struct private_mapStruct* this, Object key, Object value);
    void Map_addEntry(struct private_mapStruct* this, MapEntry entry);
    void Map_clear(struct private_mapStruct* this);
   void* Map_constructor(TypeData typeData, ...);
    void Map_createBuckets(struct private_mapStruct* this);
    void Map_delete(struct private_mapStruct* this, Object key);
    void Map_dump(struct private_mapStruct* this);
  Object Map_get(struct private_mapStruct* this, Object key);
MapEntry Map_getEntry(struct private_mapStruct* this, Object key);
    void Map_rehash(struct private_mapStruct* this);
TypeData Map_rtType(void);

#endif
