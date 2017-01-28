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

#include "../include/gvlib/private/mapPrivate.h"
#include "../include/gvlib/errors.h"
#ifdef DEBUG_MODE
# include "../include/gvlib/mem.h"
#endif

static const struct mapFunctionsStruct mapFunctions = {
  (void*)Map_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)Map_rtType,

  (void*)Map_add,
  (void*)Map_addAndReserve,
  (void*)Map_clear,
  (void*)Map_delete,
  (void*)Map_get
};

const struct typeDataStruct mapTypeDataStruct = {
  "Map",
  Map_constructor,
  ptrof(mapFunctions),
  sizeof(struct private_mapStruct),
  OBJECT_TYPE_DATA
};

Map newMap(int initialCapacity, double loadFactor) {
  Map newInstance;

  ___BTPUSH;
  newInstance = Map_constructor(MAP_TYPE_DATA, initialCapacity, loadFactor);
  ___BTPOP;
  return newInstance;
}

#define Map struct private_mapStruct*

#define HASH_INDEX(key) (key->call->hashCode(key) % this->capacity)

void Map_add(Map this, Object key, Object value) {
  ___BTPUSH;
  if(Map_getEntry(this, key) == NULL) {
    if(this->size == this->rehashSize) {
      Map_rehash(this);
    }
    Map_addEntry(this, newMapEntry(key, value));
    ++this->size;
  } else {
    key->call->dump(key);
    value->call->dump(value);
  }
  ___BTPOP;
}

void Map_addAndReserve(Map this, Object key, Object value) {
  ___BTPUSH;
  if(Map_getEntry(this, key) == NULL) {
    key->call->reserve(key);
    value->call->reserve(value);
    if(this->size == this->rehashSize) {
      Map_rehash(this);
    }
    Map_addEntry(this, newMapEntry(key, value));
    ++this->size;
  }
  ___BTPOP;
}

void Map_addEntry(Map this, MapEntry entry) {
  MapEntry list;
  int h;

  ___BTPUSH;
  h = HASH_INDEX(entry->key);
  if(this->buckets[h] != NULL) {
    list = this->buckets[h];
    while(list->next != NULL) {
      list = list->next;
    }
    list->next = entry;
  } else {
    this->buckets[h] = entry;
  }
  ___BTPOP;
}

void Map_clear(Map this) {
  MapEntry entry, nextEntry;
  int i;

  ___BTPUSH;
  for(i=0; i<this->capacity; ++i) {
    entry = this->buckets[i];
    this->buckets[i] = NULL;
    while(entry != NULL) {
      nextEntry = entry->next;
      entry->call->dump(entry);
      entry = nextEntry;
    }
  }
  this->size = 0;
  ___BTPOP;
}

void* Map_constructor(TypeData typeData, ...) {
  int initialCapacity;
  double loadFactor;
  Map newInstance;
  va_list argp;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  va_start(argp, typeData);
  initialCapacity = va_arg(argp, int);
  if(initialCapacity < 2) {
    initialCapacity = 16;
  }
  loadFactor = va_arg(argp, double);
  if(loadFactor <= 1.0/initialCapacity) {
    loadFactor = 0.75;
  }
  va_end(argp);
  newInstance->size = 0;
  newInstance->loadFactor = loadFactor;
  newInstance->capacity = initialCapacity;
  newInstance->capacityQuantum = initialCapacity;
  newInstance->rehashSize = (int)(loadFactor * initialCapacity);
  Map_createBuckets(newInstance);
  ___BTPOP;
  return newInstance;
}

void Map_createBuckets(Map this) {
  int i;

  ___BTPUSH;
#ifdef DEBUG_MODE
  this->buckets = memAllocate("MapEntry*", this->capacity * sizeof(void*));
#else
  this->buckets = malloc(this->capacity * sizeof(void*));
#endif
  for(i=0; i<this->capacity; ++i) {
    this->buckets[i] = NULL;
  }
  ___BTPOP;
}

void Map_delete(Map this, Object key) {
  MapEntry entry, nextEntry;
  int h;

  ___BTPUSH;
  h = HASH_INDEX(key);
  entry = this->buckets[h];
  if(entry != NULL) {
    if(entry->key->call->equals(entry->key, key)) {

      this->buckets[h] = entry->next;
      entry->call->dump(entry);
      --this->size;

    } else {

      nextEntry = entry->next;
      while(!(nextEntry==NULL || nextEntry->key->call->equals(nextEntry->key, key))) {
        entry = nextEntry;
        nextEntry = entry->next;
      }
      if(nextEntry != NULL) {
        entry->next = nextEntry->next;
        nextEntry->call->dump(nextEntry);
        --this->size;
      }

    }
  }
  ___BTPOP;
}

void Map_dump(Map this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    this->call->clear((void*)this);
#ifdef DEBUG_MODE
    memRelease(this->buckets);
#else
    free(this->buckets);
#endif
  }
  Object_dump((Object)this);
  ___BTPOP;
}

Object Map_get(Map this, Object key) {
  MapEntry entry;
  Object result;

  ___BTPUSH;
  entry = Map_getEntry(this, key);
  if(entry != NULL) {
    result = entry->value;
  } else {
    result = NULL;
  }
  ___BTPOP;
  return result;
}

MapEntry Map_getEntry(Map this, Object key) {
  MapEntry entry;
  int h;

  ___BTPUSH;
  h = HASH_INDEX(key);
  entry = this->buckets[h];
  while(!(entry==NULL || entry->key->call->equals(entry->key, key))) {
    entry = entry->next;
  }
  ___BTPOP;
  return entry;
}

void Map_rehash(Map this) {
  MapEntry* oldBuckets;
  MapEntry entry;
  int i, oldCapacity;

  ___BTPUSH;
  oldBuckets = this->buckets;
  oldCapacity = this->capacity;
  this->capacity += this->capacityQuantum;
  this->rehashSize = (int)(this->loadFactor * this->capacity);
  Map_createBuckets(this);
  for(i=0; i<oldCapacity; ++i) {
    while(oldBuckets[i] != NULL) {
      entry = oldBuckets[i];
      oldBuckets[i] = entry->next;
      entry->next = NULL;
      Map_addEntry(this, entry);
    }
  }
#ifdef DEBUG_MODE
  memRelease(oldBuckets);
#else
  free(oldBuckets);
#endif
  ___BTPOP;
}

TypeData Map_rtType(void) {
  return MAP_TYPE_DATA;
}



/* Implementation for MapEntry. */

#define MAP_ENTRY_TYPE_DATA ptrof(mapEntryTypeDataStruct)

static void* MapEntry_constructor(TypeData typeData, ...);
static void MapEntry_dump(MapEntry this);
static TypeData MapEntry_rtType(void);

static const struct mapEntryFunctionsStruct mapEntryFunctions = {
  MapEntry_dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  MapEntry_rtType
};

static const struct typeDataStruct mapEntryTypeDataStruct = {
  "MapEntry",
  MapEntry_constructor,
  ptrof(mapEntryFunctions),
  sizeof(struct mapEntryStruct),
  OBJECT_TYPE_DATA
};

MapEntry newMapEntry(Object key, Object value) {
  MapEntry newInstance;

  ___BTPUSH;
  newInstance = MapEntry_constructor(MAP_ENTRY_TYPE_DATA, key, value);
  ___BTPOP;
  return newInstance;
}

static void* MapEntry_constructor(TypeData typeData, ...) {
  MapEntry newInstance;
  va_list argp;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  va_start(argp, typeData);
  newInstance->key = va_arg(argp, void*);
  newInstance->value = va_arg(argp, void*);
  newInstance->next = NULL;
  va_end(argp);
  ___BTPOP;
  return newInstance;
}

static void MapEntry_dump(MapEntry this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    this->value->call->dump(this->value);
    this->key->call->dump(this->key);
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static TypeData MapEntry_rtType(void) {
  return MAP_ENTRY_TYPE_DATA;
}
