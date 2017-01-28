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
#include "../include/gvlib/io/FileInputStream.h"
#include "../include/gvlib/io/ioErrorCodes.h"
#include "../include/gvlib/errors.h"
#include <unistd.h>
#ifdef DEBUG_MODE
# include "../include/gvlib/mem.h"
#endif

#define BUF_SIZE 0x1000

#define FILE_INPUT_STREAM_TYPE_DATA ptrof(fileInputStreamTypeDataStruct)

struct private_fileInputStreamStruct {
  readonly refCount_t refCount;
  const struct fileInputStreamFunctionsStruct* const call;
  long int errorCode;
  FILE* handle;
  byte* buf;
  int index;
  int size;
};

static void* constructor(TypeData typeData, ...);
static void dump(struct private_fileInputStreamStruct* this);
static int ___read(struct private_fileInputStreamStruct* this);
static TypeData rtType(void);

static const struct fileInputStreamFunctionsStruct fileInputStreamFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,

  (void*)___read
};

static const struct typeDataStruct fileInputStreamTypeDataStruct = {
  "FileInputStream",
  constructor,
  ptrof(fileInputStreamFunctions),
  sizeof(struct private_fileInputStreamStruct),
  INPUT_STREAM_TYPE_DATA
};

FileInputStream newFileInputStream(const char* fileName) {
  FileInputStream newInstance;

  ___BTPUSH;
  newInstance = constructor(FILE_INPUT_STREAM_TYPE_DATA, fileName);
  ___BTPOP;
  return newInstance;
}

#define FileInputStream struct private_fileInputStreamStruct*

static void* constructor(TypeData typeData, ...) {
  FileInputStream newInstance;
  const char* fileName;
  va_list argp;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  va_start(argp, typeData);
  fileName = va_arg(argp, void*);
  va_end(argp);
  if(access(fileName, F_OK)) {

    newInstance->errorCode = IO_ERR__NO_SUCH_FILE;
    newInstance->handle = NULL;
    newInstance->buf = NULL;
    newInstance->size = 0;

  } else if(access(fileName, R_OK)) {

    newInstance->errorCode = IO_ERR__NO_READ_PERMISSION;
    newInstance->handle = NULL;
    newInstance->buf = NULL;
    newInstance->size = 0;

  } else {

    newInstance->errorCode = 0;
    newInstance->handle = fopen(fileName, "rb");
#ifdef DEBUG_MODE
    newInstance->buf = memAllocate("char*", BUF_SIZE);
#else
    newInstance->buf = malloc(BUF_SIZE);
#endif
    newInstance->size = fread(newInstance->buf, 1, BUF_SIZE, newInstance->handle);

  }
  newInstance->index = 0;
  ___BTPOP;
  return newInstance;
}

static void dump(FileInputStream this) {
  ___BTPUSH;
  if(this->refCount == 1  &&  this->handle != NULL) {
    fclose(this->handle);
#ifdef DEBUG_MODE
    memRelease(this->buf);
#else
    free((void*)this->buf);
#endif
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static int ___read(FileInputStream this) {
  int result;

  ___BTPUSH;
  if(this->index == BUF_SIZE) {
    this->size = fread(this->buf, 1, BUF_SIZE, this->handle);
    this->index = 0;
  }
  if(this->index < this->size) {
    result = this->buf[this->index];
    ++this->index;
  } else {
    result = -1;
  }
  ___BTPOP;
  return result;
}

static TypeData rtType(void) {
  return FILE_INPUT_STREAM_TYPE_DATA;
}
