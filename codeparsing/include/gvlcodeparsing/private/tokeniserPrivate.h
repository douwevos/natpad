/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2011 Gerard Visser.

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

#ifndef ___TOKENISER_PRIVATE_INCLUDED
#define ___TOKENISER_PRIVATE_INCLUDED

#include <gvlib/private/objectPrivate.h>
#include <gvlib/StringBuilder.h>
#include "../Token.h"
#include "../Tokeniser.h"

#define Tokeniser struct private_tokeniserStruct*

struct private_tokeniserStruct;

struct private_tokeniserFunctionsStruct {
  void (* dump)(Tokeniser this);
  gvboolean (* equals)(Tokeniser this, Object other);
  int (* hashCode)(Tokeniser this);
  void (* reserve)(Tokeniser this);
  TypeData (* rtType)(void);

  TokenList (* getTokens)(Tokeniser this, PushbackStream stream);
  void (* resetState)(Tokeniser this);
};

struct private_tokeniserStruct {
  readonly refCount_t refCount;
  const struct private_tokeniserFunctionsStruct* const call;

  Token (* nextToken)(Tokeniser this, PushbackStream stream, int cr);
  StringBuilder stringBuilder;
  int newLine;
  int column;
  int line;
};


    void* Tokeniser_constructor(TypeData typeData, ...);
     void Tokeniser_dump(Tokeniser this);
TokenList Tokeniser_getTokens(Tokeniser this, PushbackStream stream);
     void Tokeniser_resetState(Tokeniser this);

#endif
