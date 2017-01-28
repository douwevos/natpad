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

#ifndef ____FILE_READER_INCLUDED
#define ____FILE_READER_INCLUDED

typedef struct fileReaderStruct* FileReader;

#define FILE_READER_TYPE_DATA ptrof(fileReaderTypeDataStruct)

#include "../defs.h"
#include "../Object.h"
#include "../StringBuilder.h"
#include "../TypeData.h"
#include "byteOrder.h"

struct fileReaderFunctionsStruct {
  void (* dump)(FileReader this);
  gvboolean (* equals)(FileReader this, Object other);
  int (* hashCode)(FileReader this);
  void (* reserve)(FileReader this);
  TypeData (* rtType)(void);

  /* All of the read methods return whether an error occurred, if so
     see 'errorCode' and 'ioErrorCodes.h'. */

  gvboolean (* readBoolean)(FileReader this, gvboolean* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readByte)(FileReader this, byte* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readDouble)(FileReader this, double* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readFloat)(FileReader this, float* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readInt)(FileReader this, int* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readInt24)(FileReader this, int* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readInt64)(FileReader this, int64_t* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readShort)(FileReader this, short int* dstPtr) __attribute__ ((warn_unused_result));

  /* Tries to read 'len' bytes from the underlying stream and appends them
     to 'dst'. If 'len < 1' then this method reads until zero is read. */
  gvboolean (* readString)(FileReader this, StringBuilder dst, int len) __attribute__ ((warn_unused_result));
};

struct fileReaderStruct {
  readonly refCount_t refCount;
  const struct fileReaderFunctionsStruct* const call;
  readonly long int errorCode;
};

extern const struct typeDataStruct fileReaderTypeDataStruct;

FileReader newFileReader(const char* fileName, byteOrder_t byteOrder);

#endif
