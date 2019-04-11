/*
   File:    jagbytopreturn.h
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

#ifndef JAGBYTOPRETURN_H_
#define JAGBYTOPRETURN_H_

#include "../jagbytabstractmnemonic.h"
#include "../jagbyttype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_RETURN              (jag_byt_op_return_get_type())
#define JAG_BYT_OP_RETURN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_return_get_type(), JagBytOpReturn))
#define JAG_BYT_OP_RETURN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_RETURN, JagBytOpReturnClass))
#define JAG_BYT_IS_OP_RETURN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_RETURN))
#define JAG_BYT_IS_OP_RETURN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_RETURN))
#define JAG_BYT_OP_RETURN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_RETURN, JagBytOpReturnClass))
#define JAG_BYT_OP_RETURN_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_RETURN, JagBytOpReturnPrivate))

typedef struct _JagBytOpReturn               JagBytOpReturn;
typedef struct _JagBytOpReturnPrivate        JagBytOpReturnPrivate;
typedef struct _JagBytOpReturnClass          JagBytOpReturnClass;


struct _JagBytOpReturn {
	JagBytAbstractMnemonic parent;
	JagBytOpReturnPrivate *priv;
};

struct _JagBytOpReturnClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_return_get_type();

JagBytOpReturn *jag_byt_op_return_new(JagBytOperation operation, int offset, JagBytType return_type);

JagBytType jag_byt_op_return_get_return_type(JagBytOpReturn *op_return);

G_END_DECLS

#endif /* JAGBYTOPRETURN_H_ */
