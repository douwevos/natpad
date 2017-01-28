/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2009, 2010, 2011 Gerard Visser.

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

#include <gvlib/char.h>
#include <gvlib/errors.h>
#include "../include/gvlcodeparsing/LastToken.h"
#include "../include/gvlcodeparsing/Token.h"
#include "../include/gvlcodeparsing/private/tokeniserPrivate.h"

const struct typeDataStruct tokeniserTypeDataStruct = {
  "Tokeniser",
  NULL,
  NULL,
  sizeof(struct private_tokeniserStruct),
  OBJECT_TYPE_DATA
};

static void skipSpaces(Tokeniser this, PushbackStream stream, int cr) {
  ___BTPUSH;
  while(Char.isSpace(cr)) {
    if(cr == 0x0A) {
      this->column = 1;
      ++this->newLine;
      ++this->line;
    } else {
      ++this->column;
    }
    cr = stream->call->read(stream);
  }
  stream->call->unread(stream, cr);
  ___BTPOP;
}

void* Tokeniser_constructor(TypeData typeData, ...) {
  Tokeniser newInstance;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  newInstance->stringBuilder = newStringBuilder(256);
  ___BTPOP;
  return newInstance;
}

void Tokeniser_dump(Tokeniser this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    this->stringBuilder->call->dump(this->stringBuilder);
  }
  Object_dump((Object)this);
  ___BTPOP;
}

TokenList Tokeniser_getTokens(Tokeniser this, PushbackStream stream) {
  TokenList list;
  Token token;
  int cr;

  ___BTPUSH;
  list = newTokenList(0x1000);
  this->call->resetState(this);
  cr = stream->call->read(stream);
  while(cr != -1) {
    if(Char.isSpace(cr)) {
      skipSpaces(this, stream, cr);
    } else {
      token = this->nextToken(this, stream, cr);
      list->call->appendNew(list, token);
    }
    cr = stream->call->read(stream);
  }
  token = (Token)LastToken_getInstance();
  list->call->appendNew(list, token);
  list->call->trimToSize(list);
  ___BTPOP;
  return list;
}

void Tokeniser_resetState(Tokeniser this) {
  ___BTPUSH;
  this->newLine = 1;
  this->column = 1;
  this->line = 1;
  ___BTPOP;
}
