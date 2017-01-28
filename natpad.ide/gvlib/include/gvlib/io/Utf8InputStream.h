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

#ifndef ____UTF8_INPUT_STREAM_INCLUDED
#define ____UTF8_INPUT_STREAM_INCLUDED

typedef struct utf8InputStreamStruct* Utf8InputStream;

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"
#include "InputStream.h"

struct utf8InputStreamFunctionsStruct {
  void (* dump)(Utf8InputStream this);
  gvboolean (* equals)(Utf8InputStream this, Object other);
  int (* hashCode)(Utf8InputStream this);
  void (* reserve)(Utf8InputStream this);
  TypeData (* rtType)(void);

  /* Reads and returns the next (wide) character from this input stream;
     if no more characters are available then -1 will be returned. */
  int (* read)(Utf8InputStream this);
};

struct utf8InputStreamStruct {
  readonly refCount_t refCount;
  const struct utf8InputStreamFunctionsStruct* const call;
  readonly long int errorCode;
};

Utf8InputStream newUtf8InputStream(InputStream stream);

#endif
