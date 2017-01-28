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
#include <gvlib/private/objectPrivate.h>
#include "CheState.h"

struct private_cheStateStruct {
  readonly refCount_t refCount;
  const struct cheStateFunctionsStruct* const call;
  int parenthesesOpen;
  gvboolean typeDef;
};

#define CheState struct private_cheStateStruct*

static void* constructor(TypeData typeData, ...);
static gvboolean decParenthesesOpen(CheState this);
static void incParenthesesOpen(CheState this);
static void reset(CheState this);
static TypeData rtType(void);

#undef CheState

static const struct cheStateFunctionsStruct cheStateFunctions = {
  (void*)Object_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,
  (void*)decParenthesesOpen,
  (void*)incParenthesesOpen,
  (void*)reset
};

const struct typeDataStruct cheStateTypeDataStruct = {
  "CheState",
  constructor,
  ptrof(cheStateFunctions),
  sizeof(struct private_cheStateStruct),
  OBJECT_TYPE_DATA
};

CheState newCheState(void) {
	CheState newInstance;

  ___BTPUSH;
  newInstance = constructor(CHE_STATE_TYPE_DATA);
  ___BTPOP;
  return newInstance;
}

#define CheState struct private_cheStateStruct*

static void* constructor(TypeData typeData, ...) {
  CheState newInstance;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  ___BTPOP;
  return newInstance;
}

static gvboolean decParenthesesOpen(CheState this) {
	gvboolean result;

  ___BTPUSH;
  result = this->parenthesesOpen == 0;
  if(!result) {
    --this->parenthesesOpen;
  }
  ___BTPOP;
  return result;
}

static void incParenthesesOpen(CheState this) {
  ___BTPUSH;
  ++this->parenthesesOpen;
  ___BTPOP;
}

static void reset(CheState this) {
  ___BTPUSH;
  this->parenthesesOpen = 0;
  this->typeDef = false;
  ___BTPOP;
}

static TypeData rtType(void) {
  return CHE_STATE_TYPE_DATA;
}
