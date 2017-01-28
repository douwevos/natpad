/*
   File:    jagdecmethodframevalue.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 6, 2012
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


#ifndef JAGDECMETHODFRAMEVALUE_H_
#define JAGDECMETHODFRAMEVALUE_H_

#include "../../../type/jagdecvaluetypedefinitionlist.h"
#include "../../../type/jagdecvaluetypedefinition.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_METHOD_FRAME_VALUE              (jag_dec_method_frame_value_get_type())
#define JAG_DEC_METHOD_FRAME_VALUE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_method_frame_value_get_type(), JagDecMethodFrameValue))
#define JAG_DEC_METHOD_FRAME_VALUE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_METHOD_FRAME_VALUE, JagDecMethodFrameValueClass))
#define JAG_DEC_IS_METHOD_FRAME_VALUE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_METHOD_FRAME_VALUE))
#define JAG_DEC_IS_METHOD_FRAME_VALUE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_METHOD_FRAME_VALUE))
#define JAG_DEC_METHOD_FRAME_VALUE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_METHOD_FRAME_VALUE, JagDecMethodFrameValueClass))
#define JAG_DEC_METHOD_FRAME_VALUE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_METHOD_FRAME_VALUE, JagDecMethodFrameValuePrivate))

typedef struct _JagDecMethodFrameValue               JagDecMethodFrameValue;
typedef struct _JagDecMethodFrameValuePrivate        JagDecMethodFrameValuePrivate;
typedef struct _JagDecMethodFrameValueClass          JagDecMethodFrameValueClass;


struct _JagDecMethodFrameValue {
	JagDecValueTypeDefinitionList parent;
	JagDecMethodFrameValuePrivate *priv;
};

struct _JagDecMethodFrameValueClass {
	JagDecValueTypeDefinitionListClass parent_class;

	JagAstIExpression *(*getExpressionRef)(JagDecMethodFrameValue *frame_value, JagAstDeclarationType *type_descr);
};


GType jag_dec_method_frame_value_get_type();


void jag_dec_method_frame_value_construct(JagDecMethodFrameValue *frame_value, int frame_index, JagAstIdentifier *identifier);

JagDecMethodFrameValue *jag_dec_method_frame_value_new(int frame_index, JagAstIdentifier *identifier);

JagAstIExpression *jag_dec_method_frame_value_get_expression_ref(JagDecMethodFrameValue *frame_value, JagAstDeclarationType *type_descr);

gboolean jag_dec_method_frame_value_set_name(JagDecMethodFrameValue *frame_value, CatStringWo *a_name);

int jag_dec_method_frame_value_hash(JagDecMethodFrameValue *frame_value);

int jag_dec_method_frame_value_get_frame_index(JagDecMethodFrameValue *frame_value);


G_END_DECLS


#endif /* JAGDECMETHODFRAMEVALUE_H_ */