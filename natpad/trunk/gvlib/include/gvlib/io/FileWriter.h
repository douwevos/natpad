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

#ifndef ____FILE_WRITER_INCLUDED
#define ____FILE_WRITER_INCLUDED

typedef struct fileWriterStruct* FileWriter;

#define FILE_WRITER_TYPE_DATA ptrof(fileWriterTypeDataStruct)

#include "../defs.h"
#include "../Object.h"
#include "../String.h"
#include "../TypeData.h"
#include "byteOrder.h"

struct fileWriterFunctionsStruct {
  /* The writer should be closed before calling dump. */
  void (* dump)(FileWriter this);
  gvboolean (* equals)(FileWriter this, Object other);
  int (* hashCode)(FileWriter this);
  void (* reserve)(FileWriter this);
  TypeData (* rtType)(void);

  /* All of the methods specific for the type Writer return whether an error
     occurred, if so see 'errorCode' and 'ioErrorCodes.h'. */

  /* Automatically flushes and closes the underlying stream, which means that the
     connection between the stream and the file is broken. After that, no more
     write operations can be performed. */
  gvboolean (* close)(FileWriter this) __attribute__ ((warn_unused_result));
  /* Forces all accumulated data out. */
  gvboolean (* flush)(FileWriter this) __attribute__ ((warn_unused_result));
  gvboolean (* writeBoolean)(FileWriter this, gvboolean val) __attribute__ ((warn_unused_result));
  gvboolean (* writeByte)(FileWriter this, byte val) __attribute__ ((warn_unused_result));
  gvboolean (* writeChars)(FileWriter this, const char* val) __attribute__ ((warn_unused_result));
  gvboolean (* writeDouble)(FileWriter this, double val) __attribute__ ((warn_unused_result));
  gvboolean (* writeFloat)(FileWriter this, float val) __attribute__ ((warn_unused_result));
  gvboolean (* writeInt)(FileWriter this, int val) __attribute__ ((warn_unused_result));
  gvboolean (* writeInt24)(FileWriter this, int val) __attribute__ ((warn_unused_result));
  gvboolean (* writeInt64)(FileWriter this, int64_t val) __attribute__ ((warn_unused_result));
  gvboolean (* writeShort)(FileWriter this, short int val) __attribute__ ((warn_unused_result));
  gvboolean (* writeString)(FileWriter this, String val) __attribute__ ((warn_unused_result));
};

struct fileWriterStruct {
  readonly refCount_t refCount;
  const struct fileWriterFunctionsStruct* const call;
  readonly long int errorCode;
};

extern const struct typeDataStruct fileWriterTypeDataStruct;

FileWriter newFileWriter(const char* fileName, byteOrder_t byteOrder);

#endif
