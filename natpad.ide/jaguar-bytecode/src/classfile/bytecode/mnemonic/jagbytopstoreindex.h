/*
   File:    jagbytopstoreindex.h
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


#ifndef JAGBYTOPSTOREINDEX_H_
#define JAGBYTOPSTOREINDEX_H_

#include "../jagbytabstractmnemonic.h"
#include "../jagbyttype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_STORE_INDEX              (jag_byt_op_store_index_get_type())
#define JAG_BYT_OP_STORE_INDEX(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_store_index_get_type(), JagBytOpStoreIndex))
#define JAG_BYT_OP_STORE_INDEX_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_STORE_INDEX, JagBytOpStoreIndexClass))
#define JAG_BYT_IS_OP_STORE_INDEX(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_STORE_INDEX))
#define JAG_BYT_IS_OP_STORE_INDEX_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_STORE_INDEX))
#define JAG_BYT_OP_STORE_INDEX_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_STORE_INDEX, JagBytOpStoreIndexClass))
#define JAG_BYT_OP_STORE_INDEX_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_STORE_INDEX, JagBytOpStoreIndexPrivate))

typedef struct _JagBytOpStoreIndex               JagBytOpStoreIndex;
typedef struct _JagBytOpStoreIndexPrivate        JagBytOpStoreIndexPrivate;
typedef struct _JagBytOpStoreIndexClass          JagBytOpStoreIndexClass;


struct _JagBytOpStoreIndex {
	JagBytAbstractMnemonic parent;
	JagBytOpStoreIndexPrivate *priv;
};

struct _JagBytOpStoreIndexClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_store_index_get_type();

JagBytOpStoreIndex *jag_byt_op_store_index_new(JagBytOperation operation, int offset, JagBytType value_type, int index, gboolean is_wide);

int jag_byt_op_store_index_get_frame_index(JagBytOpStoreIndex *store_index);

G_END_DECLS


#endif /* JAGBYTOPSTOREINDEX_H_ */
