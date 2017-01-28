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

#ifndef ___CHAR_INCLUDED
#define ___CHAR_INCLUDED

#include "defs.h"

extern const struct charStruct {
	gvboolean (* isAsciiLetter)(int cr);
	gvboolean (* isBinDigit)(int cr);
	gvboolean (* isDigit)(int cr);
	gvboolean (* isHexDigit)(int cr);
	gvboolean (* isLetter)(int cr);
	gvboolean (* isOctDigit)(int cr);
	gvboolean (* isSpace)(int cr);
	gvboolean (* match)(gvboolean caseSensitive, int crA, int crB);
  void (* put)(int unicode);
  int (* toLower)(int cr);
  int (* toUpper)(int cr);
  int64_t (* toUtf8)(unsigned int cr);
  int (* utf8EncodingSize)(int cr);
  int (* valueOf)(int cr);
} Char;

#endif
