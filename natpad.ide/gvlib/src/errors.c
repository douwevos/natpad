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

#ifdef DEBUG_MODE
# include <signal.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
#endif

#include "../include/gvlib/errors.h"

#ifdef DEBUG_MODE
static void addStringToBacktrace(const char* str);
static void createBacktrace(void);
static void handleSegmentationFault(int sig);
static void pop(void);
static void printBacktraceAndExit(void);
static void printMessageAndExit(const char* message, ...);
static void push(const char* fileName, const char* functionName);
#endif

static void printBacktraceIfSegmentationFaultOccurs(void);

const struct errorsStruct Errors = {
  printBacktraceIfSegmentationFaultOccurs

#ifdef DEBUG_MODE
  , pop,
  printMessageAndExit,
  push
#endif
};

#ifdef DEBUG_MODE

# define ptrof(x) (&x)

# define BTSTACK_SIZE (1024)
# define NAMES_MAX_MEAN_LENGTH (256)
# define STRING_BUF_SIZE (BTSTACK_SIZE * (NAMES_MAX_MEAN_LENGTH + 4))

static int btsp = BTSTACK_SIZE;
static const char* btstack[BTSTACK_SIZE];

static int strbufoff = 0;
static char strbuf[STRING_BUF_SIZE];

static void addStringToBacktrace(const char* str) {
  int i;

  i = 0;
  while(str[i] != 0) {
    strbuf[strbufoff] = str[i];
    ++strbufoff;
    ++i;
  }
}

static int copyMessagePart(int* dOffPtr, const char* sPtr, int sOff) {
  while(!(sPtr[sOff]=='%' || sPtr[sOff]==0)) {
    strbuf[dOffPtr[0]] = sPtr[sOff];
    ++dOffPtr[0];
    ++sOff;
  }
  if(sPtr[sOff] == '%') {
    ++sOff;
  }
  return sOff;
}

static void createBacktrace(void) {
  while(btsp < BTSTACK_SIZE) {
    strbuf[strbufoff] = ' ';
    ++strbufoff;

    addStringToBacktrace(btstack[btsp]);
    ++btsp;

    strbuf[strbufoff] = ':';
    ++strbufoff;

    strbuf[strbufoff] = ' ';
    ++strbufoff;

    addStringToBacktrace(btstack[btsp]);
    ++btsp;

    strbuf[strbufoff] = '\n';
    ++strbufoff;
  }
  strbuf[strbufoff] = 0;
}

static void handleSegmentationFault(int sig) {
  printMessageAndExit("Segmentation fault");
}

static void pop(void) {
  btsp += 2;
}

static void printBacktraceAndExit(void) {
  createBacktrace();
  if(strbufoff > 0) {
    printf("Back trace of the current stack:\n%s\n", strbuf);
  }
  exit(EXIT_FAILURE);
}

static void printBacktraceIfSegmentationFaultOccurs(void) {
  signal(SIGSEGV, handleSegmentationFault);
}

static void printMessageAndExit(const char* message, ...) {
  int dOff, ival, len, sOff;
  long unsigned int zval;
  va_list argp;
  char* sval;

  if(!(message==NULL || message[0]==0)) {
    dOff = 0;
    sOff = 0;
    va_start(argp, message);
    sOff = copyMessagePart(ptrof(dOff), message, sOff);
    push(__FILE__, __func__);
    while(message[sOff] != 0) {
      switch(message[sOff]) {
      case 'Z': {
        zval = va_arg(argp, long int);
        len = sprintf(strbuf + dOff, "%lu", zval);
        dOff += len;
      } break;

      case 'c': {
        ival = va_arg(argp, int);
        strbuf[dOff] = (char)ival;
        ++dOff;
      } break;

      case 'd': {
        ival = va_arg(argp, int);
        len = sprintf(strbuf + dOff, "%d", ival);
        dOff += len;
      } break;

      case 's': {
        sval = va_arg(argp, char*);
        len = sprintf(strbuf + dOff, "%s", sval);
        dOff += len;
      } break;
      }
      ++sOff;
      sOff = copyMessagePart(ptrof(dOff), message, sOff);
    }
    pop();
    va_end(argp); /* ANSI C requirement. */
    strbuf[dOff] = 0;
    printf("\x1B[31;1mERROR:\n%s\x1B[0m\n", strbuf);
  } else {
    printf("\x1B[31;1mERROR\x1B[0m\n");
  }
  printBacktraceAndExit();
}

static void push(const char* fileName, const char* functionName) {
  if(btsp > 1) {
    --btsp;
    btstack[btsp] = functionName;
    --btsp;
    btstack[btsp] = fileName;
  } else {
    printMessageAndExit("Stack overflow (max. number of entries = %d, see 'errors.c' to change)", BTSTACK_SIZE / 2);
  }
}

#else

static void printBacktraceIfSegmentationFaultOccurs(void){}

#endif
