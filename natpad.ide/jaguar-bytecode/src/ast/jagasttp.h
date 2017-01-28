/*
   File:    jagasttp.h
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


#ifndef JAGASTTP_H_
#define JAGASTTP_H_

#include "jagastpackagestatement.h"
#include "jagastimportstatement.h"
#include "jagasttpheader.h"
#include "jagastfielddeclaration.h"
#include "jagastmethod.h"
#include "formater/jagastwriter.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_TP              (jag_ast_tp_get_type())
#define JAG_AST_TP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_tp_get_type(), JagAstTp))
#define JAG_AST_TP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_TP, JagAstTpClass))
#define JAG_AST_IS_TP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_TP))
#define JAG_AST_IS_TP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_TP))
#define JAG_AST_TP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_TP, JagAstTpClass))
#define JAG_AST_TP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_TP, JagAstTpPrivate))

typedef struct _JagAstTp               JagAstTp;
typedef struct _JagAstTpPrivate        JagAstTpPrivate;
typedef struct _JagAstTpClass          JagAstTpClass;


struct _JagAstTp {
	GObject parent;
	JagAstTpPrivate *priv;
};

struct _JagAstTpClass {
	GObjectClass parent_class;
};


GType jag_ast_tp_get_type();

JagAstTp *jag_ast_tp_new(JagAstPackageStatement *package_statement, CatArrayWo *e_imports, JagAstTpHeader *type_header, CatArrayWo *e_fields, CatArrayWo *e_methods);


JagAstFieldDeclaration *jag_ast_tp_find_field_by_name(JagAstTp *tp, CatStringWo *a_field_name);

void jag_ast_tp_write(JagAstTp *tp, JagAstWriter *out);

G_END_DECLS

#endif /* JAGASTTP_H_ */
