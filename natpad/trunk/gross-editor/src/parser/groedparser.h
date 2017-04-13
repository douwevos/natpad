/*
   File:    groedparser.h
   Project: gross-editor
   Author:  Douwe Vos
   Date:    Nov 20, 2016
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

#ifndef PARSER_GROEDPARSER_H_
#define PARSER_GROEDPARSER_H_

#include <caterpillar.h>
#include <grossruntime.h>
#include <grossparser.h>

G_BEGIN_DECLS

#define GROED_TYPE_PARSER              (groed_parser_get_type())
#define GROED_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), groed_parser_get_type(), GroEdParser))
#define GROED_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GROED_TYPE_PARSER, GroEdParserClass))
#define GROED_IS_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GROED_TYPE_PARSER))
#define GROED_IS_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GROED_TYPE_PARSER))
#define GROED_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GROED_TYPE_PARSER, GroEdParserClass))


#define GROED_SYM_MARKED_TERMINAL     100
#define GROED_SYM_MARKED_NON_TERMINAL 101


typedef struct _GroEdParser               GroEdParser;
typedef struct _GroEdParserPrivate        GroEdParserPrivate;
typedef struct _GroEdParserClass          GroEdParserClass;


struct _GroEdParser {
	GObject parent;
};

struct _GroEdParserClass {
	GObjectClass parent_class;
};


GType groed_parser_get_type();

GroEdParser *groed_parser_new(GroScanner *scanner, GroRunITokenFactory *token_factory, GroRunModel *model);

GroAstSpec *groed_parser_run(GroEdParser *parser);

CatArrayWo *groed_parser_get_token_cash(GroEdParser *parser);

CatHashMapWo *groed_parser_get_non_terminal_map(GroEdParser *parser);
CatHashMapWo *groed_parser_get_terminal_map(GroEdParser *parser);

CatArrayWo *groed_parser_get_messages(GroEdParser *parser);

G_END_DECLS

#endif /* PARSER_GROEDPARSER_H_ */
