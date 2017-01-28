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

#ifndef ___LIST_PRIVATE_INCLUDED
#define ___LIST_PRIVATE_INCLUDED

#include "../defs.h"
#undef readonly
#define readonly
#include "../sequences/List.h"
#undef readonly
#define readonly const

#include "../Object.h"
#include "../TypeData.h"
#include "objectPrivate.h"

#define LIST_TYPE_DATA ptrof(listTypeDataStruct)

extern const struct typeDataStruct listTypeDataStruct;

    void List_append(List this, Object val);
    void List_appendNew(List this, Object val);
    void List_appendRange(List this, const Object* sPtr, int sOff, int len);
    void List_clear(List this);
   void* List_constructor(TypeData typeData, ...);
   gvboolean List_contains(List this, Object val);
    void List_delete(List this, int dOff);
    void List_deleteLast(List this);
    void List_dump(List this);
    void List_ensureCapacity(List this, int additionalSize);
     int List_indexOf(List this, Object val);
    void List_insert(List this, int dOff, Object val);
    void List_insertNew(List this, int dOff, Object val);
  Object List_remove(List this, int dOff);
  Object List_removeLast(List this);
TypeData List_rtType(void);
    void List_setCapacityQuantum(List this, int val);
    void List_sort(List this, int (* compare)(Object,Object));
    void List_trimToSize(List this);

#endif
