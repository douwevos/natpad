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

#ifndef ___GENERIC_SET_INCLUDED
#define ___GENERIC_SET_INCLUDED

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"
#include "../macros/macros.h"

#define GENERIC_SET(ElementType) \
  typedef struct generic ## ElementType ## SetStruct* ElementType ## Set; \
  struct generic ## ElementType ## SetFunctionsStruct { \
    void (* dump)(ElementType ## Set this); \
    gvboolean (* equals)(ElementType ## Set this, Object other); \
    int (* hashCode)(ElementType ## Set this); \
    void (* reserve)(ElementType ## Set this); \
    TypeData (* rtType)(void); \
    void (* append)(ElementType ## Set this, ElementType val); \
    void (* appendNew)(ElementType ## Set this, ElementType val); \
    void (* appendRange)(ElementType ## Set this, const ElementType* sPtr, int sOff, int len); \
    void (* clear)(ElementType ## Set this); \
    gvboolean (* contains)(ElementType ## Set this, ElementType val); \
    void (* delete)(ElementType ## Set this, int dOff); \
    void (* deleteLast)(ElementType ## Set this); \
    void (* ensureCapacity)(ElementType ## Set this, int additionalSize); \
    int (* indexOf)(ElementType ## Set this, ElementType val); \
    void (* insert)(ElementType ## Set this, int dOff, ElementType val); \
    void (* insertNew)(ElementType ## Set this, int dOff, ElementType val); \
    ElementType (* remove)(ElementType ## Set this, int dOff); \
    ElementType (* removeLast)(ElementType ## Set this); \
    void (* setCapacityQuantum)(ElementType ## Set this, int val); \
    void (* sort)(ElementType ## Set this, int (* compare)(ElementType,ElementType)); \
    void (* trimToSize)(ElementType ## Set this); \
  }; \
  struct generic ## ElementType ## SetStruct { \
    readonly refCount_t refCount; \
    const struct generic ## ElementType ## SetFunctionsStruct* const call; \
    readonly ElementType* readonly buf; \
    readonly int capacityQuantum; \
    readonly int capacity; \
    readonly int size; \
  }; \
  ElementType ## Set new ## ElementType ## Set(int initialCapacity);


#ifdef DEBUG_MODE

# define GENERIC_SET_CONSTRUCTOR(ElementType) \
  ElementType ## Set new ## ElementType ## Set(int initialCapacity) { \
    struct typeDataStruct genericSetTypeDataStruct = { \
      ___QUOTE_GLUED(ElementType, Set), \
      SET_TYPE_DATA->constructor, \
      SET_TYPE_DATA->call, \
      SET_TYPE_DATA->size, \
      SET_TYPE_DATA->superTypeData \
    }; \
    ElementType ## Set newInstance; \
    ___BTPUSH; \
    newInstance = List_constructor(ptrof(genericSetTypeDataStruct), ___QUOTE(ElementType*), initialCapacity); \
    ___BTPOP; \
    return newInstance; \
  }

#else

# define GENERIC_SET_CONSTRUCTOR(ElementType) \
  ElementType ## Set new ## ElementType ## Set(int initialCapacity) { \
    ElementType ## Set newInstance; \
    newInstance = List_constructor(SET_TYPE_DATA, initialCapacity); \
    return newInstance; \
  }

#endif

#endif
