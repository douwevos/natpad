
/*
   File:    groglrparser.h
   Project: gross-glibc-runtime
   Author:  Douwe Vos
   Date:    Jun 22, 2016
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

#ifndef GROGLRPARSER_H_
#define GROGLRPARSER_H_

#include "groglrparsercontext.h"
#include "../groglibctypes.h"

G_BEGIN_DECLS

struct _GroParserConfig {
	int const *production_table;
	int const *action_table;
	int const *reduce_table;
	int const *fork_table;
	int start_state;
	int start_production;
	int eof_symbol;
	int error_symbol;
};

typedef struct _GroParserConfig GroParserConfig;


#define GRO_TYPE_GLR_PARSER              (gro_glr_parser_get_type())
#define GRO_GLR_PARSER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_glr_parser_get_type(), GroGlrParser))
#define GRO_GLR_PARSER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_GLR_PARSER, GroGlrParserClass))
#define GRO_IS_GLR_PARSER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_GLR_PARSER))
#define GRO_IS_GLR_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_GLR_PARSER))
#define GRO_GLR_PARSER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_GLR_PARSER, GroGlrParserClass))


typedef struct _GroGlrParser               GroGlrParser;
typedef struct _GroGlrParserPrivate        GroGlrParserPrivate;
typedef struct _GroGlrParserClass          GroGlrParserClass;


struct _GroGlrParser {
	GObject parent;
};

struct _GroGlrParserClass {
	GObjectClass parent_class;
};

typedef GroContent *(*GroGlrRunActionCB)(GroGlrParserContext *context, int action_id, GroContent *content, int lhs, int rhs_count);


GType gro_glr_parser_get_type();

GroGlrParser *gro_glr_parser_new(GroParserConfig *parser_config, GroGlrRunActionCB run_action_cb);

void gro_glr_parser_parse(GroGlrParser *parser, GroGlrParserContext *context);

int gro_glr_parser_parse_get_action(GroGlrParser *parser, int state, int sym);

int gro_glr_parser_parse_get_reduce(GroGlrParser *parser, int state, int sym);

G_END_DECLS

#endif /* GROGLRPARSER_H_ */
