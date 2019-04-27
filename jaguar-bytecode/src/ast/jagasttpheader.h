/*
   File:    jagasttpheader.h
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

#ifndef JAGASTTPHEADER_H_
#define JAGASTTPHEADER_H_

#include "jagastmodifiers.h"
#include "../classfile/jagbytname.h"
#include "formater/jagastwriter.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_TP_HEADER              (jag_ast_tp_header_get_type())
#define JAG_AST_TP_HEADER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_tp_header_get_type(), JagAstTpHeader))
#define JAG_AST_TP_HEADER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_TP_HEADER, JagAstTpHeaderClass))
#define JAG_AST_IS_TP_HEADER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_TP_HEADER))
#define JAG_AST_IS_TP_HEADER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_TP_HEADER))
#define JAG_AST_TP_HEADER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_TP_HEADER, JagAstTpHeaderClass))

typedef struct _JagAstTpHeader               JagAstTpHeader;
typedef struct _JagAstTpHeaderPrivate        JagAstTpHeaderPrivate;
typedef struct _JagAstTpHeaderClass          JagAstTpHeaderClass;


struct _JagAstTpHeader {
	GObject parent;
};

struct _JagAstTpHeaderClass {
	GObjectClass parent_class;
};


GType jag_ast_tp_header_get_type();

JagAstTpHeader *jag_ast_tp_header_new(JagBytName *classname, JagAstModifiers *modifiers, JagBytName *supertype, CatArrayWo *e_interfaces);

void jag_ast_tp_header_write(JagAstTpHeader *tp_header, JagAstWriter *out);

G_END_DECLS

#endif /* JAGASTTPHEADER_H_ */
