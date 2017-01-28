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

#ifndef ___NUMBER_TOKEN_PRIVATE_INCLUDED
#define ___NUMBER_TOKEN_PRIVATE_INCLUDED

#include "tokenPrivate.h"
#include "../NumberToken.h"

#define ARGS_INT_INT_STREAM_INT_STRINGBUILDER 1
#define ARGS_STRING                           2

/* There are two ways to call this constructor: the second argument, an int,
   tells what the remaining arguments are. One of the two macros
   ARGS_INT_INT_STREAM_INT_STRINGBUILDER or ARGS_STRING can be used for that
   purpose.
   1) void* NumberToken_constructor(TypeData, int, int, int, PushbackStream, int, StringBuilder)
      here, the second argument should be ARGS_INT_INT_STREAM_INT_STRINGBUILDER.
   2) void* NumberToken_constructor(TypeData, int, String)
      here, the second argument should be ARGS_STRING.          */
void* NumberToken_constructor(TypeData typeData, ...);

#endif
