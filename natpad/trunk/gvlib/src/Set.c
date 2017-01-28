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

#include "../include/gvlib/private/setPrivate.h"
#include "../include/gvlib/errors.h"

static const struct setFunctionsStruct setFunctions = {
  (void*)List_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  Set_rtType,

  Set_append,
  Set_appendNew,
  Set_appendRange,
  (void*)List_clear,
  (void*)List_contains,
  (void*)List_delete,
  (void*)List_deleteLast,
  (void*)List_ensureCapacity,
  (void*)List_indexOf,
  Set_insert,
  Set_insertNew,
  (void*)List_remove,
  (void*)List_removeLast,
  (void*)List_setCapacityQuantum,
  (void*)List_sort,
  (void*)List_trimToSize
};

const struct typeDataStruct setTypeDataStruct = {
  "Set",
  List_constructor,
  ptrof(setFunctions),
  sizeof(struct setStruct),
  LIST_TYPE_DATA
};

Set newSet(int initialCapacity) {
  Set newInstance;

  ___BTPUSH;
#ifdef DEBUG_MODE
  newInstance = List_constructor(SET_TYPE_DATA, "Object*", initialCapacity);
#else
  newInstance = List_constructor(SET_TYPE_DATA, initialCapacity);
#endif
  ___BTPOP;
  return newInstance;
}

#ifdef DEBUG_MODE
# define ERR_ARRAYS_OVERLAP "The specified object array 'sPtr' is not allowed to overlap with 'this->buf'"
# define ERR_ELEMENT_NULL "At least one of the elements of the specified object array to append is\na null-pointer"
# define ERR_LENGTH_NEGATIVE(param) "The specified parameter '%s' should be nonnegative", #param
# define ERR_OBJ_ARRAY_NULL "No object array 'sPtr' specified (null)"
#endif

void Set_append(Set this, Object val) {
  ___BTPUSH;
  if(!List_contains((List)this, val)) {
    List_append((List)this, val);
  }
  ___BTPOP;
}

void Set_appendNew(Set this, Object val) {
  ___BTPUSH;
  if(!List_contains((List)this, val)) {
    List_appendNew((List)this, val);
  } else {
    val->call->dump(val);
  }
  ___BTPOP;
}

void Set_appendRange(Set this, const Object* sPtr, int sOff, int len) {
#ifdef DEBUG_MODE
  struct {
    unsigned long int start;
    unsigned long int end;
  } regionA, regionB;
#endif
  int dOff, i;

  ___BTPUSH;
#ifdef DEBUG_MODE
  if(sPtr == NULL) {
    Errors.printMessageAndExit(ERR_OBJ_ARRAY_NULL);
  }
  if(len < 0) {
    Errors.printMessageAndExit(ERR_LENGTH_NEGATIVE(len));
  }
  if(len > 0) {
    regionA.start = (long int)this->buf;
    regionA.end = regionA.start + (this->capacity - 1) * sizeof(void*);
    regionB.start = (long int)sPtr + sOff;
    regionB.end = regionB.start + (len - 1) * sizeof(void*);
    if(!(regionA.end < regionB.start  ||  regionA.start > regionB.end)) {
      Errors.printMessageAndExit(ERR_ARRAYS_OVERLAP);
    }
  }
#endif

  if(len > 0) {
    dOff = this->size;
    List_ensureCapacity((List)this, len);
    for(i=0; i<len; ++i) {
#ifdef DEBUG_MODE
      if(sPtr[sOff] == NULL) {
        Errors.printMessageAndExit(ERR_ELEMENT_NULL);
      }
#endif
      if(!List_contains((List)this, sPtr[sOff])) {
        sPtr[sOff]->call->reserve(sPtr[sOff]);
        this->buf[dOff] = sPtr[sOff];
        ++this->size;
        ++dOff;
      }
      ++sOff;
    }
    this->buf[this->size] = NULL;
  }
  ___BTPOP;
}

void Set_insert(Set this, int dOff, Object val) {
  ___BTPUSH;
  if(!List_contains((List)this, val)) {
    List_insert((List)this, dOff, val);
  }
  ___BTPOP;
}

void Set_insertNew(Set this, int dOff, Object val) {
  ___BTPUSH;
  if(!List_contains((List)this, val)) {
    List_insertNew((List)this, dOff, val);
  } else {
    val->call->dump(val);
  }
  ___BTPOP;
}

TypeData Set_rtType(void) {
  return SET_TYPE_DATA;
}
