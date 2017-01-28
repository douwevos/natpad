/*
   File:    jagbytopgoto.h
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


#ifndef JAGBYTOPGOTO_H_
#define JAGBYTOPGOTO_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_GOTO              (jag_byt_op_goto_get_type())
#define JAG_BYT_OP_GOTO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_goto_get_type(), JagBytOpGoto))
#define JAG_BYT_OP_GOTO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_GOTO, JagBytOpGotoClass))
#define JAG_BYT_IS_OP_GOTO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_GOTO))
#define JAG_BYT_IS_OP_GOTO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_GOTO))
#define JAG_BYT_OP_GOTO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_GOTO, JagBytOpGotoClass))
#define JAG_BYT_OP_GOTO_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_GOTO, JagBytOpGotoPrivate))

typedef struct _JagBytOpGoto               JagBytOpGoto;
typedef struct _JagBytOpGotoPrivate        JagBytOpGotoPrivate;
typedef struct _JagBytOpGotoClass          JagBytOpGotoClass;


struct _JagBytOpGoto {
	JagBytAbstractMnemonic parent;
	JagBytOpGotoPrivate *priv;
};

struct _JagBytOpGotoClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_goto_get_type();

JagBytOpGoto *jag_byt_op_goto_new(JagBytOperation operation, int offset, int branch_offset);

G_END_DECLS


#endif /* JAGBYTOPGOTO_H_ */
