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

#include "../include/gvlib/private/inputStreamPrivate.h"
#include "../include/gvlib/io/Utf8InputStream.h"
#include "../include/gvlib/io/ioErrorCodes.h"
#include "../include/gvlib/errors.h"

#define UTF8_INPUT_STREAM_TYPE_DATA ptrof(utf8InputStreamTypeDataStruct)

struct private_utf8InputStreamStruct {
  readonly refCount_t refCount;
  const struct utf8InputStreamFunctionsStruct* const call;
  long int errorCode;
  InputStream stream;
};

static const int decodingMask[] = {0x3F, 0x1F, 0x0F, 0x07, 0x03};

static int codeSize(int firstByte);
static void* constructor(TypeData typeData, ...);
static void dump(struct private_utf8InputStreamStruct* this);
static int read(struct private_utf8InputStreamStruct* this);
static TypeData rtType(void);

static const struct utf8InputStreamFunctionsStruct utf8InputStreamFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,

  (void*)read
};

static const struct typeDataStruct utf8InputStreamTypeDataStruct = {
  "Utf8InputStream",
  constructor,
  ptrof(utf8InputStreamFunctions),
  sizeof(struct private_utf8InputStreamStruct),
  INPUT_STREAM_TYPE_DATA
};

Utf8InputStream newUtf8InputStream(InputStream stream) {
  Utf8InputStream newInstance;

  ___BTPUSH;
  newInstance = constructor(UTF8_INPUT_STREAM_TYPE_DATA, stream);
  ___BTPOP;
  return newInstance;
}

#define Utf8InputStream struct private_utf8InputStreamStruct*

/* 0 = error. */
static int codeSize(int firstByte) {
  int mask, result;

  if(firstByte > 0x7F) {
    result = 1;
    mask = 0x40;
    while((firstByte & mask) != 0) {
      mask >>= 1;
      ++result;
    }
    if(result > 6) {
      result = 6;
    } else if(result == 1) {
      result = 0;
    }
  } else {
    result = 1;
  }
  return result;
}

static void* constructor(TypeData typeData, ...) {
  Utf8InputStream newInstance;
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

static void dump(Utf8InputStream this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    this->stream->call->dump(this->stream);
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static int read(Utf8InputStream this) {
  int i, result, sixBits, size;

  ___BTPUSH;
  if(this->errorCode == 0) {
    result = this->stream->call->read(this->stream);
    if(result != -1) {
      size = codeSize(result);
      if(size > 0) {

        if(size > 1) {
          i = 1;
          result &= decodingMask[size - 2];
          while(i < size  &&  result != -1) {
            sixBits = this->stream->call->read(this->stream);
            if((sixBits & 0xC0) != 0x80) {
              this->errorCode = IO_ERR__STREAM_NOT_UTF8_ENCODED;
              result = -1;
            } else {
              result <<= 6;
              result |= sixBits & 0x3F;
            }
            ++i;
          }
        }

      } else {
        this->errorCode = IO_ERR__STREAM_NOT_UTF8_ENCODED;
        result = -1;
      }
    } else {
      this->errorCode = this->stream->errorCode;
    }
  } else {
    result = -1;
  }
  ___BTPOP;
  return result;
}

static TypeData rtType(void) {
  return UTF8_INPUT_STREAM_TYPE_DATA;
}
