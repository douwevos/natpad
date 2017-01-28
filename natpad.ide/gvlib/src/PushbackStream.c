/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2009, 2010 Gerard Visser.

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

#include "../include/gvlib/private/inputStreamPrivate.h"
#include "../include/gvlib/io/PushbackStream.h"
#include "../include/gvlib/errors.h"

#define PUSHBACK_STREAM_TYPE_DATA ptrof(pushbackStreamTypeDataStruct)

struct private_pushbackStreamStruct {
  readonly refCount_t refCount;
  const struct pushbackStreamFunctionsStruct* const call;
  long int errorCode;
  InputStream stream;
  int buffer;
};

static void* constructor(TypeData typeData, ...);
static void dump(struct private_pushbackStreamStruct* this);
static int read(struct private_pushbackStreamStruct* this);
static TypeData rtType(void);
static void unread(struct private_pushbackStreamStruct* this, int val);

static const struct pushbackStreamFunctionsStruct pushbackStreamFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,

  (void*)read,
  (void*)unread
};

static const struct typeDataStruct pushbackStreamTypeDataStruct = {
  "PushbackStream",
  constructor,
  ptrof(pushbackStreamFunctions),
  sizeof(struct private_pushbackStreamStruct),
  INPUT_STREAM_TYPE_DATA
};

PushbackStream newPushbackStream(InputStream stream) {
  PushbackStream newInstance;

  ___BTPUSH;
  newInstance = constructor(PUSHBACK_STREAM_TYPE_DATA, stream);
  ___BTPOP;
  return newInstance;
}

#define PushbackStream struct private_pushbackStreamStruct*

static void* constructor(TypeData typeData, ...) {
  PushbackStream newInstance;
  InputStream stream;
  va_list argp;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  va_start(argp, typeData);
  stream = va_arg(argp, void*);
  va_end(argp); /* ANSI C requirement. */
  newInstance->errorCode = stream->errorCode;
  newInstance->stream = stream;
  newInstance->buffer = -1;
  ___BTPOP;
  return newInstance;
}

static void dump(PushbackStream this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    this->stream->call->dump(this->stream);
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static int read(PushbackStream this) {
  int result;

  ___BTPUSH;
  if(this->buffer == -1) {
    result = this->stream->call->read(this->stream);
    if(result == -1) {
      this->errorCode = this->stream->errorCode;
    }
  } else {
    result = this->buffer;
    this->buffer = -1;
  }
  ___BTPOP;
  return result;
}

static TypeData rtType(void) {
  return PUSHBACK_STREAM_TYPE_DATA;
}

static void unread(PushbackStream this, int val) {
  ___BTPUSH;
  if(this->errorCode == 0) {
    this->buffer = val;
  }
  ___BTPOP;
}
