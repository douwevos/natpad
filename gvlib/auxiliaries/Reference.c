/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2008, 2009, 2010 Gerard Visser.

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

#include "Reference.h"

#ifdef DEBUG_MODE

Reference newReference(const void* pointer, size_t size, const char* typeName) {
  Reference newInstance;

  newInstance = malloc(sizeof(struct referenceStruct));
  newInstance->pointer = pointer;
  newInstance->count = 1;
  newInstance->size = size;
  newInstance->typeName = typeName;
  return newInstance;
}

#else

Reference newReference(const void* pointer) {
  Reference newInstance;

  newInstance = malloc(sizeof(struct referenceStruct));
  newInstance->pointer = pointer;
  newInstance->count = 1;
  return newInstance;
}

#endif
