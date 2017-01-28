/*
   File:    jagdecintermediateconstant.h
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


#ifndef JAGDECINTERMEDIATECONSTANT_H_
#define JAGDECINTERMEDIATECONSTANT_H_

#include "../jagdecabstractintermediateitem.h"
#include "../../subcode/impl/jagdecsubcodeconstantliteral.h"
#include "../../../type/jagdecvaluetypedefinitionlist.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_CONSTANT              (jag_dec_intermediate_constant_get_type())
#define JAG_DEC_INTERMEDIATE_CONSTANT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_constant_get_type(), JagDecIntermediateConstant))
#define JAG_DEC_INTERMEDIATE_CONSTANT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_CONSTANT, JagDecIntermediateConstantClass))
#define JAG_DEC_IS_INTERMEDIATE_CONSTANT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_CONSTANT))
#define JAG_DEC_IS_INTERMEDIATE_CONSTANT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_CONSTANT))
#define JAG_DEC_INTERMEDIATE_CONSTANT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_CONSTANT, JagDecIntermediateConstantClass))
#define JAG_DEC_INTERMEDIATE_CONSTANT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_CONSTANT, JagDecIntermediateConstantPrivate))

typedef struct _JagDecIntermediateConstant               JagDecIntermediateConstant;
typedef struct _JagDecIntermediateConstantPrivate        JagDecIntermediateConstantPrivate;
typedef struct _JagDecIntermediateConstantClass          JagDecIntermediateConstantClass;


struct _JagDecIntermediateConstant {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateConstantPrivate *priv;
};

struct _JagDecIntermediateConstantClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_constant_get_type();

JagDecIntermediateConstant *jag_dec_intermediate_constant_new(int mnemonic_index, JagDecValueTypeDefinitionList *type_set, JagAstIExpression *constant_expression);

JagAstIExpression *jag_dec_intermediate_constant_get_value_expression(JagDecIntermediateConstant *im_constant);

G_END_DECLS


#endif /* JAGDECINTERMEDIATECONSTANT_H_ */
