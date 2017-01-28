/*
   File:    jagbytloadnull.h
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


#ifndef JAGBYTLOADNULL_H_
#define JAGBYTLOADNULL_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_LOAD_NULL              (jag_byt_op_load_null_get_type())
#define JAG_BYT_OP_LOAD_NULL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_load_null_get_type(), JagBytOpLoadNull))
#define JAG_BYT_OP_LOAD_NULL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_LOAD_NULL, JagBytOpLoadNullClass))
#define JAG_BYT_IS_OP_LOAD_NULL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_LOAD_NULL))
#define JAG_BYT_IS_OP_LOAD_NULL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_LOAD_NULL))
#define JAG_BYT_OP_LOAD_NULL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_LOAD_NULL, JagBytOpLoadNullClass))

typedef struct _JagBytOpLoadNull               JagBytOpLoadNull;
typedef struct _JagBytOpLoadNullClass          JagBytOpLoadNullClass;


struct _JagBytOpLoadNull {
	JagBytAbstractMnemonic parent;
};

struct _JagBytOpLoadNullClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_load_null_get_type();

JagBytOpLoadNull *jag_byt_op_load_null_new(int offset);

G_END_DECLS


#endif /* JAGBYTLOADNULL_H_ */
