/*
   File:    jagbytopget.h
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

#ifndef JAGBYTOPGET_H_
#define JAGBYTOPGET_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_GET              (jag_byt_op_get_get_type())
#define JAG_BYT_OP_GET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_get_get_type(), JagBytOpGet))
#define JAG_BYT_OP_GET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_GET, JagBytOpGetClass))
#define JAG_BYT_IS_OP_GET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_GET))
#define JAG_BYT_IS_OP_GET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_GET))
#define JAG_BYT_OP_GET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_GET, JagBytOpGetClass))
#define JAG_BYT_OP_GET_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_GET, JagBytOpGetPrivate))

typedef struct _JagBytOpGet               JagBytOpGet;
typedef struct _JagBytOpGetPrivate        JagBytOpGetPrivate;
typedef struct _JagBytOpGetClass          JagBytOpGetClass;


struct _JagBytOpGet {
	JagBytAbstractMnemonic parent;
	JagBytOpGetPrivate *priv;
};

struct _JagBytOpGetClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_get_get_type();

JagBytOpGet *jag_byt_op_get_new(JagBytOperation operation, int offset, int pool_field_reference_index);

int jag_byt_op_get_get_field_reference_pool_index(JagBytOpGet *op_get);

G_END_DECLS

#endif /* JAGBYTOPGET_H_ */
