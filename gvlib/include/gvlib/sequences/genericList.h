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

#ifndef ___GENERIC_LIST_INCLUDED
#define ___GENERIC_LIST_INCLUDED

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"
#include "../macros/macros.h"

#define GENERIC_LIST(ElementType) \
  typedef struct generic ## ElementType ## ListStruct* ElementType ## List; \
  struct generic ## ElementType ## ListFunctionsStruct { \
    void (* dump)(ElementType ## List this); \
    gvboolean (* equals)(ElementType ## List this, Object other); \
    int (* hashCode)(ElementType ## List this); \
    void (* reserve)(ElementType ## List this); \
    TypeData (* rtType)(void); \
    void (* append)(ElementType ## List this, ElementType val); \
    void (* appendNew)(ElementType ## List this, ElementType val); \
    void (* appendRange)(ElementType ## List this, const ElementType* sPtr, int sOff, int len); \
    void (* clear)(ElementType ## List this); \
    gvboolean (* contains)(ElementType ## List this, ElementType val); \
    void (* delete)(ElementType ## List this, int dOff); \
    void (* deleteLast)(ElementType ## List this); \
    void (* ensureCapacity)(ElementType ## List this, int additionalSize); \
    int (* indexOf)(ElementType ## List this, ElementType val); \
    void (* insert)(ElementType ## List this, int dOff, ElementType val); \
    void (* insertNew)(ElementType ## List this, int dOff, ElementType val); \
    ElementType (* remove)(ElementType ## List this, int dOff); \
    ElementType (* removeLast)(ElementType ## List this); \
    void (* setCapacityQuantum)(ElementType ## List this, int val); \
    void (* sort)(ElementType ## List this, int (* compare)(ElementType,ElementType)); \
    void (* trimToSize)(ElementType ## List this); \
  }; \
  struct generic ## ElementType ## ListStruct { \
    readonly refCount_t refCount; \
    const struct generic ## ElementType ## ListFunctionsStruct* const call; \
    readonly ElementType* readonly buf; \
    readonly int capacityQuantum; \
    readonly int capacity; \
    readonly int size; \
  }; \
  ElementType ## List new ## ElementType ## List(int initialCapacity);


#ifdef DEBUG_MODE

# define GENERIC_LIST_CONSTRUCTOR(ElementType) \
  ElementType ## List new ## ElementType ## List(int initialCapacity) { \
    struct typeDataStruct genericListTypeDataStruct = { \
      ___QUOTE_GLUED(ElementType, List), \
      LIST_TYPE_DATA->constructor, \
      LIST_TYPE_DATA->call, \
      LIST_TYPE_DATA->size, \
      LIST_TYPE_DATA->superTypeData \
    }; \
    ElementType ## List newInstance; \
    ___BTPUSH; \
    newInstance = List_constructor(ptrof(genericListTypeDataStruct), ___QUOTE(ElementType*), initialCapacity); \
    ___BTPOP; \
    return newInstance; \
  }

#else

# define GENERIC_LIST_CONSTRUCTOR(ElementType) \
  ElementType ## List new ## ElementType ## List(int initialCapacity) { \
    ElementType ## List newInstance; \
    newInstance = List_constructor(LIST_TYPE_DATA, initialCapacity); \
    return newInstance; \
  }

#endif

#endif
