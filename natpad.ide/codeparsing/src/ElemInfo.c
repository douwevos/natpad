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
#include "../include/gvlcodeparsing/ElemInfo.h"

struct private_elemInfoStruct {
  readonly refCount_t refCount;
  const struct elemInfoFunctionsStruct* const call;
  String element;
  int errwarn;
  int code;
};

static void* constructor(TypeData typeData, ...);
static void dump(struct private_elemInfoStruct* this);
static TypeData rtType(void);

static const struct elemInfoFunctionsStruct elemInfoFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType
};

const struct typeDataStruct elemInfoTypeDataStruct = {
  "ElemInfo",
  constructor,
  ptrof(elemInfoFunctions),
  sizeof(struct private_elemInfoStruct),
  OBJECT_TYPE_DATA
};

ElemInfo newElemInfo(String element, int code, int errwarn) {
  ElemInfo newInstance;

  ___BTPUSH;
  newInstance = constructor(ELEM_INFO_TYPE_DATA, element, code, errwarn);
  ___BTPOP;
  return newInstance;
}

#define ElemInfo struct private_elemInfoStruct*

static void* constructor(TypeData typeData, ...) {
  ElemInfo newInstance;
  String element;
  va_list argp;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  va_start(argp, typeData);
  element = va_arg(argp, void*);
  element->call->reserve(element);
  newInstance->element = element;
  newInstance->code = va_arg(argp, int);
  newInstance->errwarn = va_arg(argp, int);
  va_end(argp);
  ___BTPOP;
  return newInstance;
}

static void dump(ElemInfo this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    this->element->call->dump(this->element);
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static TypeData rtType(void) {
  return ELEM_INFO_TYPE_DATA;
}
