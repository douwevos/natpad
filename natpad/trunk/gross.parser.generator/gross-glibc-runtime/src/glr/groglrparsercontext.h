
/*
   File:    groglrparsercontext.h
   Project: gross-glibc-runtime
   Author:  Douwe Vos
   Date:    Jun 24, 2016
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

#ifndef GLR_GROGLRPARSERCONTEXT_H_
#define GLR_GROGLRPARSERCONTEXT_H_

#include "groforkentry.h"
#include "../groglibctypes.h"
#include "../groiscanner.h"

G_BEGIN_DECLS

#define GRO_TYPE_GLR_PARSER_CONTEXT              (gro_glr_parser_context_get_type())
#define GRO_GLR_PARSER_CONTEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_glr_parser_context_get_type(), GroGlrParserContext))
#define GRO_GLR_PARSER_CONTEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_GLR_PARSER_CONTEXT, GroGlrParserContextClass))
#define GRO_IS_GLR_PARSER_CONTEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_GLR_PARSER_CONTEXT))
#define GRO_IS_GLR_PARSER_CONTEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_GLR_PARSER_CONTEXT))
#define GRO_GLR_PARSER_CONTEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_GLR_PARSER_CONTEXT, GroGlrParserContextClass))


typedef struct _GroGlrParserContext               GroGlrParserContext;
typedef struct _GroGlrParserContextPrivate        GroGlrParserContextPrivate;
typedef struct _GroGlrParserContextClass          GroGlrParserContextClass;


struct _GroGlrParserContext {
	GObject parent;
};

struct _GroGlrParserContextClass {
	GObjectClass parent_class;
};


GType gro_glr_parser_context_get_type();

GroGlrParserContext *gro_glr_parser_context_new(GroIScanner *scanner);

GroForkEntry *gro_glr_parser_context_get_best_parse_result(GroGlrParserContext *context, gboolean allow_incomplete_parsing);


gboolean gro_glr_parser_context_parsing_done(GroGlrParserContext *context);
void gro_glr_parser_context_set_parsing_done(GroGlrParserContext *context, gboolean new_val);

void gro_glr_parser_context_init_state(GroGlrParserContext *context, int start_state);

int gro_glr_parser_context_fork_entry_count(GroGlrParserContext *context);
GroForkEntry *gro_glr_parser_context_fork_entry_at(GroGlrParserContext *context, int index);
void gro_glr_parser_context_insert_fork_entry(GroGlrParserContext *context, GroForkEntry *fe, int index);
void gro_glr_parser_context_remove_fork_entry(GroGlrParserContext *context, int index);


GroToken *gro_glr_parser_context_scan_next(GroGlrParserContext *context);

G_END_DECLS

#endif /* GLR_GROGLRPARSERCONTEXT_H_ */
