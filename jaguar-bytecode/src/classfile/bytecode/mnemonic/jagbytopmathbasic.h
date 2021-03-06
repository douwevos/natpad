/*
   File:    jagbytopmathbasic.h
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

#ifndef JAGBYTOPMATHBASIC_H_
#define JAGBYTOPMATHBASIC_H_

#include "../jagbytabstractmnemonic.h"
#include "../jagbyttype.h"
#include "../jagbytmathoperator.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_MATH_BASIC              (jag_byt_op_math_basic_get_type())
#define JAG_BYT_OP_MATH_BASIC(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_math_basic_get_type(), JagBytOpMathBasic))
#define JAG_BYT_OP_MATH_BASIC_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_MATH_BASIC, JagBytOpMathBasicClass))
#define JAG_BYT_IS_OP_MATH_BASIC(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_MATH_BASIC))
#define JAG_BYT_IS_OP_MATH_BASIC_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_MATH_BASIC))
#define JAG_BYT_OP_MATH_BASIC_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_MATH_BASIC, JagBytOpMathBasicClass))

typedef struct _JagBytOpMathBasic               JagBytOpMathBasic;
typedef struct _JagBytOpMathBasicPrivate        JagBytOpMathBasicPrivate;
typedef struct _JagBytOpMathBasicClass          JagBytOpMathBasicClass;


struct _JagBytOpMathBasic {
	JagBytAbstractMnemonic parent;
};

struct _JagBytOpMathBasicClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_math_basic_get_type();

JagBytOpMathBasic *jag_byt_op_math_basic_new(JagBytOperation operation, int offset, JagBytType infix_type, JagBytMathOperator math_operator);

JagBytMathOperator jag_byt_op_math_basic_get_math_operator(JagBytOpMathBasic *op_math_logic);

G_END_DECLS

#endif /* JAGBYTOPMATHBASIC_H_ */
