/*
   File:    jagdecintermediateinvokestatic.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Jan 1, 2013
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#ifndef JAGDECINTERMEDIATEINVOKESTATIC_H_
#define JAGDECINTERMEDIATEINVOKESTATIC_H_

#include "../jagdecabstractintermediateitem.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_INVOKE_STATIC              (jag_dec_intermediate_invoke_static_get_type())
#define JAG_DEC_INTERMEDIATE_INVOKE_STATIC(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_invoke_static_get_type(), JagDecIntermediateInvokeStatic))
#define JAG_DEC_INTERMEDIATE_INVOKE_STATIC_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_INVOKE_STATIC, JagDecIntermediateInvokeStaticClass))
#define JAG_DEC_IS_INTERMEDIATE_INVOKE_STATIC(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_INVOKE_STATIC))
#define JAG_DEC_IS_INTERMEDIATE_INVOKE_STATIC_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_INVOKE_STATIC))
#define JAG_DEC_INTERMEDIATE_INVOKE_STATIC_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_INVOKE_STATIC, JagDecIntermediateInvokeStaticClass))
#define JAG_DEC_INTERMEDIATE_INVOKE_STATIC_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_INVOKE_STATIC, JagDecIntermediateInvokeStaticPrivate))

typedef struct _JagDecIntermediateInvokeStatic               JagDecIntermediateInvokeStatic;
typedef struct _JagDecIntermediateInvokeStaticPrivate        JagDecIntermediateInvokeStaticPrivate;
typedef struct _JagDecIntermediateInvokeStaticClass          JagDecIntermediateInvokeStaticClass;


struct _JagDecIntermediateInvokeStatic {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateInvokeStaticPrivate *priv;
};

struct _JagDecIntermediateInvokeStaticClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_invoke_static_get_type();

JagDecIntermediateInvokeStatic *jag_dec_intermediate_invoke_static_new(int mnemonic_index, JagAstDeclarationType *class_declaration_type, JagBytMethodHeader *method_header);

G_END_DECLS


#endif /* JAGDECINTERMEDIATEINVOKESTATIC_H_ */
