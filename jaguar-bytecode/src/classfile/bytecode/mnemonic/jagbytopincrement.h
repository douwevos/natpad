/*
   File:    jagbytopincrement.h
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

#ifndef JAGBYTOPINCREMENT_H_
#define JAGBYTOPINCREMENT_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_INCREMENT              (jag_byt_op_increment_get_type())
#define JAG_BYT_OP_INCREMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_increment_get_type(), JagBytOpIncrement))
#define JAG_BYT_OP_INCREMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_INCREMENT, JagBytOpIncrementClass))
#define JAG_BYT_IS_OP_INCREMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_INCREMENT))
#define JAG_BYT_IS_OP_INCREMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_INCREMENT))
#define JAG_BYT_OP_INCREMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_INCREMENT, JagBytOpIncrementClass))

typedef struct _JagBytOpIncrement               JagBytOpIncrement;
typedef struct _JagBytOpIncrementPrivate        JagBytOpIncrementPrivate;
typedef struct _JagBytOpIncrementClass          JagBytOpIncrementClass;


struct _JagBytOpIncrement {
	JagBytAbstractMnemonic parent;
};

struct _JagBytOpIncrementClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_increment_get_type();

JagBytOpIncrement *jag_byt_op_increment_new(int offset, int index, int constant, gboolean is_wide);

int jag_byt_op_increment_get_frame_index(JagBytOpIncrement *op_increment);
int jag_byt_op_increment_get_increment_value(JagBytOpIncrement *op_increment);

G_END_DECLS

#endif /* JAGBYTOPINCREMENT_H_ */
