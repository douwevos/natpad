/*
   Author      : Gerard Visser
   e-mail      : visser.gerard(at)gmail.com

   Copyright (C) 2009, 2010 Gerard Visser.

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

#ifndef ____COMMENT_TOKEN_INCLUDED
#define ____COMMENT_TOKEN_INCLUDED

typedef struct commentTokenStruct* CommentToken;

#define COMMENT_TOKEN_TYPE_DATA ptrof(commentTokenTypeDataStruct)

#include <gvlib/defs.h>
#include <gvlib/Object.h>
#include <gvlib/String.h>
#include <gvlib/TypeData.h>
#include <gvlib/io/PushbackStream.h>

struct commentTokenFunctionsStruct {
  void (* dump)(CommentToken this);
  gvboolean (* equals)(CommentToken this, Object other);
  int (* hashCode)(CommentToken this);
  void (* reserve)(CommentToken this);
  TypeData (* rtType)(void);
};

struct commentTokenStruct {
  readonly refCount_t refCount;
  const struct commentTokenFunctionsStruct* const call;
  const String element; /* Is null if not in GVLCP_TEST_MODE. */
  const uint64_t value;
  const int column;
  const int line;
  const int code;
  const int length;
  const int errwarn;
  int code2;
};

extern const struct typeDataStruct commentTokenTypeDataStruct;

/* newCommentToken can be called in two different ways;
   in both ways, the first four arguments are the same:
    line : the line where the token to be created starts.
    column: the column where the token to be created starts.
    stream: the input stream from which to read the charactes.
    clearedSB: a string builder with clearedSB->length == 0 at the
               time of the call.

   1) newCommentToken(int line, int column, PushbackStream stream, StringBuilder clearedSB)
     This one should be called if the last two characters that were
     read from the stream are '/' and '/'.

   2) newCommentToken(int line, int column, PushbackStream stream, StringBuilder clearedSB, gvboolean* openPtr)
     This one should be called if:
     A) the last two characters that were read from the stream are
        '/' and '*';
     B) a previous CommentToken was created because of case 2A and
     it did not end in the characters '*' and '/'.

    openPtr: the address of a gvboolean.
             Let [openPtr] be the value that openPtr points to.
             In case 2A the value [openPtr] should be False.
             In case 2B the value [openPtr] should be True.
             The value [openPtr] will be set to:
             true: iff the last to characters of the token just
                   created were Not '*' and '/';
             false: if they were. */
CommentToken newCommentToken(int line, int column, PushbackStream stream, ...);

#ifdef GVLCP_TEST_MODE

# define newCommentToken(line, column, stream, clearedStringBuilder, ...) \
  newCommentToken(line, column, stream, clearedStringBuilder, ## __VA_ARGS__, NULL)

#else

# define newCommentToken(line, column, stream, clearedStringBuilder, ...) \
  newCommentToken(line, column, stream, ## __VA_ARGS__, NULL)

#endif

#endif
