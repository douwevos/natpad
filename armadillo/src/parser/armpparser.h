/*
   File:    armpparser.h
   Project: armadillo
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

#ifndef ARMPPARSER_H_
#define ARMPPARSER_H_

#include "armpscanner.h"
#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define ARMP_TYPE_PARSER              (armp_parser_get_type())
#define ARMP_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), armp_parser_get_type(), ArmPParser))
#define ARMP_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ARMP_TYPE_PARSER, ArmPParserClass))
#define ARMP_IS_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ARMP_TYPE_PARSER))
#define ARMP_IS_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ARMP_TYPE_PARSER))
#define ARMP_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ARMP_TYPE_PARSER, ArmPParserClass))


typedef struct _ArmPParser               ArmPParser;
typedef struct _ArmPParserPrivate        ArmPParserPrivate;
typedef struct _ArmPParserClass          ArmPParserClass;


struct _ArmPParser {
	GObject parent;
};

struct _ArmPParserClass {
	GObjectClass parent_class;
};


GType armp_parser_get_type();

ArmPParser *armp_parser_new(ArmPScanner *scanner, GroRunITokenFactory *token_factory, GroRunModel *model);

/*GroAstSpec * */void armp_parser_run(ArmPParser *parser);

CatArrayWo *armp_parser_get_token_cash(ArmPParser *parser);

CatArrayWo *armp_parser_get_messages(ArmPParser *parser);

G_END_DECLS

#endif /* ARMPPARSER_H_ */
