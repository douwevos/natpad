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
#include "../include/gvlib/io/FileOutputStream.h"
#include "../include/gvlib/io/ioErrorCodes.h"
#include "../include/gvlib/array.h"
#include "../include/gvlib/errors.h"
#include <errno.h>
#ifdef DEBUG_MODE
# include "../include/gvlib/mem.h"
#endif

#define BUF_SIZE 0x1000

#define FILE_OUTPUT_STREAM_TYPE_DATA ptrof(fileOutputStreamTypeDataStruct)

struct private_fileOutputStreamStruct {
  readonly refCount_t refCount;
  const struct fileOutputStreamFunctionsStruct* const call;
  long int errorCode;
  FILE* handle;
  byte* buf;
  int index;
  gvboolean closed;
};

static int ___close(struct private_fileOutputStreamStruct* this);
static void* constructor(TypeData typeData, ...);
static void dump(struct private_fileOutputStreamStruct* this);
static int flush(struct private_fileOutputStreamStruct* this);
static TypeData rtType(void);
static void setErrorCode(struct private_fileOutputStreamStruct* this);
static int ___write(struct private_fileOutputStreamStruct* this, int val);
static int writeBuffer(struct private_fileOutputStreamStruct* this);

static const struct fileOutputStreamFunctionsStruct fileOutputStreamFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,

  (void*)___close,
  (void*)flush,
  (void*)___write
};

static const struct typeDataStruct fileOutputStreamTypeDataStruct = {
  "FileOutputStream",
  constructor,
  ptrof(fileOutputStreamFunctions),
  sizeof(struct private_fileOutputStreamStruct),
  OUTPUT_STREAM_TYPE_DATA
};

FileOutputStream newFileOutputStream(const char* fileName, gvboolean append) {
  FileOutputStream newInstance;

  ___BTPUSH;
  newInstance = constructor(FILE_OUTPUT_STREAM_TYPE_DATA, fileName, append);
  ___BTPOP;
  return newInstance;
}

#ifdef DEBUG_MODE
# define CHECK_WHETHER_STREAM_OPEN \
  if(this->closed) { \
    Errors.printMessageAndExit("Stream not open."); \
  }
#else
# define CHECK_WHETHER_STREAM_OPEN
#endif

#define FileOutputStream struct private_fileOutputStreamStruct*

static int ___close(FileOutputStream this) {
  int result;

  ___BTPUSH;
  result = flush(this);
  fclose(this->handle);
  this->closed = true;
  ___BTPOP;
  return result;
}

static void* constructor(TypeData typeData, ...) {
  FileOutputStream newInstance;
  const char* fileName;
  gvboolean append;
  va_list argp;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  va_start(argp, typeData);
  fileName = va_arg(argp, void*);
  append = va_arg(argp, int);
  va_end(argp);

  if(append) {
    newInstance->handle = fopen(fileName, "ab");
  } else {
    newInstance->handle = fopen(fileName, "wb");
  }
  if(newInstance->handle == NULL) {
    newInstance->buf = NULL;
    newInstance->closed = true;
    setErrorCode(newInstance);
  } else {
    newInstance->errorCode = 0;
#ifdef DEBUG_MODE
    newInstance->buf = memAllocate("char*", BUF_SIZE);
#else
    newInstance->buf = malloc(BUF_SIZE);
#endif
    newInstance->closed = false;
  }
  newInstance->index = 0;
  ___BTPOP;
  return newInstance;
}

static void dump(FileOutputStream this) {
  ___BTPUSH;
  if(this->refCount == 1  &&  this->handle != NULL) {
#ifdef DEBUG_MODE
    if(!this->closed) {
      fclose(this->handle);
      Errors.printMessageAndExit("Stream wasn't closed.");
    }
    memRelease(this->buf);
#else
    free((void*)this->buf);
#endif
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static int flush(FileOutputStream this) {
  int result;

  ___BTPUSH;
  CHECK_WHETHER_STREAM_OPEN;
  if(this->index > 0) {
    result = writeBuffer(this);
  } else {
    result = 0;
  }
  if(result == 0  &&  fflush(this->handle) == EOF) {
    setErrorCode(this);
    result = -1;
  }
  ___BTPOP;
  return result;
}

static TypeData rtType(void) {
  return FILE_OUTPUT_STREAM_TYPE_DATA;
}

static void setErrorCode(FileOutputStream this) {
  ___BTPUSH;
  switch(errno) {
  case EACCES: {
    this->errorCode = IO_ERR__NO_WRITE_PERMISSION;
  } break;

  case EIO: {
    this->errorCode = IO_ERR__HARDWARE_ERROR;
  } break;

  case EISDIR: {
    this->errorCode = IO_ERR__IS_DIRECTORY;
  } break;

  case EMFILE: {
    this->errorCode = IO_ERR__TOO_MANY_STREAMS_OPEN;
  } break;

  case ENOSPC: {
    this->errorCode = IO_ERR__NO_DISK_SPACE_LEFT;
  } break;

  default: {
    this->errorCode = IO_ERR__UNKNOWN_ERROR;
  }
  }
  ___BTPOP;
}

static int ___write(FileOutputStream this, int val) {
  int result;

  ___BTPUSH;
  CHECK_WHETHER_STREAM_OPEN;
  if(this->index == BUF_SIZE) {
    result = writeBuffer(this);
  } else {
    result = 0;
  }
  /* TODO: Maybe close stream in case of an error?!
           But what about EINTR/EAGAIN errors? */
  this->buf[this->index] = (byte)val;
  ++this->index;
  ___BTPOP;
  return result;
}

static int writeBuffer(FileOutputStream this) {
  int numberOfBytesWritten, result;

  ___BTPUSH;
  numberOfBytesWritten = fwrite(this->buf, 1, this->index, this->handle);
  if(numberOfBytesWritten < this->index) {
    if(numberOfBytesWritten > 0) {
      this->index -= numberOfBytesWritten;
      Array.copyBytes(this->buf, 0, this->buf, numberOfBytesWritten, this->index);
    }
    setErrorCode(this);
    result = -1;
  } else {
    this->index = 0;
    result = 0;
  }
  ___BTPOP;
  return result;
}
