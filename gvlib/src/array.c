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

#include "../include/gvlib/array.h"
#include "../include/gvlib/errors.h"

static int bytezLength(const byte* sPtr);
static void copyBytes(byte* dPtr, long dOff, const byte* sPtr, long sOff, long len);
static void copyInts(int* dPtr, long dOff, const int* sPtr, long sOff, long len);
static void copyPtrs(const void** dPtr, long dOff, const void* const * sPtr, long sOff, long len);

const struct arrayStruct Array = {
  bytezLength,
  copyBytes,
  copyInts,
  copyPtrs
};

static int bytezLength(const byte* sPtr) {
  long int sOff;
  int len;

  ___BTPUSH;
  len = 1;
  sOff = 0;
  while(sPtr[sOff] != 0) {
    ++sOff;
    ++len;
  }
  ___BTPOP;
  return len;
}

static void copyBytes(byte* dPtr, long dOff, const byte* sPtr, long sOff, long len) {
  long i, step;

  ___BTPUSH;
  if((long)dPtr + dOff - (long)sPtr - sOff  >  0) {
    dOff += len - 1;
    sOff += len - 1;
    step = -1;
  } else {
    step = 1;
  }
  for(i=0; i<len; ++i) {
    dPtr[dOff] = sPtr[sOff];
    dOff += step;
    sOff += step;
  }
  ___BTPOP;
}

static void copyInts(int* dPtr, long dOff, const int* sPtr, long sOff, long len) {
  long dstOff, i, srcOff, step;

  ___BTPUSH;
  dstOff = dOff * sizeof(int);
  srcOff = sOff * sizeof(int);
  if((long)dPtr + dstOff - (long)sPtr - srcOff  >  0) {
    dOff += len - 1;
    sOff += len - 1;
    step = -1;
  } else {
    step = 1;
  }
  for(i=0; i<len; ++i) {
    dPtr[dOff] = sPtr[sOff];
    dOff += step;
    sOff += step;
  }
  ___BTPOP;
}

static void copyPtrs(const void** dPtr, long dOff, const void* const * sPtr, long sOff, long len) {
  long dstOff, i, srcOff, step;

  ___BTPUSH;
  dstOff = dOff * sizeof(void*);
  srcOff = sOff * sizeof(void*);
  if((long)dPtr + dstOff - (long)sPtr - srcOff  >  0) {
    dOff += len - 1;
    sOff += len - 1;
    step = -1;
  } else {
    step = 1;
  }
  for(i=0; i<len; ++i) {
    dPtr[dOff] = sPtr[sOff];
    dOff += step;
    sOff += step;
  }
  ___BTPOP;
}
