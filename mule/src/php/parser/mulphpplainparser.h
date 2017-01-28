/*
   File:    mulphpplainparser.h
   Project: mule
   Author:  Douwe Vos
   Date:    Mai 7, 2016
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

#ifndef MULPHPPLAINPARSER_H_
#define MULPHPPLAINPARSER_H_

#include "../mulphpscanner.h"
#include <caterpillar.h>
#include <dragonfly.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define MUL_PHP_TYPE_PLAIN_PARSER              (mul_php_plain_parser_get_type())
#define MUL_PHP_PLAIN_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_php_plain_parser_get_type(), MulPhpPlainParser))
#define MUL_PHP_PLAIN_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_PHP_TYPE_PLAIN_PARSER, MulPhpPlainParserClass))
#define MUL_PHP_IS_PLAIN_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_PHP_TYPE_PLAIN_PARSER))
#define MUL_PHP_IS_PLAIN_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_PHP_TYPE_PLAIN_PARSER))
#define MUL_PHP_PLAIN_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_PHP_TYPE_PLAIN_PARSER, MulPhpPlainParserClass))


typedef struct _MulPhpPlainParser               MulPhpPlainParser;
typedef struct _MulPhpPlainParserPrivate        MulPhpPlainParserPrivate;
typedef struct _MulPhpPlainParserClass          MulPhpPlainParserClass;


struct _MulPhpPlainParser {
		GObject parent;
};

struct _MulPhpPlainParserClass {
		GObjectClass parent_class;
};


GType mul_php_plain_parser_get_type();

MulPhpPlainParser *mul_php_plain_parser_new(MulPhpScanner *token_scanner, GroRunITokenFactory *token_factory, GroRunModel *model, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer);

void mul_php_plain_parser_run(MulPhpPlainParser *parser);

G_END_DECLS

#endif /* MULPHPPLAINPARSER_H_ */
