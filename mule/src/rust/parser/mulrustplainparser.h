/*
   File:    mulrustplainparser.h
   Project: mule
   Author:  Douwe Vos
   Date:    Jan 9, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef RUST_PARSER_MULRUSTPLAINPARSER_H_
#define RUST_PARSER_MULRUSTPLAINPARSER_H_

#include "../mulrustscanner.h"
#include <caterpillar.h>
#include <dragonfly.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define MUL_TYPE_RUST_PLAIN_PARSER              (mul_rust_plain_parser_get_type())
#define MUL_RUST_PLAIN_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_rust_plain_parser_get_type(), MulRustPlainParser))
#define MUL_RUST_PLAIN_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_RUST_PLAIN_PARSER, MulRustPlainParserClass))
#define MUL_IS_RUST_PLAIN_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_RUST_PLAIN_PARSER))
#define MUL_IS_RUST_PLAIN_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_RUST_PLAIN_PARSER))
#define MUL_RUST_PLAIN_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_RUST_PLAIN_PARSER, MulRustPlainParserClass))


typedef struct _MulRustPlainParser               MulRustPlainParser;
typedef struct _MulRustPlainParserPrivate        MulRustPlainParserPrivate;
typedef struct _MulRustPlainParserClass          MulRustPlainParserClass;


struct _MulRustPlainParser {
	GObject parent;
};

struct _MulRustPlainParserClass {
	GObjectClass parent_class;
};


GType mul_rust_plain_parser_get_type();

MulRustPlainParser *mul_rust_plain_parser_new(MulRustScanner *token_scanner, GroRunITokenFactory *token_factory, GroRunModel *model, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer);

void mul_rust_plain_parser_run(MulRustPlainParser *parser);


G_END_DECLS

#endif /* RUST_PARSER_MULRUSTPLAINPARSER_H_ */
