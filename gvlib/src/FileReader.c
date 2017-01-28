/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2010, 2011 Gerard Visser.

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

#include "../include/gvlib/errors.h"
#include "../include/gvlib/io/FileInputStream.h"
#include "../include/gvlib/io/FileReader.h"
#include "../include/gvlib/io/ioErrorCodes.h"
#include "../include/gvlib/io/Reader.h"
#include "../include/gvlib/private/objectPrivate.h"

#define FileReader struct private_fileReaderStruct*

struct private_fileReaderStruct {
  readonly refCount_t refCount;
  const struct fileReaderFunctionsStruct* const call;
  long int errorCode;
  void (* read)(FileReader this, byte* dstPtr, int dstSize, int srcSize);
  FileInputStream stream;
};

static void* constructor(TypeData typeData, ...);
static void dump(FileReader this);
static void read_bb(FileReader this, byte* dstPtr, int dstSize, int srcSize);
static void read_bl(FileReader this, byte* dstPtr, int dstSize, int srcSize);
static void read_lb(FileReader this, byte* dstPtr, int dstSize, int srcSize);
static void read_ll(FileReader this, byte* dstPtr, int dstSize, int srcSize);
static gvboolean readBoolean(FileReader this, gvboolean* dstPtr);
static gvboolean readByte(FileReader this, byte* dstPtr);
static gvboolean readDouble(FileReader this, double* dstPtr);
static gvboolean readFloat(FileReader this, float* dstPtr);
static gvboolean readInt(FileReader this, int* dstPtr);
static gvboolean readInt24(FileReader this, int* dstPtr);
static gvboolean readInt64(FileReader this, int64_t* dstPtr);
static gvboolean readShort(FileReader this, short int* dstPtr);
static gvboolean readString(FileReader this, StringBuilder dst, int len);
static TypeData rtType(void);

#undef FileReader

static const struct fileReaderFunctionsStruct fileReaderFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,
  (void*)readBoolean,
  (void*)readByte,
  (void*)readDouble,
  (void*)readFloat,
  (void*)readInt,
  (void*)readInt24,
  (void*)readInt64,
  (void*)readShort,
  (void*)readString
};

const struct typeDataStruct fileReaderTypeDataStruct = {
  "FileReader",
  constructor,
  ptrof(fileReaderFunctions),
  sizeof(struct private_fileReaderStruct),
  READER_TYPE_DATA
};

FileReader newFileReader(const char* fileName, byteOrder_t byteOrder) {
  FileReader newInstance;

  ___BTPUSH;
  newInstance = constructor(FILE_READER_TYPE_DATA, fileName, byteOrder);
  ___BTPOP;
  return newInstance;
}

#define FileReader struct private_fileReaderStruct*

static void* constructor(TypeData typeData, ...) {
  FileReader newInstance;
  byteOrder_t byteOrder;
  const char* fileName;
  va_list args;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  va_start(args, typeData);
  fileName = va_arg(args, void*);
  byteOrder = va_arg(args, int);
  va_end(args);
  if(getMachineByteOrder() == little_endian) {
    if(byteOrder == little_endian) {
      newInstance->read = read_ll;
    } else {
      newInstance->read = read_lb;
    }
  } else if(byteOrder == little_endian) {
    newInstance->read = read_bl;
  } else {
    newInstance->read = read_bb;
  }
  newInstance->stream = newFileInputStream(fileName);
  newInstance->errorCode = newInstance->stream->errorCode;
  ___BTPOP;
  return newInstance;
}

static void dump(FileReader this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    this->stream->call->dump(this->stream);
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static int read(FileReader this) {
  int result;

  ___BTPUSH;
  if(this->errorCode == 0) {
    result = this->stream->call->read(this->stream);
    if(result == -1) {
      this->errorCode = this->stream->errorCode;
      if(this->errorCode == 0) {
        this->errorCode = IO_ERR__EOF;
      }
    }
  } else {
    result = -1;
  }
  ___BTPOP;
  return result;
}

/*
Example (BB):
     address:  0  1  2  3
dst (size=4): 00 12 34 56
src (size=3): 12 34 56
 */
static void read_bb(FileReader this, byte* dstPtr, int dstSize, int srcSize) {
  int i;

  ___BTPUSH;
  i = 0;
  while(i < dstSize - srcSize) {
    dstPtr[i] = 0;
    ++i;
  }
  while(i < dstSize) {
    dstPtr[i] = read(this);
    ++i;
  }
  ___BTPOP;
}

/*
Example (BL):
     address:  0  1  2  3
dst (size=4): 00 12 34 56
src (size=3): 56 34 12
 */
static void read_bl(FileReader this, byte* dstPtr, int dstSize, int srcSize) {
  int i;

  ___BTPUSH;
  i = dstSize - 1;
  while(i >= dstSize - srcSize) {
    dstPtr[i] = read(this);
    --i;
  }
  while(i > -1) {
    dstPtr[i] = 0;
    --i;
  }
  ___BTPOP;
}

/*
Example (LB):
     address:  0  1  2  3
dst (size=4): 56 34 12 00
src (size=3): 12 34 56
 */
static void read_lb(FileReader this, byte* dstPtr, int dstSize, int srcSize) {
  int i;

  ___BTPUSH;
  i = dstSize - 1;
  while(i >= srcSize) {
    dstPtr[i] = 0;
    --i;
  }
  while(i > -1) {
    dstPtr[i] = read(this);
    --i;
  }
  ___BTPOP;
}

/*
Example (LL):
     address:  0  1  2  3
dst (size=4): 56 34 12 00
src (size=3): 56 34 12
 */
static void read_ll(FileReader this, byte* dstPtr, int dstSize, int srcSize) {
  int i;

  ___BTPUSH;
  i = 0;
  while(i < srcSize) {
    dstPtr[i] = read(this);
    ++i;
  }
  while(i < dstSize) {
    dstPtr[i] = 0;
    ++i;
  }
  ___BTPOP;
}

static gvboolean readBoolean(FileReader this, gvboolean* dstPtr) {
  ___BTPUSH;
  dstPtr[0] = read(this) != 0;
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean readByte(FileReader this, byte* dstPtr) {
  ___BTPUSH;
  dstPtr[0] = read(this);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean readDouble(FileReader this, double* dstPtr) {
  ___BTPUSH;
  this->read(this, (void*)dstPtr, 8, 8);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean readFloat(FileReader this, float* dstPtr) {
  ___BTPUSH;
  this->read(this, (void*)dstPtr, 4, 4);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean readInt(FileReader this, int* dstPtr) {
  ___BTPUSH;
  this->read(this, (void*)dstPtr, 4, 4);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean readInt24(FileReader this, int* dstPtr) {
  ___BTPUSH;
  this->read(this, (void*)dstPtr, 4, 3);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean readInt64(FileReader this, int64_t* dstPtr) {
  ___BTPUSH;
  this->read(this, (void*)dstPtr, 8, 8);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean readShort(FileReader this, short int* dstPtr) {
  ___BTPUSH;
  this->read(this, (void*)dstPtr, 2, 2);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean readString(FileReader this, StringBuilder dst, int len) {
  int i;

  ___BTPUSH;
  if(len > 0) {

    for(i=0; i<len; ++i) {
      dst->call->append(dst, read(this));
    }

  } else {

    i = read(this);
    while(this->errorCode == 0  &&  i != 0) {
      dst->call->append(dst, i);
      i = read(this);
    }

  }
  ___BTPOP;
  return this->errorCode != 0;
}

static TypeData rtType(void) {
  return FILE_READER_TYPE_DATA;
}
