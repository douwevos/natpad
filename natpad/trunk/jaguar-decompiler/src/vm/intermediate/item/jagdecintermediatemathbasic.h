/*
   File:    jagdecintermediatemathbasic.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 15, 2012
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


#ifndef JAGDECINTERMEDIATEMATHBASIC_H_
#define JAGDECINTERMEDIATEMATHBASIC_H_

#include "../jagdecabstractintermediateitem.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_MATH_BASIC              (jag_dec_intermediate_math_basic_get_type())
#define JAG_DEC_INTERMEDIATE_MATH_BASIC(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_math_basic_get_type(), JagDecIntermediateMathBasic))
#define JAG_DEC_INTERMEDIATE_MATH_BASIC_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_MATH_BASIC, JagDecIntermediateMathBasicClass))
#define JAG_DEC_IS_INTERMEDIATE_MATH_BASIC(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_MATH_BASIC))
#define JAG_DEC_IS_INTERMEDIATE_MATH_BASIC_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_MATH_BASIC))
#define JAG_DEC_INTERMEDIATE_MATH_BASIC_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_MATH_BASIC, JagDecIntermediateMathBasicClass))
#define JAG_DEC_INTERMEDIATE_MATH_BASIC_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_MATH_BASIC, JagDecIntermediateMathBasicPrivate))

typedef struct _JagDecIntermediateMathBasic               JagDecIntermediateMathBasic;
typedef struct _JagDecIntermediateMathBasicPrivate        JagDecIntermediateMathBasicPrivate;
typedef struct _JagDecIntermediateMathBasicClass          JagDecIntermediateMathBasicClass;


struct _JagDecIntermediateMathBasic {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateMathBasicPrivate *priv;
};

struct _JagDecIntermediateMathBasicClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_math_basic_get_type();

JagDecIntermediateMathBasic *jag_dec_intermediate_math_basic_new(int mnemonic_index, JagBytMathOperator math_operator);


JagBytMathOperator jag_dec_intermediate_math_basic_get_operator(JagDecIntermediateMathBasic *math_basic);

G_END_DECLS


#endif /* JAGDECINTERMEDIATEMATHBASIC_H_ */
