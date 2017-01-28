/*
   File:    grorunparser.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 11, 2016
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

#ifndef GRORUNPARSER_H_
#define GRORUNPARSER_H_

#include "groruncontext.h"
#include "groruniscanner.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_PARSER              (grorun_parser_get_type())
#define GRORUN_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_parser_get_type(), GroRunParser))
#define GRORUN_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_PARSER, GroRunParserClass))
#define GRORUN_IS_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_PARSER))
#define GRORUN_IS_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_PARSER))
#define GRORUN_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_PARSER, GroRunParserClass))


typedef struct _GroRunParser               GroRunParser;
typedef struct _GroRunParserPrivate        GroRunParserPrivate;
typedef struct _GroRunParserClass          GroRunParserClass;


struct _GroRunParser {
	GObject parent;
};

struct _GroRunParserClass {
	GObjectClass parent_class;
};


GType grorun_parser_get_type();

GroRunParser *grorun_parser_new();

GObject *grorun_parser_do_parse(GroRunContext *context, GroRunIScanner *scanner);

G_END_DECLS

#endif /* GRORUNPARSER_H_ */
