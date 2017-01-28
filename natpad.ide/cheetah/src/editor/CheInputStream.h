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

#ifndef ____CHE_INPUT_STREAM_INCLUDED
#define ____CHE_INPUT_STREAM_INCLUDED

typedef struct cheInputStreamStruct* CheInputStream;

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/TypeData.h>

#include <chameleon.h>

struct cheInputStreamFunctionsStruct {
  void (* dump)(CheInputStream this);
  gvboolean (* equals)(CheInputStream this, Object other);
  int (* hashCode)(CheInputStream this);
  void (* reserve)(CheInputStream this);
  TypeData (* rtType)(void);

  /* Reads and returns the next character from this input stream;
     if no more characters are available then -1 will be returned. */
  int (* read)(CheInputStream this);
};

struct cheInputStreamStruct {
  readonly refCount_t refCount;
  const struct cheInputStreamFunctionsStruct* const call;
  readonly long int errorCode;
};

CheInputStream newCheInputStream(ChaRevisionWo* a_revision);

#endif
