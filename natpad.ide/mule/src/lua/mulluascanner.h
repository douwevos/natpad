/*
   File:    mulluascanner.h
   Project: mule
   Author:  Douwe Vos
   Date:    Oct 16, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#ifndef MULLUASCANNER_H_
#define MULLUASCANNER_H_

#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define MUL_LUA_SYM_END_OF_INPUT                   0
#define MUL_LUA_SYM_ERROR                          1
#define MUL_LUA_SYM_LBRACK                         2
#define MUL_LUA_SYM_RBRACK                         3
#define MUL_LUA_SYM_LBRACE                         4
#define MUL_LUA_SYM_RBRACE                         5
#define MUL_LUA_SYM_KWAND                          6
#define MUL_LUA_SYM_KWBREAK                        7
#define MUL_LUA_SYM_KWDO                           8
#define MUL_LUA_SYM_KWELSE                         9
#define MUL_LUA_SYM_KWELSEIF                       10
#define MUL_LUA_SYM_KWEND                          11
#define MUL_LUA_SYM_KWFALSE                        12
#define MUL_LUA_SYM_KWFOR                          13
#define MUL_LUA_SYM_KWFUNCTION                     14
#define MUL_LUA_SYM_KWIF                           15
#define MUL_LUA_SYM_KWIN                           16
#define MUL_LUA_SYM_KWLOCAL                        17
#define MUL_LUA_SYM_KWNIL                          18
#define MUL_LUA_SYM_KWNOT                          19
#define MUL_LUA_SYM_KWOR                           20
#define MUL_LUA_SYM_KWREPEAT                       21
#define MUL_LUA_SYM_KWRETURN                       22
#define MUL_LUA_SYM_KWTHEN                         23
#define MUL_LUA_SYM_KWTRUE                         24
#define MUL_LUA_SYM_KWUNTIL                        25
#define MUL_LUA_SYM_KWWHILE                        26
#define MUL_LUA_SYM_KWCONCAT                       27
#define MUL_LUA_SYM_ELLIPSES                       28
#define MUL_LUA_SYM_EQ                             29
#define MUL_LUA_SYM_GE                             30
#define MUL_LUA_SYM_LE                             31
#define MUL_LUA_SYM_NE                             32
#define MUL_LUA_SYM_OP_PLUS                        33
#define MUL_LUA_SYM_OP_MINUS                       34
#define MUL_LUA_SYM_OP_MUL                         35
#define MUL_LUA_SYM_OP_DIV                         36
#define MUL_LUA_SYM_OP_MOD                         37
#define MUL_LUA_SYM_OP_HASH                        38
#define MUL_LUA_SYM_OP_NEG                         39
#define MUL_LUA_SYM_SPECIAL                        40
#define MUL_LUA_SYM_SPECIAL_CONST                  41
#define MUL_LUA_SYM_SPECIAL_NUMBER                 42
#define MUL_LUA_SYM_OP_LT                          43
#define MUL_LUA_SYM_OP_GT                          44
#define MUL_LUA_SYM_NUMBER                         45
#define MUL_LUA_SYM_STRING                         46
#define MUL_LUA_SYM_IDENTIFIER                     47
#define MUL_LUA_SYM_UNARY_OPERATOR                 48
#define MUL_LUA_SYM_SEMICOLON                      49
#define MUL_LUA_SYM_OP_ASSIGN                      50
#define MUL_LUA_SYM_COMMA                          51
#define MUL_LUA_SYM_COLON                          52
#define MUL_LUA_SYM_DOT                            53
#define MUL_LUA_SYM_LPAREN                         54
#define MUL_LUA_SYM_RPAREN                         55

#define MUL_LUA_SYM_EOL_COMMENT                    56
#define MUL_LUA_SYM_STRING_CONTINUE                57


#define MUL_TYPE_LUA_SCANNER            (mul_lua_scanner_get_type())
#define MUL_LUA_SCANNER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_lua_scanner_get_type(), MulLuaScanner))
#define MUL_LUA_SCANNER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_LUA_SCANNER, MulLuaScannerClass))
#define MUL_IS_LUA_SCANNER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_LUA_SCANNER))
#define MUL_IS_LUA_SCANNER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_LUA_SCANNER))
#define MUL_LUA_SCANNER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_LUA_SCANNER, MulLuaScannerClass))

typedef struct _MulLuaScanner              MulLuaScanner;
typedef struct _MulLuaScannerPrivate       MulLuaScannerPrivate;
typedef struct _MulLuaScannerClass         MulLuaScannerClass;


struct _MulLuaScanner {
	GObject parent;
};


struct _MulLuaScannerClass {
	GObjectClass parent_class;
};

GType mul_lua_scanner_get_type(void);

MulLuaScanner *mul_lua_scanner_new(GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, GroRunSymbol **connected_symbols);

gboolean mul_lua_scanner_is_eof(MulLuaScanner *scanner, GroRunIToken *token);

G_END_DECLS

#endif /* MULLUASCANNER_H_ */
