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

#ifndef ___GENERIC_MAP_INCLUDED
#define ___GENERIC_MAP_INCLUDED

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"
#include "../macros/macros.h"

#define GENERIC_MAP(KeyType, ValueType) \
  typedef struct generic ## KeyType ## ValueType ## MapStruct* KeyType ## ValueType ## Map; \
  struct generic ## KeyType ## ValueType ## MapFunctionsStruct { \
    void (* dump)(KeyType ## ValueType ## Map this); \
    gvboolean (* equals)(KeyType ## ValueType ## Map this, Object other); \
    int (* hashCode)(KeyType ## ValueType ## Map this); \
    void (* reserve)(KeyType ## ValueType ## Map this); \
    TypeData (* rtType)(void); \
    void (* add)(KeyType ## ValueType ## Map this, KeyType key, ValueType value); \
    void (* addAndReserve)(KeyType ## ValueType ## Map this, KeyType key, ValueType value); \
    void (* clear)(KeyType ## ValueType ## Map this); \
    void (* delete)(KeyType ## ValueType ## Map this, KeyType key); \
    ValueType (* get)(KeyType ## ValueType ## Map this, KeyType key); \
  }; \
  struct generic ## KeyType ## ValueType ## MapStruct { \
    readonly refCount_t refCount; \
    const struct generic ## KeyType ## ValueType ## MapFunctionsStruct* const call; \
    readonly int size; \
  }; \
  KeyType ## ValueType ## Map new ## KeyType ## ValueType ## Map(int initialCapacity, double loadFactor);


#ifdef DEBUG_MODE

# define GENERIC_MAP_CONSTRUCTOR(KeyType, ValueType) \
  KeyType ## ValueType ## Map new ## KeyType ## ValueType ## Map(int initialCapacity, double loadFactor) { \
    struct typeDataStruct genericMapTypeDataStruct = { \
      ___QUOTE_GLUED(KeyType ## ValueType, Map), \
      MAP_TYPE_DATA->constructor, \
      MAP_TYPE_DATA->call, \
      MAP_TYPE_DATA->size, \
      MAP_TYPE_DATA->superTypeData \
    }; \
    KeyType ## ValueType ## Map newInstance; \
    ___BTPUSH; \
    newInstance = Map_constructor(ptrof(genericMapTypeDataStruct), initialCapacity, loadFactor); \
    ___BTPOP; \
    return newInstance; \
  }

#else

# define GENERIC_MAP_CONSTRUCTOR(KeyType, ValueType) \
  KeyType ## ValueType ## Map new ## KeyType ## ValueType ## Map(int initialCapacity, double loadFactor) { \
    return Map_constructor(MAP_TYPE_DATA, initialCapacity, loadFactor); \
  }

#endif

#endif
