/*
   File:    jagbytopcompareint.h
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

#ifndef JAGBYTOPCOMPAREINT_H_
#define JAGBYTOPCOMPAREINT_H_

#include "../jagbytabstractmnemonic.h"
#include "../jagbytconditiontype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_COMPARE_INT              (jag_byt_op_compare_int_get_type())
#define JAG_BYT_OP_COMPARE_INT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_compare_int_get_type(), JagBytOpCompareInt))
#define JAG_BYT_OP_COMPARE_INT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_COMPARE_INT, JagBytOpCompareIntClass))
#define JAG_BYT_IS_OP_COMPARE_INT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_COMPARE_INT))
#define JAG_BYT_IS_OP_COMPARE_INT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_COMPARE_INT))
#define JAG_BYT_OP_COMPARE_INT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_COMPARE_INT, JagBytOpCompareIntClass))
#define JAG_BYT_OP_COMPARE_INT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_COMPARE_INT, JagBytOpCompareIntPrivate))

typedef struct _JagBytOpCompareInt               JagBytOpCompareInt;
typedef struct _JagBytOpCompareIntPrivate        JagBytOpCompareIntPrivate;
typedef struct _JagBytOpCompareIntClass          JagBytOpCompareIntClass;


struct _JagBytOpCompareInt {
	JagBytAbstractMnemonic parent;
	JagBytOpCompareIntPrivate *priv;
};

struct _JagBytOpCompareIntClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_compare_int_get_type();

JagBytOpCompareInt *jag_byt_op_compare_int_new(JagBytOperation operation, int offset, int branch_offset, JagBytConditionType condition_type, gboolean with_zero);


JagBytConditionType jag_byt_op_compare_int_get_condition_type(JagBytOpCompareInt *compare_int);

gboolean jag_byt_op_compare_int_is_with_zero(JagBytOpCompareInt *compare_int);

G_END_DECLS

#endif /* JAGBYTOPCOMPAREINT_H_ */
