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

#ifndef ____TYPE_DATA_INCLUDED
#define ____TYPE_DATA_INCLUDED

typedef const struct typeDataStruct* TypeData;

#include "defs.h"

struct typeDataStruct {
  const char* name;
  void* (* constructor)(TypeData typeData, ...);
  const void* call;
  const size_t size;
  TypeData superTypeData;
};

gvboolean assignableFrom(TypeData subject, TypeData object);

#endif
