/*
   File:    jagbytopmultianewarray.h
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

#ifndef JAGBYTOPMULTIANEWARRAY_H_
#define JAGBYTOPMULTIANEWARRAY_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_MULTI_A_NEW_ARRAY              (jag_byt_op_multi_a_new_array_get_type())
#define JAG_BYT_OP_MULTI_A_NEW_ARRAY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_multi_a_new_array_get_type(), JagBytOpMultiANewArray))
#define JAG_BYT_OP_MULTI_A_NEW_ARRAY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_MULTI_A_NEW_ARRAY, JagBytOpMultiANewArrayClass))
#define JAG_BYT_IS_OP_MULTI_A_NEW_ARRAY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_MULTI_A_NEW_ARRAY))
#define JAG_BYT_IS_OP_MULTI_A_NEW_ARRAY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_MULTI_A_NEW_ARRAY))
#define JAG_BYT_OP_MULTI_A_NEW_ARRAY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_MULTI_A_NEW_ARRAY, JagBytOpMultiANewArrayClass))
#define JAG_BYT_OP_MULTI_A_NEW_ARRAY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_MULTI_A_NEW_ARRAY, JagBytOpMultiANewArrayPrivate))

typedef struct _JagBytOpMultiANewArray               JagBytOpMultiANewArray;
typedef struct _JagBytOpMultiANewArrayPrivate        JagBytOpMultiANewArrayPrivate;
typedef struct _JagBytOpMultiANewArrayClass          JagBytOpMultiANewArrayClass;


struct _JagBytOpMultiANewArray {
	JagBytAbstractMnemonic parent;
	JagBytOpMultiANewArrayPrivate *priv;
};

struct _JagBytOpMultiANewArrayClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_multi_a_new_array_get_type();

JagBytOpMultiANewArray *jag_byt_op_multi_a_new_array_new(int offset, int class_pool_index, int dim_count);

int jag_byt_op_multi_a_new_array_get_class_info_pool_index(JagBytOpMultiANewArray *op_multi_new_array);

int jag_byt_op_multi_a_new_array_get_nr_of_dimensions(JagBytOpMultiANewArray *op_multi_new_array);

G_END_DECLS

#endif /* JAGBYTOPMULTIANEWARRAY_H_ */
