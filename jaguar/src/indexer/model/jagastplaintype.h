/*
   File:    jagastplaintype.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 23, 2012
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

#ifndef JAGASTPLAINTYPE_H_
#define JAGASTPLAINTYPE_H_

#include "jagasttype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_AST_PLAIN_TYPE              (jag_ast_plain_type_get_type())
#define JAG_AST_PLAIN_TYPE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_plain_type_get_type(), JagAstPlainType))
#define JAG_AST_PLAIN_TYPE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_AST_PLAIN_TYPE, JagAstPlainTypeClass))
#define JAG_IS_AST_PLAIN_TYPE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_AST_PLAIN_TYPE))
#define JAG_IS_AST_PLAIN_TYPE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_AST_PLAIN_TYPE))
#define JAG_AST_PLAIN_TYPE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_AST_PLAIN_TYPE, JagAstPlainTypeClass))
#define JAG_AST_PLAIN_TYPE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_AST_PLAIN_TYPE, JagAstPlainTypePrivate))

typedef struct _JagAstPlainType               JagAstPlainType;
typedef struct _JagAstPlainTypePrivate        JagAstPlainTypePrivate;
typedef struct _JagAstPlainTypeClass          JagAstPlainTypeClass;


struct _JagAstPlainType {
	JagAstType parent;
	JagAstPlainTypePrivate *priv;
};

struct _JagAstPlainTypeClass {
	JagAstTypeClass parent_class;
	volatile gsize init_prims_once;
	JagAstPlainType *primitive_boolean;
	JagAstPlainType *primitive_byte;
	JagAstPlainType *primitive_short;
	JagAstPlainType *primitive_char;
	JagAstPlainType *primitive_int;
	JagAstPlainType *primitive_long;
	JagAstPlainType *primitive_float;
	JagAstPlainType *primitive_double;
};


GType jag_ast_plain_type_get_type();

JagAstPlainType *jag_ast_plain_type_new(CatStringWo *a_name, gboolean isQualifiedName);


JagAstPlainType *jag_ast_plain_type_get_boolean();
JagAstPlainType *jag_ast_plain_type_get_byte();
JagAstPlainType *jag_ast_plain_type_get_short();
JagAstPlainType *jag_ast_plain_type_get_char();
JagAstPlainType *jag_ast_plain_type_get_int();
JagAstPlainType *jag_ast_plain_type_get_long();
JagAstPlainType *jag_ast_plain_type_get_float();
JagAstPlainType *jag_ast_plain_type_get_double();

CatStringWo *jag_ast_plain_type_get_name(const JagAstPlainType *plain_type);
gboolean jag_ast_plain_type_is_qualified(const JagAstPlainType *plain_type);

gboolean jag_ast_plain_type_equal(const JagAstPlainType *type_a, const JagAstPlainType *type_b);

CatStringWo *jag_ast_plain_type_to_string(const JagAstPlainType *plain_type);

G_END_DECLS

#endif /* JAGASTPLAINTYPE_H_ */
