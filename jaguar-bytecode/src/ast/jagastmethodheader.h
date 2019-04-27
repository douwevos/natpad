/*
   File:    jagastmethodheader.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 8, 2012
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

#ifndef JAGASTMETHODHEADER_H_
#define JAGASTMETHODHEADER_H_

#include "jagastidentifier.h"
#include "jagastdeclarationtype.h"
#include "jagastfielddeclaration.h"
#include "formater/jagastwriter.h"
#include "../classfile/jagbytname.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_METHOD_HEADER              (jag_ast_method_header_get_type())
#define JAG_AST_METHOD_HEADER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_method_header_get_type(), JagAstMethodHeader))
#define JAG_AST_METHOD_HEADER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_METHOD_HEADER, JagAstMethodHeaderClass))
#define JAG_AST_IS_METHOD_HEADER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_METHOD_HEADER))
#define JAG_AST_IS_METHOD_HEADER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_METHOD_HEADER))
#define JAG_AST_METHOD_HEADER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_METHOD_HEADER, JagAstMethodHeaderClass))

typedef struct _JagAstMethodHeader               JagAstMethodHeader;
typedef struct _JagAstMethodHeaderPrivate        JagAstMethodHeaderPrivate;
typedef struct _JagAstMethodHeaderClass          JagAstMethodHeaderClass;


struct _JagAstMethodHeader {
	GObject parent;
};

struct _JagAstMethodHeaderClass {
	GObjectClass parent_class;
};


GType jag_ast_method_header_get_type();

JagAstMethodHeader *jag_ast_method_header_new(JagAstIdentifier *method_name, gboolean is_constructor, JagAstDeclarationType *return_type, CatArrayWo *e_args, CatArrayWo *e_exceptions);

void jag_ast_method_header_write(JagAstMethodHeader *method_header, JagAstWriter *out);

JagAstDeclarationType *jag_ast_method_header_get_return_type(JagAstMethodHeader *method_header);

G_END_DECLS

#endif /* JAGASTMETHODHEADER_H_ */
