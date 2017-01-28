/*
   File:    jagbytopinvoke.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 4, 2012
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


#ifndef JAGBYTOPINVOKE_H_
#define JAGBYTOPINVOKE_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_INVOKE              (jag_byt_op_invoke_get_type())
#define JAG_BYT_OP_INVOKE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_invoke_get_type(), JagBytOpInvoke))
#define JAG_BYT_OP_INVOKE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_INVOKE, JagBytOpInvokeClass))
#define JAG_BYT_IS_OP_INVOKE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_INVOKE))
#define JAG_BYT_IS_OP_INVOKE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_INVOKE))
#define JAG_BYT_OP_INVOKE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_INVOKE, JagBytOpInvokeClass))
#define JAG_BYT_OP_INVOKE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_INVOKE, JagBytOpInvokePrivate))

typedef struct _JagBytOpInvoke               JagBytOpInvoke;
typedef struct _JagBytOpInvokePrivate        JagBytOpInvokePrivate;
typedef struct _JagBytOpInvokeClass          JagBytOpInvokeClass;


struct _JagBytOpInvoke {
	JagBytAbstractMnemonic parent;
	JagBytOpInvokePrivate *priv;
};

struct _JagBytOpInvokeClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_invoke_get_type();

JagBytOpInvoke *jag_byt_op_invoke_new(JagBytOperation operation, int offset, int methodref_pool_index);
JagBytOpInvoke *jag_byt_op_invoke_new_interface(int offset, int methodref_pool_index, int count);

int jag_byt_op_invoke_get_methodref_pool_index(JagBytOpInvoke *op_invoke);

int jag_byt_op_invoke_get_count(JagBytOpInvoke *op_invoke);

G_END_DECLS


#endif /* JAGBYTOPINVOKE_H_ */
