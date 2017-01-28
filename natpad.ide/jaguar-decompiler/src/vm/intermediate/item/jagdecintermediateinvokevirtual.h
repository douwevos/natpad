/*
   File:    jagdecintermediateinvokevirtual.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 15, 2012
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


#ifndef JAGDECINTERMEDIATEINVOKEVIRTUAL_H_
#define JAGDECINTERMEDIATEINVOKEVIRTUAL_H_


#include "../jagdecabstractintermediateitem.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_INVOKE_VIRTUAL              (jag_dec_intermediate_invoke_virtual_get_type())
#define JAG_DEC_INTERMEDIATE_INVOKE_VIRTUAL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_invoke_virtual_get_type(), JagDecIntermediateInvokeVirtual))
#define JAG_DEC_INTERMEDIATE_INVOKE_VIRTUAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_INVOKE_VIRTUAL, JagDecIntermediateInvokeVirtualClass))
#define JAG_DEC_IS_INTERMEDIATE_INVOKE_VIRTUAL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_INVOKE_VIRTUAL))
#define JAG_DEC_IS_INTERMEDIATE_INVOKE_VIRTUAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_INVOKE_VIRTUAL))
#define JAG_DEC_INTERMEDIATE_INVOKE_VIRTUAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_INVOKE_VIRTUAL, JagDecIntermediateInvokeVirtualClass))
#define JAG_DEC_INTERMEDIATE_INVOKE_VIRTUAL_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_INVOKE_VIRTUAL, JagDecIntermediateInvokeVirtualPrivate))

typedef struct _JagDecIntermediateInvokeVirtual               JagDecIntermediateInvokeVirtual;
typedef struct _JagDecIntermediateInvokeVirtualPrivate        JagDecIntermediateInvokeVirtualPrivate;
typedef struct _JagDecIntermediateInvokeVirtualClass          JagDecIntermediateInvokeVirtualClass;


struct _JagDecIntermediateInvokeVirtual {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateInvokeVirtualPrivate *priv;
};

struct _JagDecIntermediateInvokeVirtualClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_invoke_virtual_get_type();

JagDecIntermediateInvokeVirtual *jag_dec_intermediate_invoke_virtual_new(int mnemonic_index, JagBytMethodHeader *method_header);

G_END_DECLS


#endif /* JAGDECINTERMEDIATEINVOKEVIRTUAL_H_ */
