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

#ifndef ____FILE_INPUT_STREAM_INCLUDED
#define ____FILE_INPUT_STREAM_INCLUDED

typedef struct fileInputStreamStruct* FileInputStream;

#include "../defs.h"
#include "../Object.h"
#include "../TypeData.h"

struct fileInputStreamFunctionsStruct {
  void (* dump)(FileInputStream this);
  gvboolean (* equals)(FileInputStream this, Object other);
  int (* hashCode)(FileInputStream this);
  void (* reserve)(FileInputStream this);
  TypeData (* rtType)(void);

  /* Reads and returns the next byte from this input stream;
     if no more bytes are available then -1 will be returned. */
  int (* read)(FileInputStream this);
};

struct fileInputStreamStruct {
  readonly refCount_t refCount;
  const struct fileInputStreamFunctionsStruct* const call;
  readonly long int errorCode;
};

FileInputStream newFileInputStream(const char* fileName);

#endif
