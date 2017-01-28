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

#include <gvlib/defs.h>
#include <gvlib/errors.h>
#include "../include/gvlcodeparsing/codes.h"
#include "../include/gvlcodeparsing/tokenFilter.h"

#define GVLCP_CODE_LINE_JOINER (GVLCP_CODE_BACKSLASH | GVLCP_CODE_PREPROCESSOR)

static TokenList filter(const Token* tokens, int length);
static int getNextLineOffset(const Token* tokens, int sOff);
static int getOffsetToContinue(const Token* tokens, int sOff);

const struct tokenFilterStruct TokenFilter = {
  filter
};

static TokenList filter(const Token* tokens, int length) {
  TokenList list;
  int i, j;

  ___BTPUSH;
  i = 0;
  list = newTokenList(length);
  while(i < length) {

    switch(Token_getCode(tokens[i])) {

    case GVLCP_CODE_COMMENT:
    case GVLCP_CODE_LINE_JOINER: {
    } break;

    case GVLCP_CODE_PREPROCESSOR_START: {
      j = i;
      i = getOffsetToContinue(tokens, j);
      if(i == j) {
        list->call->append(list, tokens[i]);
      }
    } break;

    default: {
      if((tokens[i]->code & GVLCP_MASK_IGNORE) != GVLCP_CODE_IGNORE) {
        list->call->append(list, tokens[i]);
      }
    } break;

    }
    ++i;

  }
  ___BTPOP;
  return list;
}

/* PRECONDITION: (tokens[sOff]->code & GVLCP_MASK_PREPROCESSOR) == GVLCP_CODE_PREPROCESSOR
   Let 'result' be the value that is returned, then the following holds:
   (tokens[result]->code & GVLCP_MASK_PREPROCESSOR) == GVLCP_CODE_PREPROCESSOR
    A N D
   (tokens[result + 1]->code & GVLCP_MASK_PREPROCESSOR) != GVLCP_CODE_PREPROCESSOR.  */
static int getNextLineOffset(const Token* tokens, int sOff) {
  ___BTPUSH;
  do {
    ++sOff;
  } while((tokens[sOff]->code & GVLCP_MASK_PREPROCESSOR) == GVLCP_CODE_PREPROCESSOR);
  ___BTPOP;
  return sOff - 1;
}

static int getOffsetToContinue(const Token* tokens, int sOff) {
	gvboolean directiveToken, preproToken;
  int code, currentPosition;

  ___BTPUSH;
  currentPosition = sOff;
  do {
    ++sOff;
    preproToken = (tokens[sOff]->code & GVLCP_MASK_PREPROCESSOR) == GVLCP_CODE_PREPROCESSOR;
    directiveToken = (tokens[sOff]->code & GVLCP_MASK_GEN_CLASSIFICATION) == GVLCP_CODE_DIRECTIVE;
  } while(!directiveToken && preproToken);

  if(!directiveToken) {
    /* Remove lines with just '#'. */
    ___BTPOP;
    return sOff - 1;
  }

  code = tokens[sOff]->code & GVLCP_MASK_DIRECTIVE_GROUPS;
  if(code == GVLCP_CODE_GROUP_ENDIF  ||  code == GVLCP_CODE_GROUP_ELIF) {

    /* Keep lines with '#else', '#endif', '#elif', '#if'. */
    ___BTPOP;
    return currentPosition;

  } else if((tokens[sOff]->code & GVLCP_MASK_IGNORE) == GVLCP_CODE_IGNORE  ||
            Token_getErrwarn(tokens[sOff]) != 0) {

    /* Remove lines with '#error', '#line', '#pragma' or a directive marked
       with an error code (these include unknown directives). */
    sOff = getNextLineOffset(tokens, sOff);
    ___BTPOP;
    return sOff;

  }

  /* Now we are left with lines starting with
     '#define', '#ifdef', '#ifndef', '#include', '#undef', and without
     an error code on the (current) directive token.
     Next, skip the comment and '\' tokens to see if the next significant
     token has an error or might even be the last one. */
  do {
    ++sOff;
    code = Token_getCode(tokens[sOff]);
    // TODO: Assumed we do not need to check for errors on '\'-tokens.
  } while(code == GVLCP_CODE_LINE_JOINER  ||  code == GVLCP_CODE_COMMENT);

  if(code == GVLCP_CODE_LAST_TOKEN) {
    ___BTPOP;
    return sOff - 1;
  }

#ifdef DEBUG_MODE
  if((tokens[sOff]->code & GVLCP_MASK_PREPROCESSOR) != GVLCP_CODE_PREPROCESSOR) {
    Errors.printMessageAndExit("Previous while-loop incorrect or Tokeniser bug detected");
  }
#endif

  if(Token_getErrwarn(tokens[sOff]) != 0) {
    sOff = getNextLineOffset(tokens, sOff);
    ___BTPOP;
    return sOff;
  }
  ___BTPOP;
  return currentPosition;
}
