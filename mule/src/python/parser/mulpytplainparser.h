/*
   File:    mulpytplainparser.h
   Project: mule
   Author:  Douwe Vos
   Date:    Nov 14, 2016
   Web:     http://www.natpad.net/
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

#ifndef MULPYTPLAINPARSER_H_
#define MULPYTPLAINPARSER_H_

#include "../mulpytscanner.h"
#include <caterpillar.h>
#include <dragonfly.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define MUL_PYT_TYPE_PLAIN_PARSER              (mul_pyt_plain_parser_get_type())
#define MUL_PYT_PLAIN_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_pyt_plain_parser_get_type(), MulPytPlainParser))
#define MUL_PYT_PLAIN_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_PYT_TYPE_PLAIN_PARSER, MulPytPlainParserClass))
#define MUL_PYT_IS_PLAIN_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_PYT_TYPE_PLAIN_PARSER))
#define MUL_PYT_IS_PLAIN_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_PYT_TYPE_PLAIN_PARSER))
#define MUL_PYT_PLAIN_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_PYT_TYPE_PLAIN_PARSER, MulPytPlainParserClass))


typedef struct _MulPytPlainParser               MulPytPlainParser;
typedef struct _MulPytPlainParserPrivate        MulPytPlainParserPrivate;
typedef struct _MulPytPlainParserClass          MulPytPlainParserClass;


struct _MulPytPlainParser {
		GObject parent;
};

struct _MulPytPlainParserClass {
		GObjectClass parent_class;
};


GType mul_pyt_plain_parser_get_type();

MulPytPlainParser *mul_pyt_plain_parser_new(MulPytScanner *token_scanner, GroRunITokenFactory *token_factory, GroRunModel *model, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer);

void mul_pyt_plain_parser_run(MulPytPlainParser *parser);

G_END_DECLS

#endif /* MULPYTPLAINPARSER_H_ */
