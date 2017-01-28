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

#ifndef ____OUTPUT_STREAM_INCLUDED
#define ____OUTPUT_STREAM_INCLUDED

/* Interface for the type OutputStream. */

typedef struct outputStreamStruct* OutputStream;

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"

struct outputStreamFunctionsStruct {
  /* The stream should be closed before calling dump. */
  void (* dump)(OutputStream this);
  gvboolean (* equals)(OutputStream this, Object other);
  int (* hashCode)(OutputStream this);
  void (* reserve)(OutputStream this);
  TypeData (* rtType)(void);

  /* Automatically flushes and closes this stream, which means that the
     connection between the stream and the file is broken. After that, no more
     operations can be performed on the stream.
     Returns -1 if an error occurs, or zero otherwise. */
  int (* close)(OutputStream this);
  /* Force all accumulated data out;
     returns -1 if an error occurs, or zero otherwise. */
  int (* flush)(OutputStream this);
  /* Writes the specified data unit to this stream;
     returns -1 if an error occurs, or zero otherwise. */
  int (* write)(OutputStream this, int val);
};

struct outputStreamStruct {
  readonly refCount_t refCount;
  const struct outputStreamFunctionsStruct* const call;
  readonly long int errorCode;
};

#endif
