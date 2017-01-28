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

#include <gvlib/errors.h>
#include "../include/gvlcodeparsing/private/tokenPrivate.h"

const struct typeDataStruct tokenTypeDataStruct = {
  "Token",
  NULL,
  NULL,
  sizeof(struct tokenStruct),
  OBJECT_TYPE_DATA
};

void Token_dump(Token this) {
  ___BTPUSH;
  if(this->refCount == 1  &&  this->element != NULL) {
    this->element->call->dump(this->element);
  }
  Object_dump((Object)this);
  ___BTPOP;
}
