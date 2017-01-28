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

#include "../include/gvlib/defs.h"
#undef readonly
#define readonly

#include "RefList.h"
#include "../include/gvlib/array.h"

#define QUANT 32

static void add(RefList this, Reference ref);
static void dump(RefList this);
static int indexOf(RefList this, const void* p);
static void ___remove(RefList this, int index);

const struct refListFunctionsStruct refListFunctions = {
  add,
  dump,
  indexOf,
  ___remove
};

void ___initializeRefList(RefList list) {
  list->call = ptrof(refListFunctions);
  list->buf = malloc(QUANT * sizeof(void*));
  list->capacity = QUANT;
  list->length = 0;
}

static void add(RefList this, Reference ref) {
  if((this->length - this->capacity) == 0) {
    this->capacity += QUANT;
    this->buf = realloc(this->buf, this->capacity * sizeof(void*));
  }
  this->buf[this->length] = ref;
  ++this->length;
}

static void dump(RefList this) {
  free(this->buf);
}

static int indexOf(RefList this, const void* p) {
  int result;

  result = 0;
  while((result < this->length) && (p != this->buf[result]->pointer)) {
    ++result;
  }
  if(result < this->length) {
    return result;
  } else {
    return -1;
  }
}

static void ___remove(RefList this, int index) {
  --this->length;
  Array.copyPtrs((void*)this->buf, index, (void*)this->buf, index+1, this->length-index);
}
