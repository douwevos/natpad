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

#ifndef ____ELEM_INFO_INCLUDED
#define ____ELEM_INFO_INCLUDED

typedef struct elemInfoStruct* ElemInfo;

#define ELEM_INFO_TYPE_DATA ptrof(elemInfoTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/String.h>
#include <gvlib/TypeData.h>

struct elemInfoFunctionsStruct {
  void (* dump)(ElemInfo this);
  gvboolean (* equals)(ElemInfo this, Object other);
  int (* hashCode)(ElemInfo this);
  void (* reserve)(ElemInfo this);
  TypeData (* rtType)(void);
};

struct elemInfoStruct {
  readonly refCount_t refCount;
  const struct elemInfoFunctionsStruct* const call;
  const String element;
  const int errwarn;
  const int code;
};

extern const struct typeDataStruct elemInfoTypeDataStruct;

ElemInfo newElemInfo(String element, int code, int errwarn);

#endif
