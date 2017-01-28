/*
   File:    mulvhdlplainparser.h
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

#ifndef MULVHDLPLAINPARSER_H_
#define MULVHDLPLAINPARSER_H_

#include "../mulvhdlscanner.h"
#include <caterpillar.h>
#include <dragonfly.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define MUL_VHDL_TYPE_PLAIN_PARSER              (mul_vhdl_plain_parser_get_type())
#define MUL_VHDL_PLAIN_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_vhdl_plain_parser_get_type(), MulVhdlPlainParser))
#define MUL_VHDL_PLAIN_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_VHDL_TYPE_PLAIN_PARSER, MulVhdlPlainParserClass))
#define MUL_VHDL_IS_PLAIN_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_VHDL_TYPE_PLAIN_PARSER))
#define MUL_VHDL_IS_PLAIN_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_VHDL_TYPE_PLAIN_PARSER))
#define MUL_VHDL_PLAIN_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_VHDL_TYPE_PLAIN_PARSER, MulVhdlPlainParserClass))


typedef struct _MulVhdlPlainParser               MulVhdlPlainParser;
typedef struct _MulVhdlPlainParserPrivate        MulVhdlPlainParserPrivate;
typedef struct _MulVhdlPlainParserClass          MulVhdlPlainParserClass;


struct _MulVhdlPlainParser {
		GObject parent;
};

struct _MulVhdlPlainParserClass {
		GObjectClass parent_class;
};


GType mul_vhdl_plain_parser_get_type();

MulVhdlPlainParser *mul_vhdl_plain_parser_new(MulVhdlScanner *token_scanner, GroRunITokenFactory *token_factory, GroRunModel *model, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer);

void mul_vhdl_plain_parser_run(MulVhdlPlainParser *parser);

G_END_DECLS

#endif /* MULVHDLPLAINPARSER_H_ */
