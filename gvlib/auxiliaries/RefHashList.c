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

#include "RefHashList.h"
#include "RefList.h"

#define BUCKET_COUNT (0x800)
#define BUCKET_SIZE (sizeof(struct refListStruct))
#define SIZE_OF_ALL_BUCKETS (BUCKET_COUNT * BUCKET_SIZE)

#define BUCKET_INDEX(ptr) (hashCode(ptr) % BUCKET_COUNT)

static const uint64_t M = 1073741827;
static const uint64_t A = 1048583;
static const uint64_t B = 440234149;

static RefList bucket[BUCKET_COUNT];
static byte bucketBlock[SIZE_OF_ALL_BUCKETS];
static gvboolean initialized = false;
static int length;

static void add(Reference ref);
static void dump(void);
static Reference takeOut(const void* p);
static int hashCode(const void* p);
static void incShareCount(const void* p);
static int ___length(void);
static int ___remove(const void* p);

#ifdef DEBUG_MODE
static int removeAndReleaseAll(void);
#endif

static const struct refHashListStruct refHashList = {
  add,
  dump,
  incShareCount,
  ___length,
  ___remove,
  takeOut

#ifdef DEBUG_MODE
  ,removeAndReleaseAll
#endif
};

RefHashList ___getRefHashList(void) {
  RefHashList instance;
  int i;

  instance = ptrof(refHashList);
  if(!initialized) {
    length = 0;
    initialized = true;
    bucket[0] = (void*)bucketBlock;
    ___initializeRefList(bucket[0]);
    for(i=1; i<BUCKET_COUNT; ++i) {
      bucket[i] = bucket[i-1] + 1;
      ___initializeRefList(bucket[i]);
    }
  }
  return instance;
}

static void add(Reference ref) {
  int h;

  h = BUCKET_INDEX(ref->pointer);
  bucket[h]->call->add(bucket[h], ref);
  ++length;
}

static void dump(void) {
  int i;

  initialized = false;
  for(i=0; i<BUCKET_COUNT; ++i) {
    bucket[i]->call->dump(bucket[i]);
  }
}

static int hashCode(const void* p) {
  uint64_t val;

  val = (uint64_t)(long)p;
  val &= 0x3FFFFFFF;
  val *= A;
  val += B;
  val %= M;
  return (int)val;
}

static void incShareCount(const void* p) {
  int h, i;

  h = BUCKET_INDEX(p);
  i = bucket[h]->call->indexOf(bucket[h], p);
  if(i > -1) {
    ++bucket[h]->buf[i]->count;
  }
}

static int ___length(void) {
  return length;
}

static int ___remove(const void* p) {
#ifdef DEBUG_MODE

  int bytesReleased, h, i;
  Reference ref;

  bytesReleased = 0;
  h = BUCKET_INDEX(p);
  i = bucket[h]->call->indexOf(bucket[h], p);
  if(i > -1) {
    ref = bucket[h]->buf[i];
    --ref->count;
    if(ref->count == 0) {
      bucket[h]->call->remove(bucket[h], i);
      bytesReleased = ref->size;
      free((void*)ref->pointer);
      free(ref);
      --length;
    }
  }
  return bytesReleased;

#else

  int h, i;
  Reference ref;

  h = BUCKET_INDEX(p);
  i = bucket[h]->call->indexOf(bucket[h], p);
  if(i > -1) {
    ref = bucket[h]->buf[i];
    --ref->count;
    if(ref->count == 0) {
      bucket[h]->call->remove(bucket[h], i);
      free((void*)ref->pointer);
      free(ref);
      --length;
    }
  }
  return 0;

#endif
}

static Reference takeOut(const void* p) {
  Reference result;
  int h, i;

  h = BUCKET_INDEX(p);
  i = bucket[h]->call->indexOf(bucket[h], p);
  if(i > -1) {
    result = bucket[h]->buf[i];
    bucket[h]->call->remove(bucket[h], i);
    --length;
  } else {
    result = NULL;
  }
  return result;
}

#ifdef DEBUG_MODE

static int removeAndReleaseAll(void) {
  int i, j, lastIndex, releasedPointers;
  Reference ref;

  releasedPointers = 0;
  if(length > 0) {
    for(i=0; i<BUCKET_COUNT; ++i) {
      lastIndex = bucket[i]->length - 1;
      for(j=lastIndex; j>-1; --j) {
        ref = bucket[i]->buf[j];
        bucket[i]->call->remove(bucket[i], j);
        printf("  (%lu) %s\n", ref->count, ref->typeName);
        releasedPointers += ref->count;
        free((void*)ref->pointer);
        free(ref);
      }
    }
    length = 0;
  }
  return releasedPointers;
}

#endif
