/*
   File:    jagbytoparraylength.h
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

#ifndef JAGBYTOPARRAYLENGTH_H_
#define JAGBYTOPARRAYLENGTH_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_ARRAY_LENGTH              (jag_byt_op_array_length_get_type())
#define JAG_BYT_OP_ARRAY_LENGTH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_array_length_get_type(), JagBytOpArrayLength))
#define JAG_BYT_OP_ARRAY_LENGTH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_ARRAY_LENGTH, JagBytOpArrayLengthClass))
#define JAG_BYT_IS_OP_ARRAY_LENGTH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_ARRAY_LENGTH))
#define JAG_BYT_IS_OP_ARRAY_LENGTH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_ARRAY_LENGTH))
#define JAG_BYT_OP_ARRAY_LENGTH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_ARRAY_LENGTH, JagBytOpArrayLengthClass))

typedef struct _JagBytOpArrayLength               JagBytOpArrayLength;
typedef struct _JagBytOpArrayLengthClass          JagBytOpArrayLengthClass;


struct _JagBytOpArrayLength {
	JagBytAbstractMnemonic parent;
};

struct _JagBytOpArrayLengthClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_array_length_get_type();

JagBytOpArrayLength *jag_byt_op_array_length_new(int offset);

G_END_DECLS

#endif /* JAGBYTOPARRAYLENGTH_H_ */
