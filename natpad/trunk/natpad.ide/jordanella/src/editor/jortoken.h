/*
   File:    jortoken.h
   Project: jordanella
   Author:  Douwe Vos
   Date:    Feb 6, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef EDITOR_JORTOKEN_H_
#define EDITOR_JORTOKEN_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JOR_SYMBOL_EOF                     0
#define JOR_SYMBOL_LIST_OPEN               1
#define JOR_SYMBOL_LIST_CLOSE              2
#define JOR_SYMBOL_MAP_OPEN                3
#define JOR_SYMBOL_MAP_CLOSE               4
#define JOR_SYMBOL_STRING                  5
#define JOR_SYMBOL_COMMA                   6
#define JOR_SYMBOL_COLON                   7
#define JOR_SYMBOL_NULL                    8
#define JOR_SYMBOL_TRUE                    9
#define JOR_SYMBOL_FALSE                   10
#define JOR_SYMBOL_NUMBER                  11
#define JOR_SYMBOL_KEY                     12
#define JOR_SYMBOL_INVALID                 13

#define JOR_NTERM_VALUE                 14

#define JOR_TYPE_TOKEN              (jor_token_get_type())
#define JOR_TOKEN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jor_token_get_type(), JorToken))
#define JOR_TOKEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JOR_TYPE_TOKEN, JorTokenClass))
#define JOR_IS_TOKEN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JOR_TYPE_TOKEN))
#define JOR_IS_TOKEN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JOR_TYPE_TOKEN))
#define JOR_TOKEN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JOR_TYPE_TOKEN, JorTokenClass))


typedef struct _JorToken               JorToken;
typedef struct _JorTokenClass          JorTokenClass;


struct _JorToken {
	GObject parent;
	int code;
	int start_column;
	long long start_row;
	int end_column;
	long long end_row;
};

struct _JorTokenClass {
	GObjectClass parent_class;
};


GType jor_token_get_type();

JorToken *jor_token_new(int code, int start_column, long long start_row, int end_column, long long end_row);

const char *jor_token_code_as_text(int code);

G_END_DECLS


#endif /* EDITOR_JORTOKEN_H_ */
