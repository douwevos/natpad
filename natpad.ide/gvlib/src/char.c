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

#include "../include/gvlib/char.h"
#include "../include/gvlib/utils.h"

static const int64_t utf8EncodingMask[] = {0xC0, 0xE0, 0xF0, 0xF8, 0xFC};

static gvboolean inEvenUpperRange(int cr);
static gvboolean inOddUpperRange(int cr);
static gvboolean isAsciiLetter(int cr);
static gvboolean isBinDigit(int cr);
static gvboolean isDiff20Lower(int cr);
static gvboolean isDiff20Upper(int cr);
static gvboolean isDigit(int cr);
static gvboolean isEvenOddLower(int cr);
static gvboolean isEvenOddUpper(int cr);
static gvboolean isHexDigit(int cr);
static gvboolean isLetter(int cr);
static gvboolean isOctDigit(int cr);
static gvboolean isSpace(int cr);
static gvboolean match(gvboolean caseSensitive, int crA, int crB);
static void put(int unicode);
static int toLower(int cr);
static int toUpper(int cr);
static int64_t toUtf8(unsigned int cr);
static int utf8EncodingSize(int cr);
static int valueOf(int cr);

const struct charStruct Char = {
  isAsciiLetter,
  isBinDigit,
  isDigit,
  isHexDigit,
  isLetter,
  isOctDigit,
  isSpace,
  match,
  put,
  toLower,
  toUpper,
  toUtf8,
  utf8EncodingSize,
  valueOf
};

static gvboolean inEvenUpperRange(int cr) {
  return (cr>0xFF && cr<0x138) || (cr>0x149 && cr<0x178);
}

static gvboolean inOddUpperRange(int cr) {
  return (cr>0x138 && cr<0x149) || (cr>0x178 && cr<0x17F);
}

static gvboolean isAsciiLetter(int cr) {
  return (cr>0x40 && cr<0x5B) || (cr>0x60 && cr<0x7B);
}

static gvboolean isBinDigit(int cr) {
  return cr==0x30 || cr==0x31;
}

static gvboolean isDiff20Lower(int cr) {
  return (cr>0x60 && cr<0x7B) || (cr>0xDF && cr<0xFF && cr!=0xF7);
}

static gvboolean isDiff20Upper(int cr) {
  return (cr>0x40 && cr<0x5B) || (cr>0xBF && cr<0xDF && cr!=0xD7);
}

static gvboolean isDigit(int cr) {
  return cr>0x2F && cr<0x3A;
}

static gvboolean isEvenOddLower(int cr) {
  return (inEvenUpperRange(cr) && (cr & 0x01)) || (inOddUpperRange(cr) && !(cr & 0x01));
}

static gvboolean isEvenOddUpper(int cr) {
  return (inEvenUpperRange(cr) && !(cr & 0x01)) || (inOddUpperRange(cr) && (cr & 0x01));
}

static gvboolean isHexDigit(int cr) {
  return (cr>0x2F && cr<0x3A) || (cr>0x40 && cr<0x47) || (cr>0x60 && cr<0x67);
}

static gvboolean isLetter(int cr) {
  return (cr>0x40 && cr<0x5B) || (cr>0x60 && cr<0x7B) || (cr>0xBF && cr<0x180 && !(cr==0xD7 || cr==0xF7));
}

static gvboolean isOctDigit(int cr) {
  return cr>0x2F && cr<0x38;
}

static gvboolean isSpace(int cr) {
  return cr==0x20 || cr==0x09 || cr==0x0A || cr==0xA0 || cr==0x0D;
}

static gvboolean match(gvboolean caseSensitive, int crA, int crB) {
	gvboolean result;
  int temp;

  result = crA == crB;
  if(!(result || caseSensitive)) {
    if(crA > crB) {
      temp = crA;
      crA = crB;
      crB = temp;
    }
    if(crA == 0xFF) {
      crA = 0x178;
    } else {
      crA = toLower(crA);
    }
    result = crA == crB;
  }
  return result;
}

static void put(int unicode) {
  uint64_t val;

  val = toUtf8(unicode);
  while(val > 0) {
    putchar(val & 0xFF);
    val >>= 8;
  }
}

static int toLower(int cr) {
  if(isDiff20Upper(cr)) {
    cr += 0x20;
  } else if(isEvenOddUpper(cr)) {
    ++cr;
  } else if(cr == 0x178) {
    cr = 0xFF;
  }
  return cr;
}

static int toUpper(int cr) {
  if(isDiff20Lower(cr)) {
    cr -= 0x20;
  } else if(isEvenOddLower(cr)) {
    --cr;
  } else if(cr == 0xFF) {
    cr = 0x178;
  }
  return cr;
}

static int64_t toUtf8(unsigned int cr) {
  int64_t result;
  int i, len;

  len = utf8EncodingSize(cr);
  if(len > 1) {
    len -= 2;
    result = (cr & 0x3F) | 0x80;
    result <<= 8;
    cr >>= 6;
    for(i=0; i<len; ++i) {
      result |= (cr & 0x3F) | 0x80;
      result <<= 8;
      cr >>= 6;
    }
    result |= utf8EncodingMask[len] | cr;
  } else {
    result = cr;
  }
  return result;
}

static int utf8EncodingSize(int cr) {
  int result, sig;

  sig = Utils.bsr(cr);
  if(sig > 7) {
    result = (sig + 3)/5;
    if(result == 7) {
      --result;
    }
  } else if(sig == 0) {
    result = 2;
  } else {
    result = 1;
  }
  return result;
}

static int valueOf(int cr) {
  cr -= 0x30;
  if(cr > 9) {
    cr -= 7;
  }
  if(cr > 0xF) {
    cr -= 0x20;
  }
  return cr;
}
