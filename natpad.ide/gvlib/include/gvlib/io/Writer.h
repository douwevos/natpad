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

#ifndef ____WRITER_INCLUDED
#define ____WRITER_INCLUDED

/* Interface for the type Writer. */

typedef struct writerStruct* Writer;

#define WRITER_TYPE_DATA ptrof(writerTypeDataStruct)

#include "../defs.h"
#include "../Object.h"
#include "../String.h"
#include "../TypeData.h"

struct writerFunctionsStruct {
  /* The writer should be closed before calling dump. */
  void (* dump)(Writer this);
  gvboolean (* equals)(Writer this, Object other);
  int (* hashCode)(Writer this);
  void (* reserve)(Writer this);
  TypeData (* rtType)(void);

  /* All of the methods specific for the type Writer return whether an error
     occurred, if so see 'errorCode' and 'ioErrorCodes.h'. */

  /* Automatically flushes and closes the underlying stream, which means that the
     connection between the stream and the file is broken. After that, no more
     write operations can be performed. */
  gvboolean (* close)(Writer this) __attribute__ ((warn_unused_result));
  /* Forces all accumulated data out. */
  gvboolean (* flush)(Writer this) __attribute__ ((warn_unused_result));
  gvboolean (* writeBoolean)(Writer this, gvboolean val) __attribute__ ((warn_unused_result));
  gvboolean (* writeByte)(Writer this, byte val) __attribute__ ((warn_unused_result));
  gvboolean (* writeChars)(Writer this, const char* val) __attribute__ ((warn_unused_result));
  gvboolean (* writeDouble)(Writer this, double val) __attribute__ ((warn_unused_result));
  gvboolean (* writeFloat)(Writer this, float val) __attribute__ ((warn_unused_result));
  gvboolean (* writeInt)(Writer this, int val) __attribute__ ((warn_unused_result));
  gvboolean (* writeInt24)(Writer this, int val) __attribute__ ((warn_unused_result));
  gvboolean (* writeInt64)(Writer this, int64_t val) __attribute__ ((warn_unused_result));
  gvboolean (* writeShort)(Writer this, short int val) __attribute__ ((warn_unused_result));
  gvboolean (* writeString)(Writer this, String val) __attribute__ ((warn_unused_result));
};

struct writerStruct {
  readonly refCount_t refCount;
  const struct writerFunctionsStruct* const call;
  readonly long int errorCode;
};

extern const struct typeDataStruct writerTypeDataStruct;

#endif
