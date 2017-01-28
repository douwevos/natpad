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

#ifndef ___STRING_BUILDER_PRIVATE_INCLUDED
#define ___STRING_BUILDER_PRIVATE_INCLUDED

#include "../StringBuilder.h"
#include "stringPrivate.h"

#define STRING_BUILDER_TYPE_DATA ptrof(stringBuilderTypeDataStruct)

struct private_stringBuilderStruct {
  readonly refCount_t refCount;
  const struct stringBuilderFunctionsStruct* const call;
  char* buf;
  int length;
  int capacity;
  int undoPosition;
  int capacityQuantum;
};

extern const struct typeDataStruct stringBuilderTypeDataStruct;

#define StringBuilder struct private_stringBuilderStruct*

    void StringBuilder_append(StringBuilder this, char val);
    void StringBuilder_appendRange(StringBuilder this, const char* sPtr);
    void StringBuilder_appendString(StringBuilder this, String val);
    void StringBuilder_clear(StringBuilder this);
   void* StringBuilder_constructor(TypeData typeData, ...);
    void StringBuilder_markUndoPosition(StringBuilder this);
TypeData StringBuilder_rtType(void);
  String StringBuilder_toString(StringBuilder this);
    void StringBuilder_undo(StringBuilder this);

#undef StringBuilder

#endif
