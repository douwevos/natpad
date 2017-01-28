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

#ifndef ____FILE_OUTPUT_STREAM_INCLUDED
#define ____FILE_OUTPUT_STREAM_INCLUDED

typedef struct fileOutputStreamStruct* FileOutputStream;

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"

struct fileOutputStreamFunctionsStruct {
  /* The stream should be closed before calling dump. */
  void (* dump)(FileOutputStream this);
  gvboolean (* equals)(FileOutputStream this, Object other);
  int (* hashCode)(FileOutputStream this);
  void (* reserve)(FileOutputStream this);
  TypeData (* rtType)(void);

  /* Automatically flushes and closes this stream, which means that the
     connection between the stream and the file is broken. After that, no more
     operations can be performed on the stream.
     Returns -1 if an error occurs, or zero otherwise. */
  int (* close)(FileOutputStream this);
  /* Force all accumulated data out;
     returns -1 if an error occurs, or zero otherwise. */
  int (* flush)(FileOutputStream this);
  /* Writes the least significant byte of the specified value to this stream;
     returns -1 if an error occurs, or zero otherwise. */
  int (* write)(FileOutputStream this, int val);
};

struct fileOutputStreamStruct {
  readonly refCount_t refCount;
  const struct fileOutputStreamFunctionsStruct* const call;
  readonly long int errorCode;
};

FileOutputStream newFileOutputStream(const char* fileName, gvboolean append);

#endif
