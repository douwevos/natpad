/*
   File:    xanparser.h
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 17, 2017
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

#ifndef PARSER_XANPARSER_H_
#define PARSER_XANPARSER_H_

#include "xanscanner.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define XAN_TYPE_PARSER              (xan_parser_get_type())
#define XAN_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), xan_parser_get_type(), XanParser))
#define XAN_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XAN_TYPE_PARSER, XanParserClass))
#define XAN_IS_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAN_TYPE_PARSER))
#define XAN_IS_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XAN_TYPE_PARSER))
#define XAN_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XAN_TYPE_PARSER, XanParserClass))


typedef struct _XanParser               XanParser;
typedef struct _XanParserPrivate        XanParserPrivate;
typedef struct _XanParserClass          XanParserClass;


struct _XanParser {
	GObject parent;
};

struct _XanParserClass {
	GObjectClass parent_class;
};


GType xan_parser_get_type();

XanParser *xan_parser_new(XanScanner *scanner);

G_END_DECLS

#endif /* PARSER_XANPARSER_H_ */
