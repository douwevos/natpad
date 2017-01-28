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

#include "../include/gvlib/private/outputStreamPrivate.h"
#include "../include/gvlib/io/Utf8OutputStream.h"
#include "../include/gvlib/char.h"
#include "../include/gvlib/errors.h"

#define BUF_SIZE 6

#define UTF8_OUTPUT_STREAM_TYPE_DATA ptrof(utf8OutputStreamTypeDataStruct)

struct private_utf8OutputStreamStruct {
  readonly refCount_t refCount;
  const struct utf8OutputStreamFunctionsStruct* const call;
  long int errorCode;
  OutputStream stream;
  int buf[BUF_SIZE];
};

static const int encodingMask[] = {0xC0, 0xE0, 0xF0, 0xF8, 0xFC};

static int close(struct private_utf8OutputStreamStruct* this);
static void* constructor(TypeData typeData, ...);
static void dump(struct private_utf8OutputStreamStruct* this);
static int flush(struct private_utf8OutputStreamStruct* this);
static TypeData rtType(void);
static int write(struct private_utf8OutputStreamStruct* this, int val);

static const struct utf8OutputStreamFunctionsStruct utf8OutputStreamFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,

  (void*)close,
  (void*)flush,
  (void*)write
};

static const struct typeDataStruct utf8OutputStreamTypeDataStruct = {
  "Utf8OutputStream",
  constructor,
  ptrof(utf8OutputStreamFunctions),
  sizeof(struct private_utf8OutputStreamStruct),
  OUTPUT_STREAM_TYPE_DATA
};

Utf8OutputStream newUtf8OutputStream(OutputStream stream) {
  Utf8OutputStream newInstance;

  ___BTPUSH;
  newInstance = constructor(UTF8_OUTPUT_STREAM_TYPE_DATA, stream);
  ___BTPOP;
  return newInstance;
}

#define Utf8OutputStream struct private_utf8OutputStreamStruct*

static int close(Utf8OutputStream this) {
  int result;

  ___BTPUSH;
  result = this->stream->call->close(this->stream);
  this->errorCode = this->stream->errorCode;
  ___BTPOP;
  return result;
}

static void* constructor(TypeData typeData, ...) {
  Utf8OutputStream newInstance;
  va_list argp;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  va_start(argp, typeData);
  newInstance->stream = va_arg(argp, void*);
  va_end(argp);
  newInstance->errorCode = newInstance->stream->errorCode;
  ___BTPOP;
  return newInstance;
}

static void dump(Utf8OutputStream this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    this->stream->call->dump(this->stream);
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static int flush(Utf8OutputStream this) {
  int result;

  ___BTPUSH;
  result = this->stream->call->flush(this->stream);
  this->errorCode = this->stream->errorCode;
  ___BTPOP;
  return result;
}

static TypeData rtType(void) {
  return UTF8_OUTPUT_STREAM_TYPE_DATA;
}

static int write(Utf8OutputStream this, int val) {
  int i, len, result;

  ___BTPUSH;
  if(this->errorCode == 0) {
    if(val != -1) {

      len = Char.utf8EncodingSize(val);
      if(len > 1) {
        --len;
        this->buf[0] = (val & 0x3F) | 0x80;
        val >>= 6;
        for(i=1; i<len; ++i) {
          this->buf[i] = (val & 0x3F) | 0x80;
          val >>= 6;
        }
        --i;
        result = this->stream->call->write(this->stream, (val & 0xFF) | encodingMask[i]);
        while(i > -1  &&  result == 0) {
          result = this->stream->call->write(this->stream, this->buf[i]);
          --i;
        }
      } else {
        result = this->stream->call->write(this->stream, val);
      }
      this->errorCode = this->stream->errorCode;

    } else {
      result = 0;
    }
  } else {
    result = -1;
  }
  ___BTPOP;
  return result;
}
