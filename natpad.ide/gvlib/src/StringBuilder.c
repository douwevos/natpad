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

#include "../include/gvlib/private/stringBuilderPrivate.h"
#include "../include/gvlib/array.h"
#include "../include/gvlib/errors.h"
#include "../include/gvlib/mem.h"

static const struct stringBuilderFunctionsStruct stringBuilderFunctions = {
  (void*)String_dump,
  (void*)String_equals,
  (void*)String_calculateHashCode,
  (void*)Object_reserve,
  (void*)StringBuilder_rtType,

  (void*)StringBuilder_append,
  (void*)StringBuilder_appendRange,
  (void*)StringBuilder_appendString,
  (void*)StringBuilder_clear,
  (void*)StringBuilder_markUndoPosition,
  (void*)StringBuilder_toString,
  (void*)StringBuilder_undo
};

const struct typeDataStruct stringBuilderTypeDataStruct = {
  "StringBuilder",
  StringBuilder_constructor,
  ptrof(stringBuilderFunctions),
  sizeof(struct private_stringBuilderStruct),
  STRING_TYPE_DATA
};

StringBuilder newStringBuilder(int initialCapacity) {
  StringBuilder newInstance;

  ___BTPUSH;
  newInstance = StringBuilder_constructor(STRING_BUILDER_TYPE_DATA, initialCapacity);
  ___BTPOP;
  return newInstance;
}

#define StringBuilder struct private_stringBuilderStruct*

#define APPEND_RANGE(sPtr, len) \
  if(len > 0) { \
    ensureCapacity(this, len); \
    Array.copyBytes(this->buf, this->length, sPtr, 0, len); \
    this->length += len; \
    this->buf[this->length] = 0; \
  }

static void ensureCapacity(StringBuilder this, int additionalSize) {
  long int delta;

  ___BTPUSH;
  delta = this->length + additionalSize - this->capacity;
  if(delta > 0) {
    delta = 1 + (delta / this->capacityQuantum);
    delta = this->capacityQuantum * delta;
    this->capacity += delta;
    this->buf = memReallocate(this->buf, this->capacity + 1);
  }
  ___BTPOP;
}

void StringBuilder_append(StringBuilder this, char val) {
  ___BTPUSH;
  ensureCapacity(this, 1);
  this->buf[this->length] = val;
  ++this->length;
  this->buf[this->length] = 0;
  ___BTPOP;
}

void StringBuilder_appendRange(StringBuilder this, const char* sPtr) {
  int len;

  ___BTPUSH;
  len = Array.bytezLength(sPtr) - 1;
  APPEND_RANGE(sPtr, len);
  ___BTPOP;
}

void StringBuilder_appendString(StringBuilder this, String val) {
  ___BTPUSH;
  APPEND_RANGE(val->buf, val->length);
  ___BTPOP;
}

void StringBuilder_clear(StringBuilder this) {
  ___BTPUSH;
  this->undoPosition = 0;
  this->length = 0;
  this->buf[0] = 0;
  ___BTPOP;
}

void* StringBuilder_constructor(TypeData typeData, ...) {
  StringBuilder newInstance;
  int initialCapacity;
  va_list argp;

  ___BTPUSH;
  newInstance = String_constructor(typeData, 0);
  va_start(argp, typeData);
  initialCapacity = va_arg(argp, int);
  if(initialCapacity < 1) {
    initialCapacity = 16;
  }
  va_end(argp);
  newInstance->buf = memAllocate("char*", initialCapacity + 1);
  newInstance->buf[0] = 0;
  newInstance->length = 0;
  newInstance->undoPosition = 0;
  newInstance->capacity = initialCapacity;
  newInstance->capacityQuantum = initialCapacity;
  ___BTPOP;
  return newInstance;
}

void StringBuilder_markUndoPosition(StringBuilder this) {
  ___BTPUSH;
  this->undoPosition = this->length;
  ___BTPOP;
}

TypeData StringBuilder_rtType(void) {
  return STRING_BUILDER_TYPE_DATA;
}

String StringBuilder_toString(StringBuilder this) {
  String result;

  ___BTPUSH;
  result = newString(this->buf, this->length);
  ___BTPOP;
  return result;
}

void StringBuilder_undo(StringBuilder this) {
  ___BTPUSH;
  this->length = this->undoPosition;
  this->buf[this->length] = 0;
  ___BTPOP;
}
