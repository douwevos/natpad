/*
   File:    jagbytopstorearray.h
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


#ifndef JAGBYTOPSTOREARRAY_H_
#define JAGBYTOPSTOREARRAY_H_

#include "../jagbytabstractmnemonic.h"
#include "../jagbyttype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_STORE_ARRAY              (jag_byt_op_store_array_get_type())
#define JAG_BYT_OP_STORE_ARRAY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_store_array_get_type(), JagBytOpStoreArray))
#define JAG_BYT_OP_STORE_ARRAY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_STORE_ARRAY, JagBytOpStoreArrayClass))
#define JAG_BYT_IS_OP_STORE_ARRAY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_STORE_ARRAY))
#define JAG_BYT_IS_OP_STORE_ARRAY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_STORE_ARRAY))
#define JAG_BYT_OP_STORE_ARRAY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_STORE_ARRAY, JagBytOpStoreArrayClass))
#define JAG_BYT_OP_STORE_ARRAY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_STORE_ARRAY, JagBytOpStoreArrayPrivate))

typedef struct _JagBytOpStoreArray               JagBytOpStoreArray;
typedef struct _JagBytOpStoreArrayPrivate        JagBytOpStoreArrayPrivate;
typedef struct _JagBytOpStoreArrayClass          JagBytOpStoreArrayClass;


struct _JagBytOpStoreArray {
	JagBytAbstractMnemonic parent;
	JagBytOpStoreArrayPrivate *priv;
};

struct _JagBytOpStoreArrayClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_store_array_get_type();

JagBytOpStoreArray *jag_byt_op_store_array_new(JagBytOperation operation, int offset, JagBytType array_content_type);

JagBytType jag_byt_op_store_array_get_store_type(JagBytOpStoreArray *store_array);

G_END_DECLS


#endif /* JAGBYTOPSTOREARRAY_H_ */
