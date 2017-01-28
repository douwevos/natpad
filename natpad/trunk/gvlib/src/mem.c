/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

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

#include "../include/gvlib/mem.h"

#undef readonly
#define readonly const

#include "../auxiliaries/Reference.h"
#include "../auxiliaries/RefHashList.h"

static RefHashList refList = NULL;

static void memRelease_initially(const void* dPtr);
static void memRelease_subsequently(const void* dPtr);
static void memReserve_initially(const void* dPtr);
static void memReserve_subsequently(const void* dPtr);

void (* memRelease)(const void* dPtr) = memRelease_initially;
void (* memReserve)(const void* dPtr) = memReserve_initially;

static void memRelease_initially(const void* dPtr){}
static void memReserve_initially(const void* dPtr){}


#ifdef DEBUG_MODE

# include "../include/gvlib/errors.h"

# define ERR_NOTHING_ALLOCATED "Nothing has been allocated yet"
# define ERR_OUT_OF_MEMORY(value) "Not enough memory to allocate %Z more bytes", (long unsigned int)value
# define ERR_REF_COUNT_NOT_ONE "Addresses referred to at more than one location shouldn't be reallocated"
# define ERR_SIZE_IN_BYTES_NOT_NATURAL "The number of bytes to allocate, 'sizeInBytes', needs to be natural\n(i.e. a positive integer)"
# define ERR_UNKNOWN_POINTER "Unknown pointer: reallocation not possible"

# define SET_ALLOCATION_MAX                         \
  if(bytesAllocatedCurrently > bytesAllocatedMax) { \
    bytesAllocatedMax = bytesAllocatedCurrently;    \
  }

static int allocations = 0;
static int64_t bytesAllocated = 0;
static int64_t bytesAllocatedCurrently = 0;
static int64_t bytesAllocatedMax = 0;
static int64_t bytesReleased = 0;

static void* memAllocate_initially(const char* typeName, size_t sizeInBytes);
static void* memAllocate_subsequently(const char* typeName, size_t sizeInBytes);

void* (* memAllocate)(const char* typeName, size_t sizeInBytes) = memAllocate_initially;

static void* memAllocate_initially(const char* typeName, size_t sizeInBytes) {
  ___BTPUSH;
  refList = ___getRefHashList();
  ___BTPOP;
  memAllocate = memAllocate_subsequently;
  memRelease = memRelease_subsequently;
  memReserve = memReserve_subsequently;
  return memAllocate_subsequently(typeName, sizeInBytes);
}

static void* memAllocate_subsequently(const char* typeName, size_t sizeInBytes) {
  void* result;

  Errors.push(__FILE__, "memAllocate");
  if(typeName==NULL || typeName[0]==0) {
    Errors.printMessageAndExit("No type name specified");
  }
  if(sizeInBytes < 1) {
    Errors.printMessageAndExit(ERR_SIZE_IN_BYTES_NOT_NATURAL);
  }
  result = malloc(sizeInBytes);
  if(result == NULL) {
    Errors.printMessageAndExit(ERR_OUT_OF_MEMORY(sizeInBytes));
  }
  refList->add(newReference(result, sizeInBytes, typeName));
  bytesAllocatedCurrently += sizeInBytes;
  bytesAllocated += sizeInBytes;
  SET_ALLOCATION_MAX;
  ++allocations;
  ___BTPOP;
  return result;
}

void memPrintExitStats(void) {
  int releasedPointers;

  ___BTPUSH;
  printf("\nNumber of allocations:     %d\n", (int)allocations);
  printf("Bytes allocated:           %d\n", (int)bytesAllocated);
  printf("Bytes released:            %d\n", (int)bytesReleased);
  printf("Max. allocated bytes:      %d\n", (int)bytesAllocatedMax);
  if(bytesAllocatedCurrently == 0) {
    printf("\x1B[1;32m");
  } else {
    printf("\x1B[1;31m");
  }
  printf("Currently allocated bytes: %d\n", (int)bytesAllocatedCurrently);
  if(refList != NULL) {
    if(refList->length() > 0) {
      releasedPointers = refList->removeAndReleaseAll();
      if(releasedPointers == 1) {
        printf("1 pointer was released.\n");
      } else {
        printf("%d pointers were released.\n", releasedPointers);
      }
    }
    refList->dump();
  }
  printf("\x1B[0m");
  ___BTPOP;
}

void* memReallocate(const void* sPtr, size_t sizeInBytes) {
  void* dPtr;

  ___BTPUSH;
  dPtr = memReallocateObject(sPtr, sizeInBytes, NULL);
  ___BTPOP;
  return dPtr;
}

void* memReallocateObject(const void* sPtr, size_t sizeInBytes, const char* newTypeName) {
  Reference ref;
  void* dPtr;

  ___BTPUSH;
  if(refList == NULL) {
    Errors.printMessageAndExit(ERR_NOTHING_ALLOCATED);
  }
  if(sizeInBytes < 1) {
    Errors.printMessageAndExit(ERR_SIZE_IN_BYTES_NOT_NATURAL);
  }
  ref = refList->takeOut(sPtr);
  if(ref == NULL) {
    Errors.printMessageAndExit(ERR_UNKNOWN_POINTER);
  }
  if(ref->count != 1) {
    Errors.printMessageAndExit(ERR_REF_COUNT_NOT_ONE);
  }
  dPtr = realloc((void*)sPtr, sizeInBytes);
  if(dPtr == NULL) {
    Errors.printMessageAndExit(ERR_OUT_OF_MEMORY(sizeInBytes));
  }
  if(newTypeName != NULL) {
    ref->typeName = newTypeName;
  }
  if(dPtr != sPtr) {
    ref->pointer = dPtr;
    bytesAllocatedCurrently += sizeInBytes;
    bytesAllocated += sizeInBytes;
    SET_ALLOCATION_MAX;
    bytesAllocatedCurrently -= ref->size;
    bytesReleased += ref->size;
    ++allocations;
  } else {
    if(sizeInBytes > ref->size) {
      bytesAllocated += sizeInBytes - ref->size;
    } else {
      bytesReleased += ref->size - sizeInBytes;
    }
    bytesAllocatedCurrently += sizeInBytes - ref->size;
    SET_ALLOCATION_MAX;
  }
  ref->size = sizeInBytes;
  refList->add(ref);
  ___BTPOP;
  return dPtr;
}

static void memRelease_subsequently(const void* dPtr) {
  int releasedBytes;

  Errors.push(__FILE__, "memRelease");
  releasedBytes = refList->remove(dPtr);
  bytesAllocatedCurrently -= releasedBytes;
  bytesReleased += releasedBytes;
  ___BTPOP;
}

static void memReserve_subsequently(const void* dPtr) {
  Errors.push(__FILE__, "memReserve");
  refList->incShareCount(dPtr);
  ___BTPOP;
}

/* ========================================================================== */

#else

# undef memAllocate

static void* memAllocate_initially(size_t sizeInBytes);
static void* memAllocate_subsequently(size_t sizeInBytes);

void* (* memAllocate)(size_t sizeInBytes) = memAllocate_initially;

static void* memAllocate_initially(size_t sizeInBytes) {
  refList = ___getRefHashList();
  memAllocate = memAllocate_subsequently;
  memRelease = memRelease_subsequently;
  memReserve = memReserve_subsequently;
  return memAllocate_subsequently(sizeInBytes);
}

static void* memAllocate_subsequently(size_t sizeInBytes) {
  void* result;

  result = malloc(sizeInBytes);
  refList->add(newReference(result));
  return result;
}

void memPrintExitStats(void) {
  if(refList != NULL) {
    refList->dump();
  }
}

void* memReallocate(const void* sPtr, size_t sizeInBytes) {
  Reference ref;
  void* dPtr;

  dPtr = realloc((void*)sPtr, sizeInBytes);
  if(dPtr != sPtr) {
    ref = refList->takeOut(sPtr);
    ref->pointer = dPtr;
    refList->add(ref);
  }
  return dPtr;
}

static void memRelease_subsequently(const void* dPtr) {
  refList->remove(dPtr);
}

static void memReserve_subsequently(const void* dPtr) {
  refList->incShareCount(dPtr);
}

#endif
