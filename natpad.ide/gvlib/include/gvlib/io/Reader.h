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

#ifndef ____READER_INCLUDED
#define ____READER_INCLUDED

/* Interface for the type Reader. */

typedef struct readerStruct* Reader;

#define READER_TYPE_DATA ptrof(readerTypeDataStruct)

#include "../defs.h"
#include "../Object.h"
#include "../StringBuilder.h"
#include "../TypeData.h"

struct readerFunctionsStruct {
  void (* dump)(Reader this);
  gvboolean (* equals)(Reader this, Object other);
  int (* hashCode)(Reader this);
  void (* reserve)(Reader this);
  TypeData (* rtType)(void);

  /* All of the read methods return whether an error occurred, if so
     see 'errorCode' and 'ioErrorCodes.h'. */

  gvboolean (* readBoolean)(Reader this, gvboolean* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readByte)(Reader this, byte* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readDouble)(Reader this, double* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readFloat)(Reader this, float* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readInt)(Reader this, int* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readInt24)(Reader this, int* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readInt64)(Reader this, int64_t* dstPtr) __attribute__ ((warn_unused_result));
  gvboolean (* readShort)(Reader this, short int* dstPtr) __attribute__ ((warn_unused_result));

  /* Tries to read 'len' bytes from the underlying stream and appends them
     to 'dst'. If 'len < 1' then this method reads until zero is read. */
  gvboolean (* readString)(Reader this, StringBuilder dst, int len) __attribute__ ((warn_unused_result));
};

struct readerStruct {
  readonly refCount_t refCount;
  const struct readerFunctionsStruct* const call;
  readonly long int errorCode;
};

extern const struct typeDataStruct readerTypeDataStruct;

#endif
