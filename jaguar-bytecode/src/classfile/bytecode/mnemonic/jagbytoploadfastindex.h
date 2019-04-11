/*
   File:    jagbytoploadfastindex.h
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

#ifndef JAGBYTOPLOADFASTINDEX_H_
#define JAGBYTOPLOADFASTINDEX_H_

#include "../jagbytabstractmnemonic.h"
#include "../jagbyttype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_LOAD_FAST_INDEX              (jag_byt_op_load_fast_index_get_type())
#define JAG_BYT_OP_LOAD_FAST_INDEX(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_load_fast_index_get_type(), JagBytOpLoadFastIndex))
#define JAG_BYT_OP_LOAD_FAST_INDEX_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_LOAD_FAST_INDEX, JagBytOpLoadFastIndexClass))
#define JAG_BYT_IS_OP_LOAD_FAST_INDEX(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_LOAD_FAST_INDEX))
#define JAG_BYT_IS_OP_LOAD_FAST_INDEX_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_LOAD_FAST_INDEX))
#define JAG_BYT_OP_LOAD_FAST_INDEX_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_LOAD_FAST_INDEX, JagBytOpLoadFastIndexClass))
#define JAG_BYT_OP_LOAD_FAST_INDEX_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_LOAD_FAST_INDEX, JagBytOpLoadFastIndexPrivate))

typedef struct _JagBytOpLoadFastIndex               JagBytOpLoadFastIndex;
typedef struct _JagBytOpLoadFastIndexPrivate        JagBytOpLoadFastIndexPrivate;
typedef struct _JagBytOpLoadFastIndexClass          JagBytOpLoadFastIndexClass;


struct _JagBytOpLoadFastIndex {
	JagBytAbstractMnemonic parent;
	JagBytOpLoadFastIndexPrivate *priv;
};

struct _JagBytOpLoadFastIndexClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_load_fast_index_get_type();

JagBytOpLoadFastIndex *jag_byt_op_load_fast_index_new(JagBytOperation operation, int offset, JagBytType value_type, int frame_index);

int jag_byt_op_load_fast_index_get_frame_index(JagBytOpLoadFastIndex *load_fast_index);

G_END_DECLS

#endif /* JAGBYTOPLOADFASTINDEX_H_ */
