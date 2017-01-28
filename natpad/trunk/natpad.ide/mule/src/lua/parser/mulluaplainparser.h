/*
   File:    spiplainparser.h
   Project: mule
   Author:  Douwe Vos
   Date:    Mai 7, 2016
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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

#ifndef MULLUAPLAINPARSER_H_
#define MULLUAPLAINPARSER_H_

#include "../mulluascanner.h"
#include <caterpillar.h>
#include <dragonfly.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define MUL_LUA_TYPE_PLAIN_PARSER              (mul_lua_plain_parser_get_type())
#define MUL_LUA_PLAIN_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_lua_plain_parser_get_type(), MulLuaPlainParser))
#define MUL_LUA_PLAIN_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_LUA_TYPE_PLAIN_PARSER, MulLuaPlainParserClass))
#define MUL_LUA_IS_PLAIN_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_LUA_TYPE_PLAIN_PARSER))
#define MUL_LUA_IS_PLAIN_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_LUA_TYPE_PLAIN_PARSER))
#define MUL_LUA_PLAIN_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_LUA_TYPE_PLAIN_PARSER, MulLuaPlainParserClass))


typedef struct _MulLuaPlainParser               MulLuaPlainParser;
typedef struct _MulLuaPlainParserPrivate        MulLuaPlainParserPrivate;
typedef struct _MulLuaPlainParserClass          MulLuaPlainParserClass;


struct _MulLuaPlainParser {
		GObject parent;
};

struct _MulLuaPlainParserClass {
		GObjectClass parent_class;
};


GType mul_lua_plain_parser_get_type();

MulLuaPlainParser *mul_lua_plain_parser_new(MulLuaScanner *token_scanner, GroRunITokenFactory *token_factory, GroRunModel *model, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer);

void mul_lua_plain_parser_run(MulLuaPlainParser *parser);

CatArrayWo *mul_lua_plain_parser_get_token_cash(MulLuaPlainParser *parser);

G_END_DECLS

#endif /* MULLUAPLAINPARSER_H_ */
