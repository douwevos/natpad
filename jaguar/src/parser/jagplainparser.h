/*
   File:    jagplainparser.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Dec 18, 2011
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


#ifndef JAGPLAINPARSER_H_
#define JAGPLAINPARSER_H_

#include <caterpillar.h>
#include <dragonfly.h>
#include "generated/runtime/jagiscanner.h"

G_BEGIN_DECLS

#define JAG_TYPE_PLAIN_PARSER              (jag_plain_parser_get_type())
#define JAG_PLAIN_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_plain_parser_get_type(), JagPlainParser))
#define JAG_PLAIN_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PLAIN_PARSER, JagPlainParserClass))
#define JAG_IS_PLAIN_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PLAIN_PARSER))
#define JAG_IS_PLAIN_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PLAIN_PARSER))
#define JAG_PLAIN_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PLAIN_PARSER, JagPlainParserClass))
#define JAG_PLAIN_PARSER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_PLAIN_PARSER, JagPlainParserPrivate))


typedef struct _JagPlainParser               JagPlainParser;
typedef struct _JagPlainParserPrivate        JagPlainParserPrivate;
typedef struct _JagPlainParserClass          JagPlainParserClass;


struct _JagPlainParser {
		GObject parent;
		JagPlainParserPrivate *priv;
};

struct _JagPlainParserClass {
		GObjectClass parent_class;
};


GType jag_plain_parser_get_type();

JagPlainParser *jag_plain_parser_new(JagIScanner *delagate, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer);

void jag_plain_parser_run(JagPlainParser *parser);

G_END_DECLS

#endif /* JAGPLAINPARSER_H_ */
