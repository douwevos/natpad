/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2009, 2010, 2011 Gerard Visser.

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

#include <gvlib/char.h>
#include <gvlib/errors.h>
#include "../include/gvlcodeparsing/CharToken.h"
#include "../include/gvlcodeparsing/codes.h"
#include "../include/gvlcodeparsing/CommentToken.h"
#include "../include/gvlcodeparsing/CTokeniser.h"
#include "../include/gvlcodeparsing/ExtraToken.h"
#include "../include/gvlcodeparsing/IncludeToken.h"
#include "../include/gvlcodeparsing/keyWords.h"
#include "../include/gvlcodeparsing/LinenumberToken.h"
#include "../include/gvlcodeparsing/NumberToken.h"
#include "../include/gvlcodeparsing/PrimaryToken.h"
#include "../include/gvlcodeparsing/SecondaryToken.h"
#include "../include/gvlcodeparsing/StringElemInfoMap.h"
#include "../include/gvlcodeparsing/StringToken.h"
#include "../include/gvlcodeparsing/private/tokenPrivate.h"
#include "../include/gvlcodeparsing/private/tokeniserPrivate.h"

DEFINE_PRIVATE_TOKEN_STRUCT(tokenFunctionsStruct)

#define CTokeniser struct private_cTokeniserStruct*

struct private_cTokeniserStruct;

struct private_cTokeniserFunctionsStruct {
  void (* dump)(CTokeniser this);
  gvboolean (* equals)(CTokeniser this, Object other);
  int (* hashCode)(CTokeniser this);
  void (* reserve)(CTokeniser this);
  TypeData (* rtType)(void);

  TokenList (* getTokens)(CTokeniser this, PushbackStream stream);
  void (* resetState)(CTokeniser this);
};

struct private_cTokeniserStruct {
  readonly refCount_t refCount;
  const struct private_cTokeniserFunctionsStruct* const call;
  Token (* nextToken)(CTokeniser this, PushbackStream stream, int cr);
  StringBuilder stringBuilder;
  int newLine;
  int column;
  int line;
  gvboolean commentOpen;
  StringElemInfoMap directives;
  StringElemInfoMap reservedWords;
  Token storedToken[2];
  gvboolean joinLines;
};

static void appendCode(struct private_tokenStruct* token, int code);
static void* constructor(TypeData typeData, ...);
static void dump(CTokeniser this);
static gvboolean handlePreprocessorLineState(CTokeniser this);
static Token nextToken_base(CTokeniser this, PushbackStream stream, int cr);
static Token nextToken_define(CTokeniser this, PushbackStream stream, int cr);
static Token nextToken_directive(CTokeniser this, PushbackStream stream, int cr);
static Token nextToken_elif_if_macro(CTokeniser this, PushbackStream stream, int cr);
static Token nextToken_extra(CTokeniser this, PushbackStream stream, int cr);
static Token nextToken_ifdef_ifndef_undef(CTokeniser this, PushbackStream stream, int cr);
static Token nextToken_include(CTokeniser this, PushbackStream stream, int cr);
static Token nextToken_line(CTokeniser this, PushbackStream stream, int cr);
static Token nextToken_line_opt(CTokeniser this, PushbackStream stream, int cr);
static Token nextToken_superfluous(CTokeniser this, PushbackStream stream, int cr);
static void resetState(CTokeniser this);
static TypeData rtType(void);
static void setErrwarn(struct private_tokenStruct* token, int errwarnCode);

#undef CTokeniser


static const struct private_cTokeniserFunctionsStruct cTokeniserFunctions = {
  (void*)dump,
  (void*)Object_equals,
  (void*)Object_hashCode,
  (void*)Object_reserve,
  (void*)rtType,
  (void*)Tokeniser_getTokens,
  (void*)resetState
};

const struct typeDataStruct cTokeniserTypeDataStruct = {
  "CTokeniser",
  constructor,
  ptrof(cTokeniserFunctions),
  sizeof(struct private_cTokeniserStruct),
  TOKENISER_TYPE_DATA
};

CTokeniser newCTokeniser(void) {
  CTokeniser newInstance;

  ___BTPUSH;
  newInstance = constructor(C_TOKENISER_TYPE_DATA);
  ___BTPOP;
  return newInstance;
}

#define CTokeniser struct private_cTokeniserStruct*

#define FIRST_CHAR_OF_IDENTIFIER(x) (Char.isAsciiLetter(x) || x=='_' || x=='$')

#define HANDLE_PREPROCESSOR_LINE_STATE(this) \
  if(handlePreprocessorLineState(this)) { \
    this->nextToken = nextToken_extra; \
    ___BTPOP; \
    return nextToken_extra(this, stream, cr); \
  } else if(this->newLine > 0) { \
    this->nextToken = nextToken_base; \
    ___BTPOP; \
    return nextToken_base(this, stream, cr); \
  }

#define HANDLE_PREPROCESSOR_LINE_STATE__WITH_ERROR_CODE(this, errorCode) \
  if(handlePreprocessorLineState(this)) { \
    this->nextToken = nextToken_extra; \
    setErrwarn((struct private_tokenStruct*)STORED_DIRECTIVE_TOKEN, errorCode); \
    ___BTPOP; \
    return nextToken_extra(this, stream, cr); \
  } else if(this->newLine > 0) { \
    this->nextToken = nextToken_base; \
    setErrwarn((struct private_tokenStruct*)STORED_DIRECTIVE_TOKEN, errorCode); \
    ___BTPOP; \
    return nextToken_base(this, stream, cr); \
  }

#define JOIN_LINES(this, token) \
  STORED_BACKSLASH_TOKEN = token; \
  this->joinLines = true;

#define STORED_BACKSLASH_TOKEN this->storedToken[0]
#define STORED_DIRECTIVE_TOKEN this->storedToken[1]

#ifdef DEBUG_MODE

# define CLEAR_STORED_DIRECTIVE_TOKEN STORED_DIRECTIVE_TOKEN = NULL
# define TEST_PREPROCESSOR_TOKEN_CODE(token) \
  if((token->code & GVLCP_CODE_PREPROCESSOR) == 0 && \
     (token->code & GVLCP_CODE_COMMENT) == 0) { \
    Errors.printMessageAndExit("Preprocessor token code corrupt"); \
  }

#else

# define CLEAR_STORED_DIRECTIVE_TOKEN
# define TEST_PREPROCESSOR_TOKEN_CODE(token)

#endif

/* To avoid possible removal of code when compiled with option -O2 or -O3. */
static void appendCode(struct private_tokenStruct* token, int code) {
  ___BTPUSH;
  token->code |= code;
  ___BTPOP;
}

static void* constructor(TypeData typeData, ...) {
  CTokeniser newInstance;

  ___BTPUSH;
  newInstance = Tokeniser_constructor(typeData);

  newInstance->directives = KeyWords.getDirectives();
  newInstance->reservedWords = KeyWords.getReservedWords();
#ifdef DEBUG_MODE
  if(newInstance->reservedWords == NULL) {
    Errors.printMessageAndExit("KeyWords.init() has probably not been called yet");
  }
#endif
  /* Not thread-safe.
     For now, make sure KeyWords.cleanUp() isn't called in some thread
     while this constructor is being executed. */
  newInstance->directives->call->reserve(newInstance->directives);
  newInstance->reservedWords->call->reserve(newInstance->reservedWords);
  ___BTPOP;
  return newInstance;
}

static void dump(CTokeniser this) {
  ___BTPUSH;
  if(this->refCount == 1) {
    this->reservedWords->call->dump(this->reservedWords);
    this->directives->call->dump(this->directives);
  }
  Tokeniser_dump((Tokeniser)this);
  ___BTPOP;
}

static gvboolean handlePreprocessorLineState(CTokeniser this) {
  ___BTPUSH;
  if(this->joinLines) {
    this->joinLines = false;
    if(this->newLine == 0) {
      setErrwarn((struct private_tokenStruct*)STORED_BACKSLASH_TOKEN, GVLCP_ERROR_STRAY_TOKEN);
      ___BTPOP;
      return true;
    } else {
      --this->newLine;
    }
  } else if(this->commentOpen) {
    this->newLine = 0;
  }
  ___BTPOP;
  return false;
}

static Token nextToken_base(CTokeniser this, PushbackStream stream, int cr) {
  Token token;

  ___BTPUSH;
  if(this->commentOpen) {

    stream->call->unread(stream, cr);
    token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));

  } else if(FIRST_CHAR_OF_IDENTIFIER(cr)) {

    if(cr == 'L') {
      cr = stream->call->read(stream);
      if(cr != '\'') {
        stream->call->unread(stream, cr);
        token = (Token)newPrimaryToken(this->line, this->column, stream, 'L', this->stringBuilder, this->reservedWords);
      } else {
        token = (Token)newCharToken(this->line, this->column, stream, true, this->stringBuilder);
      }
    } else {
      token = (Token)newPrimaryToken(this->line, this->column, stream, cr, this->stringBuilder, this->reservedWords);
    }
    this->newLine = 0;

  } else if(Char.isDigit(cr)) {

    token = (Token)newNumberToken(this->line, this->column, stream, cr, this->stringBuilder);
    this->newLine = 0;

  } else {

    switch(cr) {

    case '"': {
      token = (Token)newStringToken(this->line, this->column, stream, this->stringBuilder);
      this->newLine = 0;
    } break;

    case '#': {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      if(this->newLine > 0  &&  token->code == 0) {
        appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR_START);
        this->nextToken = nextToken_directive;
      } else {
        setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_STRAY_TOKEN);
      }
      this->newLine = 0;
    } break;

    case '\'': {
      token = (Token)newCharToken(this->line, this->column, stream, false, this->stringBuilder);
      this->newLine = 0;
    } break;

    case '.': {
      cr = stream->call->read(stream);
      stream->call->unread(stream, cr);
      if(Char.isDigit(cr)) {
        token = (Token)newNumberToken(this->line, this->column, stream, '.', this->stringBuilder);
      } else {
        token = (Token)newSecondaryToken(this->line, this->column, stream, '.');
      }
      this->newLine = 0;
    } break;

    case '/': {
      cr = stream->call->read(stream);
      switch(cr) {

      case '/': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder);
      } break;

      case '*': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));
      } break;

      default: {
        stream->call->unread(stream, cr);
        token = (Token)newSecondaryToken(this->line, this->column, stream, '/');
        this->newLine = 0;
      } break;

      }
    } break;

    default: {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      this->newLine = 0;
    } break;

    }

  }
  this->column += token->length;
  ___BTPOP;
  return token;
}

static Token nextToken_define(CTokeniser this, PushbackStream stream, int cr) {
  Token token;

  ___BTPUSH;
  HANDLE_PREPROCESSOR_LINE_STATE__WITH_ERROR_CODE(this, GVLCP_ERROR_NO_MACRO_NAME_GIVEN);

  if(this->commentOpen) {

    stream->call->unread(stream, cr);
    token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));

  } else if(FIRST_CHAR_OF_IDENTIFIER(cr)) {

    token = (Token)newPrimaryToken(this->line, this->column, stream, cr, this->stringBuilder, this->reservedWords);
    if(token->code != GVLCP_CODE_DEFINED) {
      cr = stream->call->read(stream);
      stream->call->unread(stream, cr);
      if(cr == '(') {
        appendCode((struct private_tokenStruct*)token, GVLCP_CODE_MACRO_ID_PARAMS);
        this->nextToken = nextToken_elif_if_macro;
      } else {
        /* Note that if cr=='\\' the situation might not be handled
           in the right way: the macro name might continue on the next line
           or parameters might still be defined if the first character on
           the next line is '('. (Or after any sequence of backslash, new line
           characters.) */
        appendCode((struct private_tokenStruct*)token, GVLCP_CODE_MACRO_ID);
        this->nextToken = nextToken_elif_if_macro;
      }
    } else {
      setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_DEFINED_USED_AS_MACRO_NAME);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      this->nextToken = nextToken_extra;
    }

  } else {

    switch(cr) {

    case '/': {
      cr = stream->call->read(stream);
      switch(cr) {

      case '/': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)STORED_DIRECTIVE_TOKEN, GVLCP_ERROR_NO_MACRO_NAME_GIVEN);
        this->nextToken = nextToken_base;
      } break;

      case '*': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));
      } break;

      default: {
        stream->call->unread(stream, cr);
        token = (Token)newExtraToken(this->line, this->column, stream, '/', this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_MACRO_NAMES_MUST_BE_IDENTIFIERS);
        this->nextToken = nextToken_extra;
      } break;

      }
    } break;

    case '\\': {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      JOIN_LINES(this, token);
    } break;

    default: {
      token = (Token)newExtraToken(this->line, this->column, stream, cr, this->stringBuilder);
      setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_MACRO_NAMES_MUST_BE_IDENTIFIERS);
      this->nextToken = nextToken_extra;
    } break;

    }

  }
  this->column += token->length;

  TEST_PREPROCESSOR_TOKEN_CODE(token);
  ___BTPOP;
  return token;
}

static Token nextToken_directive(CTokeniser this, PushbackStream stream, int cr) {
  Token token;
  int groupId;

  ___BTPUSH;
  HANDLE_PREPROCESSOR_LINE_STATE(this);
  CLEAR_STORED_DIRECTIVE_TOKEN;

  if(this->commentOpen) {

    stream->call->unread(stream, cr);
    token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));

  } else if(FIRST_CHAR_OF_IDENTIFIER(cr)) {

    token = (Token)newPrimaryToken(this->line, this->column, stream, cr, this->stringBuilder, this->directives);
    if(token->code != GVLCP_CODE_IDENTIFIER) {
      groupId = token->code & GVLCP_MASK_DIRECTIVE_GROUPS;
      switch(groupId) {

      case GVLCP_CODE_GROUP_DEFINE: {
        this->nextToken = nextToken_define;
        STORED_DIRECTIVE_TOKEN = token;
      } break;

      case GVLCP_CODE_GROUP_ELIF: {
        this->nextToken = nextToken_elif_if_macro;
      } break;

      case GVLCP_CODE_GROUP_ENDIF: {
        this->nextToken = nextToken_superfluous;
      } break;

      case GVLCP_CODE_GROUP_IFDEF: {
        this->nextToken = nextToken_ifdef_ifndef_undef;
        STORED_DIRECTIVE_TOKEN = token;
      } break;

      case GVLCP_CODE_GROUP_INCLUDE: {
        this->nextToken = nextToken_include;
        STORED_DIRECTIVE_TOKEN = token;
      } break;

      case GVLCP_CODE_GROUP_LINE: {
        this->nextToken = nextToken_line;
        STORED_DIRECTIVE_TOKEN = token;
      } break;

      case GVLCP_CODE_GROUP_PRAGMA: {
        this->nextToken = nextToken_extra;
      } break;

#ifdef DEBUG_MODE
      default: {
        Errors.printMessageAndExit("Code corrupt: no such group");
      } break;
#endif

      }
    } else {
      setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_INVALID_DIRECTIVE);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      this->nextToken = nextToken_extra;
    }

  } else if(Char.isDigit(cr)) {

    token = (Token)newLinenumberToken(this->line, this->column, stream, cr, GVLCP_CODE_DIRECTIVE_LINE, this->stringBuilder);
    this->nextToken = nextToken_line_opt;

  } else {

    switch(cr) {

    case '/': {
      cr = stream->call->read(stream);
      switch(cr) {

      case '/': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder);
        this->nextToken = nextToken_base;
      } break;

      case '*': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));
      } break;

      default: {
        stream->call->unread(stream, cr);
        token = (Token)newExtraToken(this->line, this->column, stream, '/', this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_INVALID_DIRECTIVE);
        this->nextToken = nextToken_extra;
      } break;

      }
    } break;

    case '\\': {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      JOIN_LINES(this, token);
    } break;

    default: {
      token = (Token)newExtraToken(this->line, this->column, stream, cr, this->stringBuilder);
      setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_INVALID_DIRECTIVE);
      this->nextToken = nextToken_extra;
    } break;

    }

  }
  this->column += token->length;

  TEST_PREPROCESSOR_TOKEN_CODE(token);
  ___BTPOP;
  return token;
}

static Token nextToken_elif_if_macro(CTokeniser this, PushbackStream stream, int cr) {
  Token token;

  ___BTPUSH;
  HANDLE_PREPROCESSOR_LINE_STATE(this);

  if(this->commentOpen) {

    stream->call->unread(stream, cr);
    token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));

  } else if(FIRST_CHAR_OF_IDENTIFIER(cr)) {

    if(cr == 'L') {
      cr = stream->call->read(stream);
      if(cr != '\'') {
        stream->call->unread(stream, cr);
        token = (Token)newPrimaryToken(this->line, this->column, stream, 'L', this->stringBuilder, this->reservedWords);
      } else {
        token = (Token)newCharToken(this->line, this->column, stream, true, this->stringBuilder);
      }
    } else {
      token = (Token)newPrimaryToken(this->line, this->column, stream, cr, this->stringBuilder, this->reservedWords);
    }
    appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);

  } else if(Char.isDigit(cr)) {

    token = (Token)newNumberToken(this->line, this->column, stream, cr, this->stringBuilder);
    appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);

  } else {

    switch(cr) {

    case '"': {
      token = (Token)newStringToken(this->line, this->column, stream, this->stringBuilder);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
    } break;

    case '\'': {
      token = (Token)newCharToken(this->line, this->column, stream, false, this->stringBuilder);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
    } break;

    case '.': {
      cr = stream->call->read(stream);
      stream->call->unread(stream, cr);
      if(Char.isDigit(cr)) {
        token = (Token)newNumberToken(this->line, this->column, stream, '.', this->stringBuilder);
      } else {
        token = (Token)newSecondaryToken(this->line, this->column, stream, '.');
      }
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
    } break;

    case '/': {
      cr = stream->call->read(stream);
      switch(cr) {

      case '/': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder);
        this->nextToken = nextToken_base;
      } break;

      case '*': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));
      } break;

      default: {
        stream->call->unread(stream, cr);
        token = (Token)newSecondaryToken(this->line, this->column, stream, '/');
        appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      } break;

      }
    } break;

    case '\\': {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      JOIN_LINES(this, token);
    } break;

    default: {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
    } break;

    }

  }
  this->column += token->length;

  TEST_PREPROCESSOR_TOKEN_CODE(token);
  ___BTPOP;
  return token;
}

static Token nextToken_extra(CTokeniser this, PushbackStream stream, int cr) {
  Token token;

  ___BTPUSH;
  handlePreprocessorLineState(this);
  if(this->newLine > 0) {
    this->nextToken = nextToken_base;
    ___BTPOP;
    return nextToken_base(this, stream, cr);
  }

  if(this->commentOpen) {

    stream->call->unread(stream, cr);
    token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));

  } else {

    switch(cr) {

    case '/': {
      cr = stream->call->read(stream);
      switch(cr) {

      case '/': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder);
        this->nextToken = nextToken_base;
      } break;

      case '*': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));
      } break;

      default: {
        stream->call->unread(stream, cr);
        token = (Token)newExtraToken(this->line, this->column, stream, '/', this->stringBuilder);
      } break;

      }
    } break;

    case '\\': {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      JOIN_LINES(this, token);
    } break;

    default: {
      token = (Token)newExtraToken(this->line, this->column, stream, cr, this->stringBuilder);
    } break;

    }

  }
  this->column += token->length;

  TEST_PREPROCESSOR_TOKEN_CODE(token);
  ___BTPOP;
  return token;
}

static Token nextToken_ifdef_ifndef_undef(CTokeniser this, PushbackStream stream, int cr) {
  Token token;

  ___BTPUSH;
  HANDLE_PREPROCESSOR_LINE_STATE__WITH_ERROR_CODE(this, GVLCP_ERROR_NO_MACRO_NAME_GIVEN);

  if(this->commentOpen) {

    stream->call->unread(stream, cr);
    token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));

  } else if(FIRST_CHAR_OF_IDENTIFIER(cr)) {

    if(cr == 'L') {
      cr = stream->call->read(stream);
      stream->call->unread(stream, cr);
      if(cr != '\'') {
        token = (Token)newPrimaryToken(this->line, this->column, stream, 'L', this->stringBuilder, this->reservedWords);
      } else {
        token = (Token)newExtraToken(this->line, this->column, stream, 'L', this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_MACRO_NAMES_MUST_BE_IDENTIFIERS);
      }
    } else {
      token = (Token)newPrimaryToken(this->line, this->column, stream, cr, this->stringBuilder, this->reservedWords);
    }
    appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
    this->nextToken = nextToken_superfluous;

  } else {

    switch(cr) {

    case '/': {
      cr = stream->call->read(stream);
      switch(cr) {

      case '/': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)STORED_DIRECTIVE_TOKEN, GVLCP_ERROR_NO_MACRO_NAME_GIVEN);
        this->nextToken = nextToken_base;
      } break;

      case '*': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));
      } break;

      default: {
        stream->call->unread(stream, cr);
        token = (Token)newExtraToken(this->line, this->column, stream, '/', this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_MACRO_NAMES_MUST_BE_IDENTIFIERS);
        this->nextToken = nextToken_superfluous;
      } break;

      }
    } break;

    case '\\': {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      JOIN_LINES(this, token);
    } break;

    default: {
      token = (Token)newExtraToken(this->line, this->column, stream, cr, this->stringBuilder);
      setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_MACRO_NAMES_MUST_BE_IDENTIFIERS);
      this->nextToken = nextToken_superfluous;
    } break;

    }

  }
  this->column += token->length;

  TEST_PREPROCESSOR_TOKEN_CODE(token);
  ___BTPOP;
  return token;
}

static Token nextToken_include(CTokeniser this, PushbackStream stream, int cr) {
  Token token;

  ___BTPUSH;
  HANDLE_PREPROCESSOR_LINE_STATE__WITH_ERROR_CODE(this, GVLCP_ERROR_INCLUDE);

  if(this->commentOpen) {

    stream->call->unread(stream, cr);
    token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));

  } else {

    switch(cr) {

    case '/': {
      cr = stream->call->read(stream);
      switch(cr) {

      case '/': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)STORED_DIRECTIVE_TOKEN, GVLCP_ERROR_INCLUDE);
        this->nextToken = nextToken_base;
      } break;

      case '*': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));
      } break;

      default: {
        stream->call->unread(stream, cr);
        token = (Token)newExtraToken(this->line, this->column, stream, '/', this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_INCLUDE);
        this->nextToken = nextToken_extra;
      } break;

      }
    } break;

    case '\\': {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      JOIN_LINES(this, token);
    } break;

    default: {
      token = (Token)newIncludeToken(this->line, this->column, stream, cr, this->stringBuilder);
      if(Token_getErrwarn(token) == 0) {
        this->nextToken = nextToken_superfluous;
      } else {
        this->nextToken = nextToken_extra;
      }
    } break;

    }

  }
  this->column += token->length;

  TEST_PREPROCESSOR_TOKEN_CODE(token);
  ___BTPOP;
  return token;
}

static Token nextToken_line(CTokeniser this, PushbackStream stream, int cr) {
  Token token;

  ___BTPUSH;
  HANDLE_PREPROCESSOR_LINE_STATE__WITH_ERROR_CODE(this, GVLCP_ERROR_LINE_EXPECTS_NONNEGATIVE_INTEGER);

  if(this->commentOpen) {

    stream->call->unread(stream, cr);
    token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));

  } else if(Char.isDigit(cr)) {

    token = (Token)newLinenumberToken(this->line, this->column, stream, cr,
                                      GVLCP_CODE_NUMBER | GVLCP_CODE_INTEGER | GVLCP_CODE_PREPROCESSOR,
                                      this->stringBuilder);
    this->nextToken = nextToken_line_opt;

  } else {

    switch(cr) {

    case '/': {
      cr = stream->call->read(stream);
      switch(cr) {

      case '/': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)STORED_DIRECTIVE_TOKEN, GVLCP_ERROR_LINE_EXPECTS_NONNEGATIVE_INTEGER);
        this->nextToken = nextToken_base;
      } break;

      case '*': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));
      } break;

      default: {
        stream->call->unread(stream, cr);
        token = (Token)newExtraToken(this->line, this->column, stream, '/', this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_NOT_NONNEGATIVE_INTEGER);
        this->nextToken = nextToken_extra;
      } break;

      }
    } break;

    case '\\': {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      JOIN_LINES(this, token);
    } break;

    default: {
      token = (Token)newExtraToken(this->line, this->column, stream, cr, this->stringBuilder);
      setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_NOT_NONNEGATIVE_INTEGER);
      this->nextToken = nextToken_extra;
    } break;

    }

  }
  this->column += token->length;

  TEST_PREPROCESSOR_TOKEN_CODE(token);
  ___BTPOP;
  return token;
}

static Token nextToken_line_opt(CTokeniser this, PushbackStream stream, int cr) {
  Token token;

  ___BTPUSH;
  HANDLE_PREPROCESSOR_LINE_STATE(this);

  if(this->commentOpen) {

    stream->call->unread(stream, cr);
    token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));

  } else {

    switch(cr) {

    case '"': {
      token = (Token)newStringToken(this->line, this->column, stream, this->stringBuilder);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      this->nextToken = nextToken_superfluous;
    } break;

    case '/': {
      cr = stream->call->read(stream);
      switch(cr) {

      case '/': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder);
        this->nextToken = nextToken_base;
      } break;

      case '*': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));
      } break;

      default: {
        stream->call->unread(stream, cr);
        token = (Token)newExtraToken(this->line, this->column, stream, '/', this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_STRING_EXPECTED);
        this->nextToken = nextToken_extra;
      } break;

      }
    } break;

    case '\\': {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      JOIN_LINES(this, token);
    } break;

    default: {
      token = (Token)newExtraToken(this->line, this->column, stream, cr, this->stringBuilder);
      setErrwarn((struct private_tokenStruct*)token, GVLCP_ERROR_STRING_EXPECTED);
      this->nextToken = nextToken_extra;
    } break;

    }

  }
  this->column += token->length;

  TEST_PREPROCESSOR_TOKEN_CODE(token);
  ___BTPOP;
  return token;
}

static Token nextToken_superfluous(CTokeniser this, PushbackStream stream, int cr) {
  Token token;

  ___BTPUSH;
  HANDLE_PREPROCESSOR_LINE_STATE(this);

  if(this->commentOpen) {

    stream->call->unread(stream, cr);
    token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));

  } else {

    switch(cr) {

    case '/': {
      cr = stream->call->read(stream);
      switch(cr) {

      case '/': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder);
        this->nextToken = nextToken_base;
      } break;

      case '*': {
        token = (Token)newCommentToken(this->line, this->column, stream, this->stringBuilder, ptrof(this->commentOpen));
      } break;

      default: {
        stream->call->unread(stream, cr);
        token = (Token)newExtraToken(this->line, this->column, stream, '/', this->stringBuilder);
        setErrwarn((struct private_tokenStruct*)token, GVLCP_WARN_SUPERFLUOUS_TOKEN);
      } break;

      }
    } break;

    case '\\': {
      token = (Token)newSecondaryToken(this->line, this->column, stream, cr);
      appendCode((struct private_tokenStruct*)token, GVLCP_CODE_PREPROCESSOR);
      JOIN_LINES(this, token);
    } break;

    default: {
      token = (Token)newExtraToken(this->line, this->column, stream, cr, this->stringBuilder);
      setErrwarn((struct private_tokenStruct*)token, GVLCP_WARN_SUPERFLUOUS_TOKEN);
    } break;

    }

  }
  this->column += token->length;

  TEST_PREPROCESSOR_TOKEN_CODE(token);
  ___BTPOP;
  return token;
}

static void resetState(CTokeniser this) {
  ___BTPUSH;
  Tokeniser_resetState((Tokeniser)this);
  this->nextToken = nextToken_base;
  this->commentOpen = false;
  this->joinLines = false;
  ___BTPOP;
}

static TypeData rtType(void) {
  return C_TOKENISER_TYPE_DATA;
}

/* To avoid possible removal of code when compiled with option -O2 or -O3. */
static void setErrwarn(struct private_tokenStruct* token, int errwarnCode) {
  ___BTPUSH;
#ifdef DEBUG_MODE
  if(token == NULL) {
    Errors.printMessageAndExit("No token specified");
  }
  if(Token_getErrwarn(token) != 0) {
    Errors.printMessageAndExit("Cannot set error/warning for %s: it already has an error code", token->call->rtType()->name);
  }
#endif
  Token_setErrwarn(token, errwarnCode);
  ___BTPOP;
}
