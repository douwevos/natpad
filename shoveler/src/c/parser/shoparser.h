/*
   File:    shoparser.h
   Project: shoveler
   Author:  Douwe Vos
   Date:    May 7, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#ifndef SHOPARSER_H_
#define SHOPARSER_H_

#include "../model/shomodel.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_PARSER              (sho_parser_get_type())
#define SHO_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_parser_get_type(), ShoParser))
#define SHO_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_PARSER, ShoParserClass))
#define SHO_IS_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_PARSER))
#define SHO_IS_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_PARSER))
#define SHO_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_PARSER, ShoParserClass))

typedef struct _ShoParser               ShoParser;
typedef struct _ShoParserClass          ShoParserClass;


struct _ShoParser {
	GObject parent;
};

struct _ShoParserClass {
	GObjectClass parent_class;
};


GType sho_parser_get_type();

ShoParser *sho_parser_new();

ShoModel *sho_parser_parse(ShoParser *parser, CatIInputStream *stream_in);

G_END_DECLS

#endif /* SHOPARSER_H_ */
