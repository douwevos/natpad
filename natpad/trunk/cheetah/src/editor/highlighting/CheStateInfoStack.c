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

#include <gvlib/errors.h>
#include <gvlib/private/objectPrivate.h>
#include "CheStateInfoStack.h"

#ifdef DEBUG_MODE
# include <gvlib/mem.h>
#endif

struct private_cheStateInfoStackStruct {
  readonly refCount_t refCount;
  const struct cheStateInfoStackFunctionsStruct* const call;
  cheStateInfo_t* buf;
  int capacityQuantum;
  int capacity;
  int size;
};

#define CheStateInfoStack struct private_cheStateInfoStackStruct*

static void clear(CheStateInfoStack this);
static void* constructor(TypeData typeData, ...);
static void dump(CheStateInfoStack this);
static gvboolean isEmpty(CheStateInfoStack this);
static cheStateInfo_t peek(CheStateInfoStack this);
static cheStateInfo_t pop(CheStateInfoStack this);
static void push(CheStateInfoStack this, cheStateInfo_t val);
static TypeData rtType(void);

#undef CheStateInfoStack

static const struct cheStateInfoStackFunctionsStruct cheStateInfoStackFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,
  (void*)clear,
  (void*)isEmpty,
  (void*)peek,
  (void*)pop,
  (void*)push
};

const struct typeDataStruct cheStateInfoStackTypeDataStruct = {
  "CheStateInfoStack",
  constructor,
  ptrof(cheStateInfoStackFunctions),
  sizeof(struct private_cheStateInfoStackStruct),
  OBJECT_TYPE_DATA
};

CheStateInfoStack newCheStateInfoStack(int initialCapacity) {
  CheStateInfoStack newInstance;

  ___BTPUSH;
  if(initialCapacity < 8) {
    initialCapacity = 8;
  }
  newInstance = constructor(CHE_STATE_INFO_STACK_TYPE_DATA, initialCapacity);
  ___BTPOP;
  return newInstance;
}

#define CheStateInfoStack struct private_cheStateInfoStackStruct*

#define ERR_STACK_EMPTY(opname) "Cannot %s: stack empty", opname

static void clear(CheStateInfoStack this) {
  ___BTPUSH;
  this->size = 0;
  ___BTPOP;
}

static void* constructor(TypeData typeData, ...) {
  CheStateInfoStack newInstance;
  int initialCapacity;
  va_list args;

  ___BTPUSH;
  va_start(args, typeData);
  initialCapacity = va_arg(args, int);
  va_end(args);

  newInstance = Object_constructor(typeData);
#ifdef DEBUG_MODE
  newInstance->buf = memAllocate("cheStateInfo_t*", initialCapacity * sizeof(cheStateInfo_t));
#else
  newInstance->buf = malloc(initialCapacity * sizeof(cheStateInfo_t));
#endif
  newInstance->capacityQuantum = initialCapacity;
  newInstance->capacity = initialCapacity;
  newInstance->size = 0;
  ___BTPOP;
  return newInstance;
}

static void dump(CheStateInfoStack this) {
  ___BTPUSH;
  if(this->refCount == 1) {
#ifdef DEBUG_MODE
    memRelease(this->buf);
#else
    free(this->buf);
#endif
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static gvboolean isEmpty(CheStateInfoStack this) {
	gvboolean result;

  ___BTPUSH;
  result = this->size == 0;
  ___BTPOP;
  return result;
}

static cheStateInfo_t peek(CheStateInfoStack this) {
  cheStateInfo_t result;

  ___BTPUSH;
#ifdef DEBUG_MODE
  if(this->size == 0) {
    Errors.printMessageAndExit(ERR_STACK_EMPTY("peek"));
  }
#endif
  result = this->buf[this->size - 1];
  ___BTPOP;
  return result;
}

static cheStateInfo_t pop(CheStateInfoStack this) {
  cheStateInfo_t result;

  ___BTPUSH;
#ifdef DEBUG_MODE
  if(this->size == 0) {
    Errors.printMessageAndExit(ERR_STACK_EMPTY("pop"));
  }
#endif
  --this->size;
  result = this->buf[this->size];
  ___BTPOP;
  return result;
}

static void push(CheStateInfoStack this, cheStateInfo_t val) {
  ___BTPUSH;
  if(this->size == this->capacity) {
    this->capacity += this->capacityQuantum;
#ifdef DEBUG_MODE
    this->buf = memReallocate(this->buf, this->capacity * sizeof(cheStateInfo_t));
#else
    this->buf = realloc(this->buf, this->capacity * sizeof(cheStateInfo_t));
#endif
  }
  this->buf[this->size] = val;
  ++this->size;
  ___BTPOP;
}

static TypeData rtType(void) {
  return CHE_STATE_INFO_STACK_TYPE_DATA;
}
