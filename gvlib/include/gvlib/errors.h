/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

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

#ifndef ___ERRORS_INCLUDED
#define ___ERRORS_INCLUDED

#ifdef DEBUG_MODE
# define ___BTPUSH Errors.push(__FILE__, __func__);
# define ___BTPOP Errors.pop();
#else
# define ___BTPUSH
# define ___BTPOP
#endif

extern const struct errorsStruct {
  void (* printBacktraceIfSegmentationFaultOccurs)(void);

#ifdef DEBUG_MODE
  void (* pop)(void);
  void (* printMessageAndExit)(const char* message, ...);
  void (* push)(const char* fileName, const char* functionName);
#endif
} Errors;

#endif
