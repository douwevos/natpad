/*
   File:    jagbytoploadconstant.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 2, 2012
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

#ifndef JAGBYTOPLOADCONSTANT_H_
#define JAGBYTOPLOADCONSTANT_H_

#include "../jagbytabstractmnemonic.h"
#include "../jagbyttype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_LOAD_CONSTANT              (jag_byt_op_load_constant_get_type())
#define JAG_BYT_OP_LOAD_CONSTANT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_load_constant_get_type(), JagBytOpLoadConstant))
#define JAG_BYT_OP_LOAD_CONSTANT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_LOAD_CONSTANT, JagBytOpLoadConstantClass))
#define JAG_BYT_IS_OP_LOAD_CONSTANT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_LOAD_CONSTANT))
#define JAG_BYT_IS_OP_LOAD_CONSTANT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_LOAD_CONSTANT))
#define JAG_BYT_OP_LOAD_CONSTANT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_LOAD_CONSTANT, JagBytOpLoadConstantClass))
#define JAG_BYT_OP_LOAD_CONSTANT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_LOAD_CONSTANT, JagBytOpLoadConstantPrivate))

typedef struct _JagBytOpLoadConstant               JagBytOpLoadConstant;
typedef struct _JagBytOpLoadConstantPrivate        JagBytOpLoadConstantPrivate;
typedef struct _JagBytOpLoadConstantClass          JagBytOpLoadConstantClass;


struct _JagBytOpLoadConstant {
	JagBytAbstractMnemonic parent;
	JagBytOpLoadConstantPrivate *priv;
};

struct _JagBytOpLoadConstantClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_load_constant_get_type();

JagBytOpLoadConstant *jag_byt_op_load_constant_new(JagBytOperation operation, JagBytType constant_type, CatIStringable *constant_value, int offset);

JagBytType jag_byt_op_load_constant_get_constant_type(JagBytOpLoadConstant *load_constant);

CatIStringable *jag_byt_op_load_constant_get_contant_value(JagBytOpLoadConstant *load_constant);

G_END_DECLS

#endif /* JAGBYTOPLOADCONSTANT_H_ */
