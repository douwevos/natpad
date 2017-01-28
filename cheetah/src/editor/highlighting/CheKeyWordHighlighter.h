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

#ifndef ____CHE_KEY_WORD_HIGHLIGHTER_INCLUDED
#define ____CHE_KEY_WORD_HIGHLIGHTER_INCLUDED

typedef struct cheKeyWordHighlighterStruct* CheKeyWordHighlighter;

#define CHE_KEY_WORD_HIGHLIGHTER_TYPE_DATA ptrof(cheKeyWordHighlighterTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/TypeData.h>
#include <gvlib/io/PushbackStream.h>
#include <gvlcodeparsing/TokenList.h>

struct cheKeyWordHighlighterFunctionsStruct {
  void (* dump)(CheKeyWordHighlighter this);
  gvboolean (* equals)(CheKeyWordHighlighter this, Object other);
  int (* hashCode)(CheKeyWordHighlighter this);
  void (* reserve)(CheKeyWordHighlighter this);
  TypeData (* rtType)(void);

  /* Dumps the specified stream. */
  TokenList (* getTokens)(CheKeyWordHighlighter this, PushbackStream stream);
};

struct cheKeyWordHighlighterStruct {
  readonly refCount_t refCount;
  const struct cheKeyWordHighlighterFunctionsStruct* const call;
};

const struct typeDataStruct cheKeyWordHighlighterTypeDataStruct;

CheKeyWordHighlighter newCheKeyWordHighlighter(void);

#endif
