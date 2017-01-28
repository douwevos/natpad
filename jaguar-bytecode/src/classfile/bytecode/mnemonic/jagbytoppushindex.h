/*
   File:    jagbytoppushindex.h
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


#ifndef JAGBYTOPPUSHINDEX_H_
#define JAGBYTOPPUSHINDEX_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_PUSH_INDEX              (jag_byt_op_push_index_get_type())
#define JAG_BYT_OP_PUSH_INDEX(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_push_index_get_type(), JagBytOpPushIndex))
#define JAG_BYT_OP_PUSH_INDEX_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_PUSH_INDEX, JagBytOpPushIndexClass))
#define JAG_BYT_IS_OP_PUSH_INDEX(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_PUSH_INDEX))
#define JAG_BYT_IS_OP_PUSH_INDEX_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_PUSH_INDEX))
#define JAG_BYT_OP_PUSH_INDEX_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_PUSH_INDEX, JagBytOpPushIndexClass))
#define JAG_BYT_OP_PUSH_INDEX_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_PUSH_INDEX, JagBytOpPushIndexPrivate))

typedef struct _JagBytOpPushIndex               JagBytOpPushIndex;
typedef struct _JagBytOpPushIndexPrivate        JagBytOpPushIndexPrivate;
typedef struct _JagBytOpPushIndexClass          JagBytOpPushIndexClass;


struct _JagBytOpPushIndex {
	JagBytAbstractMnemonic parent;
	JagBytOpPushIndexPrivate *priv;
};

struct _JagBytOpPushIndexClass {
	JagBytAbstractMnemonicClass parent_class;
};

GType jag_byt_op_push_index_get_type();

JagBytOpPushIndex *jag_byt_op_push_index_new(JagBytOperation operation, int offset, int length, gboolean is_wide, int index, gboolean is_long_or_double);

int jag_byt_op_push_index_get_pool_index(JagBytOpPushIndex *op_push_index);

G_END_DECLS


#endif /* JAGBYTOPPUSHINDEX_H_ */
