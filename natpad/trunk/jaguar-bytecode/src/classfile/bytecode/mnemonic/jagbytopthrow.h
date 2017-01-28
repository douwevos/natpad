/*
   File:    jagbytopthrow.h
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


#ifndef JAGBYTOPTHROW_H_
#define JAGBYTOPTHROW_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_THROW              (jag_byt_op_throw_get_type())
#define JAG_BYT_OP_THROW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_throw_get_type(), JagBytOpThrow))
#define JAG_BYT_OP_THROW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_THROW, JagBytOpThrowClass))
#define JAG_BYT_IS_OP_THROW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_THROW))
#define JAG_BYT_IS_OP_THROW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_THROW))
#define JAG_BYT_OP_THROW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_THROW, JagBytOpThrowClass))

typedef struct _JagBytOpThrow               JagBytOpThrow;
typedef struct _JagBytOpThrowClass          JagBytOpThrowClass;


struct _JagBytOpThrow {
	JagBytAbstractMnemonic parent;
};

struct _JagBytOpThrowClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_throw_get_type();

JagBytOpThrow *jag_byt_op_throw_new(int offset);

G_END_DECLS


#endif /* JAGBYTOPTHROW_H_ */
