/*
   File:    jagpparser.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Mar 10, 2017
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

#ifndef JAGPPARSER_H_
#define JAGPPARSER_H_

#include "jagpscanner.h"
#include "jagpilexer.h"
#include "jagpnames.h"
#include "tree/jagpjccompilationunit.h"
#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

typedef enum _JagPParensResult JagPParensResult;

enum _JagPParensResult {
	JAGP_PAR_CAST,
	JAGP_PAR_EXPLICIT_LAMBDA,
	JAGP_PAR_IMPLICIT_LAMBDA,
	JAGP_PAR_PARENS
};



#define JAGP_TYPE_PARSER              (jagp_parser_get_type())
#define JAGP_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_parser_get_type(), JagPParser))
#define JAGP_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_PARSER, JagPParserClass))
#define JAGP_IS_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_PARSER))
#define JAGP_IS_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_PARSER))
#define JAGP_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_PARSER, JagPParserClass))


typedef struct _JagPParser               JagPParser;
typedef struct _JagPParserPrivate        JagPParserPrivate;
typedef struct _JagPParserClass          JagPParserClass;


struct _JagPParser {
	GObject parent;
};

struct _JagPParserClass {
	GObjectClass parent_class;
};


GType jagp_parser_get_type();

JagPParser *jagp_parser_new(JagPILexer *lexer, JagPNames *names);

/*GroAstSpec * */void jagp_parser_run(JagPParser *parser);

JagPJCCompilationUnit *jagp_parser_get_compilation_unit(JagPParser *parser);

CatArrayWo *jagp_parser_get_messages(JagPParser *parser);

G_END_DECLS

#endif /* JAGPPARSER_H_ */
