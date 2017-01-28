/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

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

#ifndef ___DEFS_INCLUDED
#define ___DEFS_INCLUDED

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ptrof(x) (&x)

/* For fields that are not constant, but shouldn't be modified directly. */
#define readonly const

#define doubleBitsToInt64(x) ((int64_t*)ptrof(x))[0]
#define int64BitsToDouble(x) ((double*)ptrof(x))[0]

/* See stdbool.h */
#if defined __bool_true_false_are_defined || defined __cplusplus
typedef int gvboolean;
#else
typedef enum {false, true} gvboolean;
#endif

typedef unsigned char byte;

typedef long unsigned int refCount_t;

#endif
