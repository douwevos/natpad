/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

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

#ifndef ____STRING_INCLUDED
#define ____STRING_INCLUDED

typedef struct stringStruct* String;

#include "defs.h"
#include "Object.h"
#include "TypeData.h"

struct stringFunctionsStruct {
  void (* dump)(String this);
  gvboolean (* equals)(String this, Object other);
  int (* hashCode)(String this);
  void (* reserve)(String this);
  TypeData (* rtType)(void);
};

struct stringStruct {
  readonly refCount_t refCount;
  const struct stringFunctionsStruct* const call;
  const char* const buf;
  const int length;
};

String newString(const char* str, ...);

/*   String newString can be called in two different ways:

  1. String newString(const char* str);
     Wraps the specified char-array into a string object.
  2. String newString(const char* str, int len);
     Wraps the first 'len' chars of array 'str' into a string object. */
#define newString(char_ptr, ...) newString(char_ptr, ## __VA_ARGS__, -1)

#endif
