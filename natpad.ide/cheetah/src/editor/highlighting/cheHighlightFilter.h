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

#ifndef ___STA_HIGHLIGHT_FILTER_INCLUDED
#define ___STA_HIGHLIGHT_FILTER_INCLUDED

#include <gvlcodeparsing/Token.h>
#include <gvlcodeparsing/TokenList.h>

const extern struct cheHighlightFilterStruct {
  TokenList (* filter)(const Token* tokens, int length);
} CheHighlightFilter;

#endif
