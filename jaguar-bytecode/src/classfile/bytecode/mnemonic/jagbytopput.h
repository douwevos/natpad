/*
   File:    jagbytopput.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 3, 2012
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

#ifndef JAGBYTOPPUT_H_
#define JAGBYTOPPUT_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_PUT              (jag_byt_op_put_get_type())
#define JAG_BYT_OP_PUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_put_get_type(), JagBytOpPut))
#define JAG_BYT_OP_PUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_PUT, JagBytOpPutClass))
#define JAG_BYT_IS_OP_PUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_PUT))
#define JAG_BYT_IS_OP_PUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_PUT))
#define JAG_BYT_OP_PUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_PUT, JagBytOpPutClass))
#define JAG_BYT_OP_PUT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_PUT, JagBytOpPutPrivate))

typedef struct _JagBytOpPut               JagBytOpPut;
typedef struct _JagBytOpPutPrivate        JagBytOpPutPrivate;
typedef struct _JagBytOpPutClass          JagBytOpPutClass;


struct _JagBytOpPut {
	JagBytAbstractMnemonic parent;
	JagBytOpPutPrivate *priv;
};

struct _JagBytOpPutClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_put_get_type();

JagBytOpPut *jag_byt_op_put_new(JagBytOperation operation, int offset, int pool_field_reference_index);

int jag_byt_op_put_get_field_reference_pool_index(JagBytOpPut *op_put);

G_END_DECLS

#endif /* JAGBYTOPPUT_H_ */
