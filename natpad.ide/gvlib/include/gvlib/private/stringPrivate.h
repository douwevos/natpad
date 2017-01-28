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

#ifndef ___STRING_PRIVATE_INCLUDED
#define ___STRING_PRIVATE_INCLUDED

#include "../defs.h"
#include "../Object.h"
#include "../String.h"
#include "../TypeData.h"
#include "objectPrivate.h"

#define STRING_TYPE_DATA ptrof(stringTypeDataStruct)

struct private_stringStruct {
  readonly refCount_t refCount;
  const struct stringFunctionsStruct* const call;
  const char* buf;
  int length;
  int hashCode;
};

extern const struct typeDataStruct stringTypeDataStruct;

extern const struct stringFunctionsStruct predefStringFunctions;

#define String struct private_stringStruct*

     int String_calculateHashCode(String this);
   void* String_constructor(TypeData typeData, ...);
    void String_dump(String this);
    gvboolean String_equals(String this, Object other);
     int String_hashCode(String this);
TypeData String_rtType(void);

    void predefString_doesNothing(String this);
     int predefString_hashCode(String this);

#undef String

#endif
