/*
   File:    jagdecsubcodeconstantliteral.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 9, 2012
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


#ifndef JAGDECSUBCODECONSTANTLITERAL_H_
#define JAGDECSUBCODECONSTANTLITERAL_H_

#include "../jagdecabstractsubcode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_CONSTANT_LITERAL              (jag_dec_sub_code_constant_literal_get_type())
#define JAG_DEC_SUB_CODE_CONSTANT_LITERAL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_constant_literal_get_type(), JagDecSubCodeConstantLiteral))
#define JAG_DEC_SUB_CODE_CONSTANT_LITERAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_CONSTANT_LITERAL, JagDecSubCodeConstantLiteralClass))
#define JAG_DEC_IS_SUB_CODE_CONSTANT_LITERAL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_CONSTANT_LITERAL))
#define JAG_DEC_IS_SUB_CODE_CONSTANT_LITERAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_CONSTANT_LITERAL))
#define JAG_DEC_SUB_CODE_CONSTANT_LITERAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_CONSTANT_LITERAL, JagDecSubCodeConstantLiteralClass))
#define JAG_DEC_SUB_CODE_CONSTANT_LITERAL_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_CONSTANT_LITERAL, JagDecSubCodeConstantLiteralPrivate))

typedef struct _JagDecSubCodeConstantLiteral               JagDecSubCodeConstantLiteral;
typedef struct _JagDecSubCodeConstantLiteralPrivate        JagDecSubCodeConstantLiteralPrivate;
typedef struct _JagDecSubCodeConstantLiteralClass          JagDecSubCodeConstantLiteralClass;


struct _JagDecSubCodeConstantLiteral {
	JagDecAbstractSubCode parent;
	JagDecSubCodeConstantLiteralPrivate *priv;
};

struct _JagDecSubCodeConstantLiteralClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_constant_literal_get_type();

JagDecSubCodeConstantLiteral *jag_dec_sub_code_constant_literal_new(int mnemonic_index, JagAstIExpression *constant_expression);

G_END_DECLS


#endif /* JAGDECSUBCODECONSTANTLITERAL_H_ */
