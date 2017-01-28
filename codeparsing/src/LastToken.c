/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2009, 2010 Gerard Visser.

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

#include "../include/gvlcodeparsing/codes.h"
#include "../include/gvlcodeparsing/LastToken.h"
#include "../include/gvlcodeparsing/private/tokenPrivate.h"

static void dummy(LastToken this);
static TypeData rtType(void);

static const struct lastTokenFunctionsStruct lastTokenFunctions = {
  dummy,
  (void*)Object_equals,
  (void*)Object_hashCode,
  dummy,
  rtType
};

static struct lastTokenStruct lastToken = {
  0L,
  ptrof(lastTokenFunctions),
  NULL,
  0L,
  -1,
  -1,
  GVLCP_CODE_LAST_TOKEN,
  0,
  0,
  0
};

const struct typeDataStruct lastTokenTypeDataStruct = {
  "LastToken",
  NULL,
  ptrof(lastTokenFunctions),
  sizeof(struct lastTokenStruct),
  TOKEN_TYPE_DATA
};

LastToken LastToken_getInstance(void) {
  return ptrof(lastToken);
}

static void dummy(LastToken this){}

static TypeData rtType(void) {
  return LAST_TOKEN_TYPE_DATA;
}
