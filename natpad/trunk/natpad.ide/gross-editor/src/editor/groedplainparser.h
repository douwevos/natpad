///*
//   File:    groedplainparser.h
//   Project: gross-editor
//   Author:  Douwe Vos
//   Date:    Oct 12, 2016
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2016 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#ifndef EDITOR_GROEDPLAINPARSER_H_
//#define EDITOR_GROEDPLAINPARSER_H_
//
//#include <caterpillar.h>
//#include <dragonfly.h>
//#include <grossruntime.h>
//#include <grossparser.h>
//
//G_BEGIN_DECLS
//
//#define GROED_TYPE_PLAIN_PARSER              (groed_plain_parser_get_type())
//#define GROED_PLAIN_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), groed_plain_parser_get_type(), GroEdPlainParser))
//#define GROED_PLAIN_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GROED_TYPE_PLAIN_PARSER, GroEdPlainParserClass))
//#define GROED_IS_PLAIN_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GROED_TYPE_PLAIN_PARSER))
//#define GROED_IS_PLAIN_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GROED_TYPE_PLAIN_PARSER))
//#define GROED_PLAIN_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GROED_TYPE_PLAIN_PARSER, GroEdPlainParserClass))
//
//
//typedef struct _GroEdPlainParser               GroEdPlainParser;
//typedef struct _GroEdPlainParserPrivate        GroEdPlainParserPrivate;
//typedef struct _GroEdPlainParserClass          GroEdPlainParserClass;
//
//
//struct _GroEdPlainParser {
//	GObject parent;
//};
//
//struct _GroEdPlainParserClass {
//	GObjectClass parent_class;
//};
//
//
//GType groed_plain_parser_get_type();
//
//GroEdPlainParser *groed_plain_parser_new(GroScanner *scanner, GroRunITokenFactory *token_factory, GroRunModel *model, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer);
//
//void groed_plain_parser_run(GroEdPlainParser *parser);
//
//G_END_DECLS
//
//#endif /* EDITOR_GROEDPLAINPARSER_H_ */
