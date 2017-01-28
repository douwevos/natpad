/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

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

#include "../include/gvlib/private/listPrivate.h"
#include "../include/gvlib/array.h"
#include "../include/gvlib/errors.h"
#ifdef DEBUG_MODE
# include "../include/gvlib/mem.h"
#endif

static const struct listFunctionsStruct listFunctions = {
  List_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  List_rtType,

  List_append,
  List_appendNew,
  List_appendRange,
  List_clear,
  List_contains,
  List_delete,
  List_deleteLast,
  List_ensureCapacity,
  List_indexOf,
  List_insert,
  List_insertNew,
  List_remove,
  List_removeLast,
  List_setCapacityQuantum,
  List_sort,
  List_trimToSize
};

const struct typeDataStruct listTypeDataStruct = {
  "List",
  List_constructor,
  ptrof(listFunctions),
  sizeof(struct listStruct),
  OBJECT_TYPE_DATA
};

List newList(int initialCapacity) {
  List newInstance;

  ___BTPUSH;
#ifdef DEBUG_MODE
  newInstance = List_constructor(LIST_TYPE_DATA, "Object*", initialCapacity);
#else
  newInstance = List_constructor(LIST_TYPE_DATA, initialCapacity);
#endif
  ___BTPOP;
  return newInstance;
}

#ifdef DEBUG_MODE
# define ERR_ADD_NULL_POINTER "Only pointers to objects should be added to a list of objects: this rules out\nthe null-pointer"
# define ERR_ARRAYS_OVERLAP "The specified object array 'sPtr' is not allowed to overlap with 'this->buf'"
# define ERR_CAPACITY_QUANTUM_NOT_NATURAL "The value specified for capacity quantum should be a natural number (i.e. a\npositive integer)"
# define ERR_ELEMENT_NULL "At least one of the elements of the specified object array to append is\na null-pointer"
# define ERR_INDEX_OUT_OF_RANGE "Index out of range: list size=%d, dOff=%d", this->size, dOff
# define ERR_LENGTH_NEGATIVE(param) "The specified parameter '%s' should be nonnegative", #param
# define ERR_LIST_EMPTY "List is empty"
# define ERR_MAX_CAPACITY_EXCEEDED "List's maximum capacity (%d) exceeded", MAX_CAPACITY
# define ERR_NULL_POINTER_IS_NOT_OBJECT "The null-pointer is not a reference to an object"
# define ERR_OBJ_ARRAY_NULL "No object array 'sPtr' specified (null)"
# define MAX_CAPACITY 0x7FFFFFFE

# define DBG_ERR_CHECK_APPEND(obj)                    \
  if(obj == NULL) {                                   \
    Errors.printMessageAndExit(ERR_ADD_NULL_POINTER); \
  }

# define DBG_ERR_CHECK_INSERT(dOff, obj)                \
  if(obj == NULL) {                                     \
    Errors.printMessageAndExit(ERR_ADD_NULL_POINTER);   \
  }                                                     \
  if(dOff < 0  ||  dOff > this->size) {                 \
    Errors.printMessageAndExit(ERR_INDEX_OUT_OF_RANGE); \
  }

# define DBG_ERR_CHECK_REMOVE(dOff)                     \
  if(dOff < 0  ||  dOff >= this->size) {                \
    Errors.printMessageAndExit(ERR_INDEX_OUT_OF_RANGE); \
  }

# define DBG_ERR_CHECK_REMOVE_LAST()            \
  if(this->size == 0) {                         \
    Errors.printMessageAndExit(ERR_LIST_EMPTY); \
  }

#else

# define DBG_ERR_CHECK_APPEND(obj)
# define DBG_ERR_CHECK_INSERT(dOff, obj)
# define DBG_ERR_CHECK_REMOVE(dOff)
# define DBG_ERR_CHECK_REMOVE_LAST()

#endif

static inline void append(List this, Object val) {
  DBG_ERR_CHECK_APPEND(val);
  List_ensureCapacity(this, 1);
  this->buf[this->size] = val;
  ++this->size;
  this->buf[this->size] = NULL;
}

static inline int indexOf(List this, Object val) {
  int index;

#ifdef DEBUG_MODE
  if(val == NULL) {
    Errors.printMessageAndExit(ERR_NULL_POINTER_IS_NOT_OBJECT);
  }
#endif
  index = 0;
  while(index < this->size  &&  !val->call->equals(val, this->buf[index])) {
    ++index;
  }
  if(index < this->size) {
    return index;
  } else {
    return -1;
  }
}

static inline void insertElement(List this, int dOff, Object elem) {
  DBG_ERR_CHECK_INSERT(dOff, elem);
  List_ensureCapacity(this, 1);
  ++this->size;
  Array.copyPtrs((void*)this->buf, dOff + 1, (void*)this->buf, dOff, this->size - dOff);
  this->buf[dOff] = elem;
}

static inline void reallocateBuffer(List this) {
#ifdef DEBUG_MODE
  this->buf = memReallocate(this->buf, (this->capacity + 1) * sizeof(void*));
#else
  this->buf = realloc((void*)this->buf, (this->capacity + 1) * sizeof(void*));
#endif
}

static inline void removeElement(List this, int dOff) {
  Array.copyPtrs((void*)this->buf, dOff, (void*)this->buf, dOff + 1, this->size - dOff);
  --this->size;
}

void List_append(List this, Object val) {
  ___BTPUSH;
  append(this, val);
  val->call->reserve(val);
  ___BTPOP;
}

void List_appendNew(List this, Object val) {
  ___BTPUSH;
  append(this, val);
  ___BTPOP;
}

void List_appendRange(List this, const Object* sPtr, int sOff, int len) {
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
    List_ensureCapacity(this, len);
    for(i=0; i<len; ++i) {
#ifdef DEBUG_MODE
      if(sPtr[sOff] == NULL) {
        Errors.printMessageAndExit(ERR_ELEMENT_NULL);
      }
#endif
      sPtr[sOff]->call->reserve(sPtr[sOff]);
      this->buf[dOff] = sPtr[sOff];
      ++dOff;
      ++sOff;
    }
    this->size += len;
    this->buf[this->size] = NULL;
  }
  ___BTPOP;
}

void List_clear(List this) {
  int i;

  ___BTPUSH;
  for(i=0; i<this->size; ++i) {
    this->buf[i]->call->dump(this->buf[i]);
  }
  this->buf[0] = NULL;
  this->size = 0;
  ___BTPOP;
}

void* List_constructor(TypeData typeData, ...) {
  List newInstance;
  int initialCapacity;
  va_list argp;
#ifdef DEBUG_MODE
  const char* bufTypeName;
#endif

  ___BTPUSH;
  va_start(argp, typeData);
#ifdef DEBUG_MODE
  bufTypeName = va_arg(argp, char*);
#endif
  initialCapacity = va_arg(argp, int);
  if(initialCapacity < 1) {
    initialCapacity = 10;
  }
  va_end(argp); /* ANSI C requirement. */

  newInstance = Object_constructor(typeData);
#ifdef DEBUG_MODE
  newInstance->buf = memAllocate(bufTypeName, (initialCapacity + 1)*sizeof(void*));
#else
  newInstance->buf = malloc((initialCapacity + 1)*sizeof(void*));
#endif
  newInstance->buf[0] = NULL;
  newInstance->capacityQuantum = initialCapacity;
  newInstance->capacity = initialCapacity;
  newInstance->size = 0;
  ___BTPOP;
  return newInstance;
}

gvboolean List_contains(List this, Object val) {
	gvboolean result;

  ___BTPUSH;
  result = indexOf(this, val) > -1;
  ___BTPOP;
  return result;
}

void List_delete(List this, int dOff) {
  ___BTPUSH;
  DBG_ERR_CHECK_REMOVE(dOff);
  this->buf[dOff]->call->dump(this->buf[dOff]);
  removeElement(this, dOff);
  ___BTPOP;
}

void List_deleteLast(List this) {
  ___BTPUSH;
  DBG_ERR_CHECK_REMOVE_LAST();
  --this->size;
  this->buf[this->size]->call->dump(this->buf[this->size]);
  this->buf[this->size] = NULL;
  ___BTPOP;
}

void List_dump(List this) {
  int i;

  ___BTPUSH;
  if(this->refCount == 1) {
    for(i=0; i<this->size; ++i) {
      this->buf[i]->call->dump(this->buf[i]);
    }
#ifdef DEBUG_MODE
    memRelease(this->buf);
#else
    free((void*)this->buf);
#endif
  }
  Object_dump((Object)this);
  ___BTPOP;
}

void List_ensureCapacity(List this, int additionalSize) {
  long int delta;

  ___BTPUSH;
#ifdef DEBUG_MODE
  if(additionalSize < 0) {
    Errors.printMessageAndExit(ERR_LENGTH_NEGATIVE(additionalSize));
  }
#endif
  delta = this->size + additionalSize - this->capacity;
  if(delta > 0) {
    delta = 1 + (delta / this->capacityQuantum);
    delta = this->capacityQuantum * delta;
#ifdef DEBUG_MODE
    if(this->capacity + delta > MAX_CAPACITY) {
      Errors.printMessageAndExit(ERR_MAX_CAPACITY_EXCEEDED);
    }
#endif
    this->capacity += delta;
    reallocateBuffer(this);
  }
  ___BTPOP;
}

int List_indexOf(List this, Object val) {
  int result;

  ___BTPUSH;
  result = indexOf(this, val);
  ___BTPOP;
  return result;
}

void List_insert(List this, int dOff, Object val) {
  ___BTPUSH;
  insertElement(this, dOff, val);
  val->call->reserve(val);
  ___BTPOP;
}

void List_insertNew(List this, int dOff, Object val) {
  ___BTPUSH;
  insertElement(this, dOff, val);
  ___BTPOP;
}

Object List_remove(List this, int dOff) {
  Object result;

  ___BTPUSH;
  DBG_ERR_CHECK_REMOVE(dOff);
  result = this->buf[dOff];
  removeElement(this, dOff);
  ___BTPOP;
  return result;
}

Object List_removeLast(List this) {
  Object result;

  ___BTPUSH;
  DBG_ERR_CHECK_REMOVE_LAST();
  --this->size;
  result = this->buf[this->size];
  this->buf[this->size] = NULL;
  ___BTPOP;
  return result;
}

TypeData List_rtType(void) {
  return LIST_TYPE_DATA;
}

void List_setCapacityQuantum(List this, int val) {
  ___BTPUSH;
#ifdef DEBUG_MODE
  if(val < 1) {
    Errors.printMessageAndExit(ERR_CAPACITY_QUANTUM_NOT_NATURAL);
  }
#endif
  this->capacityQuantum = val;
  ___BTPOP;
}

void List_sort(List this, int (* compare)(Object,Object)) {
  int i, j, k;
  Object obj;

  ___BTPUSH;
  if(this->size > 1) {

    for(i = 1;  i < this->size;  ++i) {
      obj = this->buf[i];
      j = 0;
      while(j < i  &&  compare(obj, this->buf[j]) > 0) {
        ++j;
      }
      for(k = i;  k > j;  --k) {
        this->buf[k] = this->buf[k - 1];
      }
      this->buf[j] = obj;
    }

  }
  ___BTPOP;
}

void List_trimToSize(List this) {
  ___BTPUSH;
  if(this->capacity > this->size) {
    this->capacity = this->size;
    reallocateBuffer(this);
  }
  ___BTPOP;
}
