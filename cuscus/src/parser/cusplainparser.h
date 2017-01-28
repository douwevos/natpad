/*
   File:    cusplainparser.h
   Project: cuscus
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


#ifndef CUSPLAINPARSER_H_
#define CUSPLAINPARSER_H_

#include <caterpillar.h>
#include <dragonfly.h>
#include <gross.h>

G_BEGIN_DECLS

#define CUS_TYPE_PLAIN_PARSER              (cus_plain_parser_get_type())
#define CUS_PLAIN_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cus_plain_parser_get_type(), CusPlainParser))
#define CUS_PLAIN_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CUS_TYPE_PLAIN_PARSER, CusPlainParserClass))
#define CUS_IS_PLAIN_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CUS_TYPE_PLAIN_PARSER))
#define CUS_IS_PLAIN_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CUS_TYPE_PLAIN_PARSER))
#define CUS_PLAIN_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CUS_TYPE_PLAIN_PARSER, CusPlainParserClass))


typedef struct _CusPlainParser               CusPlainParser;
typedef struct _CusPlainParserPrivate        CusPlainParserPrivate;
typedef struct _CusPlainParserClass          CusPlainParserClass;


struct _CusPlainParser {
		GObject parent;
};

struct _CusPlainParserClass {
		GObjectClass parent_class;
};


GType cus_plain_parser_get_type();

CusPlainParser *cus_plain_parser_new(GroIScanner *delagate, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer);

void cus_plain_parser_run(CusPlainParser *parser);

G_END_DECLS

#endif /* CUSPLAINPARSER_H_ */
