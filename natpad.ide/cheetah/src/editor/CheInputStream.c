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

#include <gvlib/private/inputStreamPrivate.h>
#include <gvlib/errors.h>

#include <caterpillar.h>
#include "CheInputStream.h"

#define CHE_INPUT_STREAM_TYPE_DATA ptrof(cheInputStreamTypeDataStruct)

struct private_cheInputStreamStruct {
  readonly refCount_t refCount;
  const struct cheInputStreamFunctionsStruct* const call;
  long int errorCode;
  CatIUtf8Scanner* scanner;
  CatStreamStatus status;
};

static void* constructor(TypeData typeData, ...);
static void dump(struct private_cheInputStreamStruct* this);
static int ___read(struct private_cheInputStreamStruct* this);
static TypeData rtType(void);

static const struct cheInputStreamFunctionsStruct cheInputStreamFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,
  (void*)___read
};

static const struct typeDataStruct cheInputStreamTypeDataStruct = {
  "CheInputStream",
  constructor,
  ptrof(cheInputStreamFunctions),
  sizeof(struct private_cheInputStreamStruct),
  INPUT_STREAM_TYPE_DATA
};

CheInputStream newCheInputStream(ChaRevisionWo* a_revision) {
  CheInputStream newInstance;

  ___BTPUSH;
  newInstance = constructor(CHE_INPUT_STREAM_TYPE_DATA, a_revision);
  ___BTPOP;
  return newInstance;
}

#define CheInputStream struct private_cheInputStreamStruct*

static void* constructor(TypeData typeData, ...) {
  CheInputStream newInstance;
  ChaRevisionWo* a_revision;
  va_list argp;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  va_start(argp, typeData);
  a_revision = va_arg(argp, void*);
  va_end(argp);
  newInstance->errorCode = 0;
  newInstance->status = CAT_STREAM_OK;
  newInstance->scanner = CAT_IUTF8_SCANNER(cha_revision_reader_new(a_revision, NULL, NULL));
  ___BTPOP;
  return newInstance;
}

static void dump(CheInputStream this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    cat_unref(this->scanner);
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static int ___read(CheInputStream this) {
  int result;

  ___BTPUSH;
  result = cat_iutf8_scanner_next_char(this->scanner, ptrof(this->status));
  if (this->status != CAT_STREAM_OK) {
    result = -1;
  }
  ___BTPOP;
  return result;
}

static TypeData rtType(void) {
  return CHE_INPUT_STREAM_TYPE_DATA;
}
