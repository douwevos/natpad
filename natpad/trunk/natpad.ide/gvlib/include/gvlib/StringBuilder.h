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

#ifndef ____STRING_BUILDER_INCLUDED
#define ____STRING_BUILDER_INCLUDED

typedef struct stringBuilderStruct* StringBuilder;

#include "defs.h"
#include "Object.h"
#include "String.h"
#include "TypeData.h"

struct stringBuilderFunctionsStruct {
  void (* dump)(StringBuilder this);
  gvboolean (* equals)(StringBuilder this, Object other);
  int (* hashCode)(StringBuilder this);
  void (* reserve)(StringBuilder this);
  TypeData (* rtType)(void);

  void (* append)(StringBuilder this, char val);
  void (* appendRange)(StringBuilder this, const char* sPtr);
  void (* appendString)(StringBuilder this, String val);
  void (* clear)(StringBuilder this);
  void (* markUndoPosition)(StringBuilder this);
  String (* toString)(StringBuilder this);
  void (* undo)(StringBuilder this);
};

struct stringBuilderStruct {
  readonly refCount_t refCount;
  const struct stringBuilderFunctionsStruct* const call;
  readonly char* readonly buf;
  readonly int length;
};

StringBuilder newStringBuilder(int initialCapacity);

#endif
