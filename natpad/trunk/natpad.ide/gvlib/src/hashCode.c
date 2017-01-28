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

#include "../include/gvlib/hashCode.h"
#include <stdint.h>

#define M (1000000007)
#define A (335544323)
#define B (65536)

static unsigned int get(unsigned int val);

const struct hashCodeStruct HashCode = {
  get
};

static unsigned int get(unsigned int val) {
  uint64_t result;

  result = val;
  result *= A;
  result += B;
  result %= M;
  return (unsigned int)result;
}
