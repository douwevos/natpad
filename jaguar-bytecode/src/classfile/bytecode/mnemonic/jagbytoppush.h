/*
   File:    jagbytoppush.h
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

#ifndef JAGBYTOPPUSH_H_
#define JAGBYTOPPUSH_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_PUSH              (jag_byt_op_push_get_type())
#define JAG_BYT_OP_PUSH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_push_get_type(), JagBytOpPush))
#define JAG_BYT_OP_PUSH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_PUSH, JagBytOpPushClass))
#define JAG_BYT_IS_OP_PUSH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_PUSH))
#define JAG_BYT_IS_OP_PUSH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_PUSH))
#define JAG_BYT_OP_PUSH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_PUSH, JagBytOpPushClass))

typedef struct _JagBytOpPush               JagBytOpPush;
typedef struct _JagBytOpPushPrivate        JagBytOpPushPrivate;
typedef struct _JagBytOpPushClass          JagBytOpPushClass;


struct _JagBytOpPush {
	JagBytAbstractMnemonic parent;
};

struct _JagBytOpPushClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_push_get_type();

JagBytOpPush *jag_byt_op_push_new_byte(int offset, int value);
JagBytOpPush *jag_byt_op_push_new_short(int offset, int value);

int jag_byt_op_push_get_value(JagBytOpPush *op_push);

G_END_DECLS

#endif /* JAGBYTOPPUSH_H_ */
