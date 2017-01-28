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

#ifndef ____UTF8_OUTPUT_STREAM_INCLUDED
#define ____UTF8_OUTPUT_STREAM_INCLUDED

typedef struct utf8OutputStreamStruct* Utf8OutputStream;

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"
#include "OutputStream.h"

struct utf8OutputStreamFunctionsStruct {
  /* The stream should be closed before calling dump. */
  void (* dump)(Utf8OutputStream this);
  gvboolean (* equals)(Utf8OutputStream this, Object other);
  int (* hashCode)(Utf8OutputStream this);
  void (* reserve)(Utf8OutputStream this);
  TypeData (* rtType)(void);

  /* Flushes and closes the backing stream, which means that the connection
     between the stream and the file is broken. After that, no more operations
     can be performed on the stream.
     Returns -1 if an error occurs, or zero otherwise. */
  int (* close)(Utf8OutputStream this);
  /* Force all accumulated data out;
     returns -1 if an error occurs, or zero otherwise. */
  int (* flush)(Utf8OutputStream this);
  /* Writes the specified character to this stream (ignoring -1);
     returns -1 if an error occurs, or zero otherwise. */
  int (* write)(Utf8OutputStream this, int val);
};

struct utf8OutputStreamStruct {
  readonly refCount_t refCount;
  const struct utf8OutputStreamFunctionsStruct* const call;
  readonly long int errorCode;
};

Utf8OutputStream newUtf8OutputStream(OutputStream stream);

#endif
