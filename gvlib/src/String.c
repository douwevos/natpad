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

#include "../include/gvlib/private/stringPrivate.h"
#include "../include/gvlib/array.h"
#include "../include/gvlib/errors.h"
#include "../include/gvlib/hashCode.h"
#include "../include/gvlib/mem.h"

#undef newString

static const struct stringFunctionsStruct stringFunctions = {
  (void*)String_dump,
  (void*)String_equals,
  (void*)String_hashCode,
  (void*)Object_reserve,
  (void*)String_rtType
};

const struct typeDataStruct stringTypeDataStruct = {
  "String",
  String_constructor,
  ptrof(stringFunctions),
  sizeof(struct private_stringStruct),
  OBJECT_TYPE_DATA
};

const struct stringFunctionsStruct predefStringFunctions = {
  (void*)predefString_doesNothing,
  (void*)String_equals,
  (void*)predefString_hashCode,
  (void*)predefString_doesNothing,
  (void*)String_rtType
};

String newString(const char* str, ...) {
  String newInstance;
  va_list argp;
  int length;

  ___BTPUSH;
  va_start(argp, str);
  length = va_arg(argp, int);
  va_end(argp); /* ANSI C requirement. */
  if(length != -1) {
    newInstance = String_constructor(STRING_TYPE_DATA, 2, str, length);
  } else {
    newInstance = String_constructor(STRING_TYPE_DATA, 1, str);
  }
  ___BTPOP;
  return newInstance;
}

#define String struct private_stringStruct*

int String_calculateHashCode(String this) {
  int code, i;

  ___BTPUSH;
  i = 0;
  code = 0;
  while(this->buf[i] != 0) {
    code += (unsigned char)this->buf[i];
    ++i;
  }
  ___BTPOP;
  return HashCode.get(code);
}

void* String_constructor(TypeData typeData, ...) {
  String newInstance;
  const char* sPtr;
  int argc, len;
  va_list argp;
  char* dPtr;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  va_start(argp, typeData);
  argc = va_arg(argp, int);
  /* The case argc=0 is intended for constructors of types that extend String. */
  if(argc != 0) {
    sPtr = va_arg(argp, char*);
    if(argc == 1) {
      memReserve(sPtr);
      newInstance->buf = sPtr;
      len = Array.bytezLength(sPtr) - 1;
    } else {
      /* Assume argc=2. */
      len = va_arg(argp, int);
      dPtr = memAllocate("char*", len + 1);
      Array.copyBytes(dPtr, 0, sPtr, 0, len);
      dPtr[len] = 0;
      newInstance->buf = dPtr;
    }
    newInstance->hashCode = String_calculateHashCode(newInstance);
    newInstance->length = len;
  }
  va_end(argp); /* ANSI C requirement. */
  ___BTPOP;
  return newInstance;
}

void String_dump(String this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    memRelease(this->buf);
  }
  Object_dump((Object)this);
  ___BTPOP;
}

gvboolean String_equals(String this, Object other) {
	gvboolean result;
  int i;

  ___BTPUSH;
  if((void*)other == (void*)this) {
    ___BTPOP;
    return true;
  }
  if(!assignableFrom(STRING_TYPE_DATA, other->call->rtType())) {
    ___BTPOP;
    return false;
  }
  i = 0;
#define other ((String)other)
  result = (this->length == other->length) && (this->call->hashCode((void*)this) == other->call->hashCode((void*)other));
  while(i < this->length  &&  result) {
    result = this->buf[i] == other->buf[i];
    ++i;
  }
#undef other
  ___BTPOP;
  return result;
}

int String_hashCode(String this) {
  int hashCode;

  ___BTPUSH;
  hashCode = this->hashCode;
  ___BTPOP;
  return hashCode;
}

TypeData String_rtType(void) {
  return STRING_TYPE_DATA;
}

void predefString_doesNothing(String this){}

int predefString_hashCode(String this) {
  int hashCode;

  ___BTPUSH;
  if(this->hashCode == -1) {
    this->hashCode = String_calculateHashCode(this);
  }
  hashCode = this->hashCode;
  ___BTPOP;
  return hashCode;
}
