/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2011 Gerard Visser.

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
#include "../include/gvlib/io/FileOutputStream.h"
#include "../include/gvlib/io/FileWriter.h"
#include "../include/gvlib/io/Writer.h"
#include "../include/gvlib/private/objectPrivate.h"

#define FileWriter struct private_fileWriterStruct*

struct private_fileWriterStruct {
  readonly refCount_t refCount;
  const struct fileWriterFunctionsStruct* const call;
  long int errorCode;
  void (* write)(FileWriter this, int dstSize, byte* srcPtr, int srcSize);
  FileOutputStream stream;
};

static gvboolean close(FileWriter this);
static void* constructor(TypeData typeData, ...);
static void dump(FileWriter this);
static gvboolean flush(FileWriter this);
static TypeData rtType(void);
static void write_bb(FileWriter this, int dstSize, byte* srcPtr, int srcSize);
static void write_bl(FileWriter this, int dstSize, byte* srcPtr, int srcSize);
static void write_lb(FileWriter this, int dstSize, byte* srcPtr, int srcSize);
static void write_ll(FileWriter this, int dstSize, byte* srcPtr, int srcSize);
static gvboolean writeBoolean(FileWriter this, gvboolean val);
static gvboolean writeByte(FileWriter this, byte val);
static gvboolean writeChars(FileWriter this, const char* val);
static gvboolean writeDouble(FileWriter this, double val);
static gvboolean writeFloat(FileWriter this, float val);
static gvboolean writeInt(FileWriter this, int val);
static gvboolean writeInt24(FileWriter this, int val);
static gvboolean writeInt64(FileWriter this, int64_t val);
static gvboolean writeShort(FileWriter this, short int val);
static gvboolean writeString(FileWriter this, String val);

#undef FileWriter

static const struct fileWriterFunctionsStruct fileWriterFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,
  (void*)close,
  (void*)flush,
  (void*)writeBoolean,
  (void*)writeByte,
  (void*)writeChars,
  (void*)writeDouble,
  (void*)writeFloat,
  (void*)writeInt,
  (void*)writeInt24,
  (void*)writeInt64,
  (void*)writeShort,
  (void*)writeString
};

const struct typeDataStruct fileWriterTypeDataStruct = {
  "FileWriter",
  constructor,
  ptrof(fileWriterFunctions),
  sizeof(struct private_fileWriterStruct),
  WRITER_TYPE_DATA
};

FileWriter newFileWriter(const char* fileName, byteOrder_t byteOrder) {
  FileWriter newInstance;

  ___BTPUSH;
  newInstance = constructor(FILE_WRITER_TYPE_DATA, fileName, byteOrder);
  ___BTPOP;
  return newInstance;
}

#define FileWriter struct private_fileWriterStruct*

static gvboolean close(FileWriter this) {
  int result;

  ___BTPUSH;
  result = this->stream->call->close(this->stream);
  ___BTPOP;
  return result != 0;
}

static void* constructor(TypeData typeData, ...) {
  FileWriter newInstance;
  byteOrder_t byteOrder;
  const char* fileName;
  va_list args;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  va_start(args, typeData);
  fileName = va_arg(args, void*);
  byteOrder = va_arg(args, int);
  va_end(args);
  if(byteOrder == little_endian) {
    if(getMachineByteOrder() == little_endian) {
      newInstance->write = write_ll;
    } else {
      newInstance->write = write_lb;
    }
  } else if(getMachineByteOrder() == little_endian) {
    newInstance->write = write_bl;
  } else {
    newInstance->write = write_bb;
  }
  newInstance->stream = newFileOutputStream(fileName, false);
  newInstance->errorCode = newInstance->stream->errorCode;
  ___BTPOP;
  return newInstance;
}

static void dump(FileWriter this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    this->stream->call->dump(this->stream);
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static gvboolean flush(FileWriter this) {
  int result;

  ___BTPUSH;
  result = this->stream->call->flush(this->stream);
  ___BTPOP;
  return result != 0;
}

static TypeData rtType(void) {
  return FILE_WRITER_TYPE_DATA;
}

static void write(FileWriter this, int val) {
  ___BTPUSH;
  if(this->errorCode == 0) {
    if(this->stream->call->write(this->stream, val) == -1) {
      this->errorCode = this->stream->errorCode;
    }
  }
  ___BTPOP;
}

/*
Example (BB):
     address:  0  1  2  3
dst (size=3): 12 34 56
src (size=4): 00 12 34 56
 */
static void write_bb(FileWriter this, int dstSize, byte* srcPtr, int srcSize) {
  int i;

  ___BTPUSH;
  i = srcSize - dstSize;
  while(i < srcSize) {
    write(this, srcPtr[i]);
    ++i;
  }
  ___BTPOP;
}

/*
Example (BL):
     address:  0  1  2  3
dst (size=3): 12 34 56
src (size=4): 56 34 12 00
 */
static void write_bl(FileWriter this, int dstSize, byte* srcPtr, int srcSize) {
  int i;

  ___BTPUSH;
  i = dstSize - 1;
  while(i > -1) {
    write(this, srcPtr[i]);
    --i;
  }
  ___BTPOP;
}

/*
Example (LB):
     address:  0  1  2  3
dst (size=3): 56 34 12
src (size=4): 00 12 34 56
 */
static void write_lb(FileWriter this, int dstSize, byte* srcPtr, int srcSize) {
  int i;

  ___BTPUSH;
  i = srcSize - 1;
  while(i >= srcSize - dstSize) {
    write(this, srcPtr[i]);
    --i;
  }
  ___BTPOP;
}

/*
Example (LL):
     address:  0  1  2  3
dst (size=3): 56 34 12
src (size=4): 56 34 12 00
 */
static void write_ll(FileWriter this, int dstSize, byte* srcPtr, int srcSize) {
  int i;

  ___BTPUSH;
  i = 0;
  while(i < dstSize) {
    write(this, srcPtr[i]);
    ++i;
  }
  ___BTPOP;
}

static gvboolean writeBoolean(FileWriter this, gvboolean val) {
  ___BTPUSH;
  if(val) {
    write(this, 1);
  } else {
    write(this, 0);
  }
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean writeByte(FileWriter this, byte val) {
  ___BTPUSH;
  write(this, val);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean writeChars(FileWriter this, const char* val) {
  int i;

  ___BTPUSH;
  i = 0;
  while(val[i] != 0) {
    write(this, val[i]);
    ++i;
  }
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean writeDouble(FileWriter this, double val) {
  ___BTPUSH;
  this->write(this, 8, (void*)ptrof(val), 8);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean writeFloat(FileWriter this, float val) {
  ___BTPUSH;
  this->write(this, 4, (void*)ptrof(val), 4);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean writeInt(FileWriter this, int val) {
  ___BTPUSH;
  this->write(this, 4, (void*)ptrof(val), 4);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean writeInt24(FileWriter this, int val) {
  ___BTPUSH;
  this->write(this, 3, (void*)ptrof(val), 4);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean writeInt64(FileWriter this, int64_t val) {
  ___BTPUSH;
  this->write(this, 8, (void*)ptrof(val), 8);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean writeShort(FileWriter this, short int val) {
  ___BTPUSH;
  this->write(this, 2, (void*)ptrof(val), 2);
  ___BTPOP;
  return this->errorCode != 0;
}

static gvboolean writeString(FileWriter this, String val) {
	gvboolean result;

  ___BTPUSH;
  result = writeChars(this, val->buf);
  ___BTPOP;
  return result;
}
