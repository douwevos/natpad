/*
   File:    jagbytopcheckcast.h
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


#ifndef JAGBYTOPCHECKCAST_H_
#define JAGBYTOPCHECKCAST_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_CHECK_CAST              (jag_byt_op_check_cast_get_type())
#define JAG_BYT_OP_CHECK_CAST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_check_cast_get_type(), JagBytOpCheckCast))
#define JAG_BYT_OP_CHECK_CAST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_CHECK_CAST, JagBytOpCheckCastClass))
#define JAG_BYT_IS_OP_CHECK_CAST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_CHECK_CAST))
#define JAG_BYT_IS_OP_CHECK_CAST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_CHECK_CAST))
#define JAG_BYT_OP_CHECK_CAST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_CHECK_CAST, JagBytOpCheckCastClass))
#define JAG_BYT_OP_CHECK_CAST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_CHECK_CAST, JagBytOpCheckCastPrivate))

typedef struct _JagBytOpCheckCast               JagBytOpCheckCast;
typedef struct _JagBytOpCheckCastPrivate        JagBytOpCheckCastPrivate;
typedef struct _JagBytOpCheckCastClass          JagBytOpCheckCastClass;


struct _JagBytOpCheckCast {
	JagBytAbstractMnemonic parent;
	JagBytOpCheckCastPrivate *priv;
};

struct _JagBytOpCheckCastClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_check_cast_get_type();

JagBytOpCheckCast *jag_byt_op_check_cast_new(JagBytOperation operation, int offset, int class_pool_index);

int jag_byt_op_check_cast_get_class_info_pool_index(JagBytOpCheckCast *op_check_cast);

G_END_DECLS


#endif /* JAGBYTOPCHECKCAST_H_ */
