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

#include <gvlib/errors.h>
#include <gvlib/private/objectPrivate.h>
#include <gvlcodeparsing/Token.h>
#include <gvlcodeparsing/CTokeniser.h>
#include "../checodes.h"
#include "cheHighlightFilter.h"
#include "CheKeyWordHighlighter.h"
#include "CheState.h"
#include "CheStateInfoStack.h"
#include "codesetting/chesetcolourcode.h"

struct private_cheKeyWordHighlighterStruct {
  readonly refCount_t refCount;
  const struct cheKeyWordHighlighterFunctionsStruct* const call;
  CTokeniser tokeniser;
  CheStateInfoStack stack;
  CheState state;
};

#define CheKeyWordHighlighter struct private_cheKeyWordHighlighterStruct*

static void* constructor(TypeData typeData, ...);
static void dump(CheKeyWordHighlighter this);
static TokenList getTokens(CheKeyWordHighlighter this, PushbackStream stream);
static TypeData rtType(void);
static void setCodesNotYetSet(const Token* tokens);

#undef CheKeyWordHighlighter

static const struct cheKeyWordHighlighterFunctionsStruct cheKeyWordHighlighterFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,
  (void*)getTokens
};

const struct typeDataStruct cheKeyWordHighlighterTypeDataStruct = {
  "CheKeyWordHighlighter",
  constructor,
  ptrof(cheKeyWordHighlighterFunctions),
  sizeof(struct private_cheKeyWordHighlighterStruct),
  OBJECT_TYPE_DATA
};

CheKeyWordHighlighter newCheKeyWordHighlighter(void) {
  CheKeyWordHighlighter newInstance;

  ___BTPUSH;
  newInstance = constructor(CHE_KEY_WORD_HIGHLIGHTER_TYPE_DATA);
  ___BTPOP;
  return newInstance;
}

#define CheKeyWordHighlighter struct private_cheKeyWordHighlighterStruct*

static void* constructor(TypeData typeData, ...) {
  CheKeyWordHighlighter newInstance;

  ___BTPUSH;
  newInstance = Object_constructor(typeData);
  newInstance->tokeniser = newCTokeniser();
  newInstance->stack = newCheStateInfoStack(8);
  newInstance->state = newCheState();
  ___BTPOP;
  return newInstance;
}

static void dump(CheKeyWordHighlighter this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    this->tokeniser->call->dump(this->tokeniser);
    this->stack->call->dump(this->stack);
    this->state->call->dump(this->state);
  }
  Object_dump((Object)this);
  ___BTPOP;
}

static TokenList getTokens(CheKeyWordHighlighter this, PushbackStream stream) {
  /* 'setColourCode' sets code2 of tokens[sOff] and returns "itself"
     (at least a pointer to the same type of function). */
  void* (* setColourCode)(CheState state, const Token* tokens, int sOff);
  TokenList allTokens, filteredTokens;
  cheStateInfo_t stateInfo;
  int code, sOff;

  ___BTPUSH;
  allTokens = this->tokeniser->call->getTokens(this->tokeniser, stream);
  stream->call->dump(stream);
  filteredTokens = CheHighlightFilter.filter(allTokens->buf, allTokens->size);

  sOff = 0;
  this->stack->call->clear(this->stack);
  this->state->call->reset(this->state);
  setColourCode = che_set_colour_code__base;
  code = Token_getCode(filteredTokens->buf[0]);
  while(code != GVLCP_CODE_LAST_TOKEN) {

    if(code == GVLCP_CODE_BRACE_OPEN) {
      stateInfo.typeDef = this->state->typeDef;
      if(setColourCode == che_set_colour_code__halftype  ||  setColourCode == che_set_colour_code__type) {
        stateInfo.setColourCode = che_set_colour_code__type;
      } else {
        stateInfo.setColourCode = che_set_colour_code__base;
      }
      this->stack->call->push(this->stack, stateInfo);
      this->state->typeDef = false;
    }

    setColourCode = setColourCode(this->state, filteredTokens->buf, sOff);

    switch(code) {

    case GVLCP_CODE_BRACE_CLOSE: {
      if(!this->stack->call->isEmpty(this->stack)) {
        stateInfo = this->stack->call->pop(this->stack);
        this->state->typeDef = stateInfo.typeDef;
        setColourCode = stateInfo.setColourCode;
      } else {
        filteredTokens->buf[sOff]->code2 |= CHE_CODE_ERROR;
      }
    } break;

    case GVLCP_CODE_PARENTHESIS_CLOSE: {
      if(this->state->call->decParenthesesOpen(this->state)) {
        filteredTokens->buf[sOff]->code2 |= CHE_CODE_ERROR;
      }
    } break;

    case GVLCP_CODE_PARENTHESIS_OPEN: {
      this->state->call->incParenthesesOpen(this->state);
    } break;

    case GVLCP_CODE_SEMICOLON: {
      this->state->typeDef = false;
    } break;

    case GVLCP_CODE_TYPEDEF: {
      this->state->typeDef = true;
    } break;

    }
    ++sOff;
    code = Token_getCode(filteredTokens->buf[sOff]);
  }

  filteredTokens->call->dump(filteredTokens);
  setCodesNotYetSet(allTokens->buf);
  ___BTPOP;
  return allTokens;
}

static TypeData rtType(void) {
  return CHE_KEY_WORD_HIGHLIGHTER_TYPE_DATA;
}

static void setCodesNotYetSet(const Token* tokens) {
  int code, i;

  ___BTPUSH;
  i = 0;
  code = Token_getCode(tokens[0]);
  while(code != GVLCP_CODE_LAST_TOKEN) {
    if(tokens[i]->code2 == 0) {
      switch(code) {

      case GVLCP_CODE_COMMENT: {
        tokens[i]->code2 = CHE_CODE_COMMENT;
      } break;

      case GVLCP_CODE_PREPROCESSOR_START:
      case (GVLCP_CODE_DEFINED | GVLCP_CODE_PREPROCESSOR): {
        tokens[i]->code2 = CHE_CODE_DIRECTIVE;
      } break;

      case (GVLCP_CODE_MACRO_ID | GVLCP_CODE_IDENTIFIER): {
        tokens[i]->code2 = CHE_CODE_DECLAR_MACRO;
      } break;

      case (GVLCP_CODE_MACRO_ID_PARAMS | GVLCP_CODE_IDENTIFIER): {
        tokens[i]->code2 = CHE_CODE_DECLAR_MACRO_PARAMS;
      } break;

      default: {
        code &= GVLCP_MASK_GEN_CLASSIFICATION;
        switch(code) {

        case GVLCP_CODE_DIRECTIVE: {
          tokens[i]->code2 = CHE_CODE_DIRECTIVE;
        } break;

        case GVLCP_CODE_FILE_NAME: {
          tokens[i]->code2 = CHE_CODE_STRING;
        } break;

        case GVLCP_CODE_NUMBER: {
          tokens[i]->code2 = CHE_CODE_NUMBER;
        } break;

        case GVLCP_CODE_STRING: {
          tokens[i]->code2 = CHE_CODE_STRING;
        } break;

        default: {
          tokens[i]->code2 = CHE_CODE_NONE;
        } break;

        }
      } break;

      }
    }
    ++i;
    code = Token_getCode(tokens[i]);
  }
  ___BTPOP;
}
