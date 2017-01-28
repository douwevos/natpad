/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2009, 2010 Gerard Visser.

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

#ifndef ____PUSHBACK_STREAM_INCLUDED
#define ____PUSHBACK_STREAM_INCLUDED

typedef struct pushbackStreamStruct* PushbackStream;

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"
#include "InputStream.h"

struct pushbackStreamFunctionsStruct {
  void (* dump)(PushbackStream this);
  gvboolean (* equals)(PushbackStream this, Object other);
  int (* hashCode)(PushbackStream this);
  void (* reserve)(PushbackStream this);
  TypeData (* rtType)(void);

  /* Reads and returns the next data unit from this input stream;
     if no more data are available then -1 will be returned. */
  int (* read)(PushbackStream this);

  /* Pushes the specified value onto the stream, which means that a
     subsequent read-operation will return the value that was unread.
     Just one value can be unread at a time. The value -1 will be
     ignored. */
  void (* unread)(PushbackStream this, int val);
};

struct pushbackStreamStruct {
  readonly refCount_t refCount;
  const struct pushbackStreamFunctionsStruct* const call;
  readonly long int errorCode;
};

PushbackStream newPushbackStream(InputStream stream);

#endif
