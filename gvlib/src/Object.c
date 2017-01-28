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

#include "../include/gvlib/defs.h"
#undef readonly
#define readonly
#include "../include/gvlib/Object.h"
#undef readonly
#define readonly const

#include "../include/gvlib/errors.h"
#include "../include/gvlib/hashCode.h"
#include "../include/gvlib/private/objectPrivate.h"
#ifdef DEBUG_MODE
# include "../include/gvlib/mem.h"
#endif

static const struct objectFunctionsStruct objectFunctions = {
  Object_dump,
  Object_equals,
  Object_hashCode,
  Object_reserve,
  Object_rtType
};

const struct typeDataStruct objectTypeDataStruct = {
  "Object",
  Object_constructor,
  ptrof(objectFunctions),
  sizeof(struct objectStruct),
  NULL
};

Object newObject(void) {
  Object newInstance;

  ___BTPUSH;
  newInstance = Object_constructor(OBJECT_TYPE_DATA);
  ___BTPOP;
  return newInstance;
}

void* Object_constructor(TypeData typeData, ...) {
  Object newInstance;

  ___BTPUSH;
#ifdef DEBUG_MODE
  newInstance = memAllocate(typeData->name, typeData->size);
#else
  newInstance = malloc(typeData->size);
#endif
  newInstance->refCount = 1;
  newInstance->call = typeData->call;
  ___BTPOP;
  return newInstance;
}

void Object_dump(Object this) {
  ___BTPUSH;
  --this->refCount;
  if(this->refCount == 0) {

#ifdef DEBUG_MODE
    memRelease(this);
#else
    free(this);
#endif

  }
  ___BTPOP;
}

gvboolean Object_equals(Object this, Object other) {
  return this == other;
}

int Object_hashCode(Object this) {
  return HashCode.get((unsigned int)(long int)this);
}

void Object_reserve(Object this) {
  ___BTPUSH;
  ++this->refCount;
  ___BTPOP;
}

TypeData Object_rtType(void) {
  return OBJECT_TYPE_DATA;
}
