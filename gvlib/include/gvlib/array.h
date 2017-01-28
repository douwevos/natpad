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

#ifndef ___ARRAY_INCLUDED
#define ___ARRAY_INCLUDED

#include "defs.h"

extern const struct arrayStruct {
   int (* bytezLength)(const byte* sPtr);
  void (* copyBytes)(byte* dPtr, long dOff, const byte* sPtr, long sOff, long len);
  void (* copyInts)(int* dPtr, long dOff, const int* sPtr, long sOff, long len);
  void (* copyPtrs)(const void** dPtr, long dOff, const void* const * sPtr, long sOff, long len);
} Array;

#endif
