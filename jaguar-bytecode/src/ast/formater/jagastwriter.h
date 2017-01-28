/*
   File:    jagastwriter.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 28, 2012
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


#ifndef JAGASTWRITER_H_
#define JAGASTWRITER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_WRITER              (jag_ast_writer_get_type())
#define JAG_AST_WRITER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_writer_get_type(), JagAstWriter))
#define JAG_AST_WRITER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_WRITER, JagAstWriterClass))
#define JAG_AST_IS_WRITER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_WRITER))
#define JAG_AST_IS_WRITER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_WRITER))
#define JAG_AST_WRITER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_WRITER, JagAstWriterClass))
#define JAG_AST_WRITER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_WRITER, JagAstWriterPrivate))

typedef struct _JagAstWriter               JagAstWriter;
typedef struct _JagAstWriterPrivate        JagAstWriterPrivate;
typedef struct _JagAstWriterClass          JagAstWriterClass;


struct _JagAstWriter {
	GObject parent;
	JagAstWriterPrivate *priv;
};

struct _JagAstWriterClass {
	GObjectClass parent_class;
};


GType jag_ast_writer_get_type();

JagAstWriter *jag_ast_writer_new();

void jag_ast_writer_set_at_least_line_nr(JagAstWriter *writer, int lineNumber);

JagAstWriter *jag_ast_writer_print(JagAstWriter *writer, CatStringWo *a_text);

void jag_ast_writer_clear(JagAstWriter *writer);

void jag_ast_writer_increase_indent(JagAstWriter *writer);

void jag_ast_writer_decrease_indent(JagAstWriter *writer);

CatStringWo *jag_ast_writer_to_string(JagAstWriter *writer);

G_END_DECLS


#endif /* JAGASTWRITER_H_ */
