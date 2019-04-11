/*
   File:    jagbytattributeconstantvalue.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 1, 2012
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


#ifndef JAGBYTATTRIBUTECONSTANTVALUE_H_
#define JAGBYTATTRIBUTECONSTANTVALUE_H_

#include "jagbytattribute.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_ATTRIBUTE_CONSTANT_VALUE              (jag_byt_attribute_constant_value_get_type())
#define JAG_BYT_ATTRIBUTE_CONSTANT_VALUE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_attribute_constant_value_get_type(), JagBytAttributeConstantValue))
#define JAG_BYT_ATTRIBUTE_CONSTANT_VALUE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_ATTRIBUTE_CONSTANT_VALUE, JagBytAttributeConstantValueClass))
#define JAG_BYT_IS_ATTRIBUTE_CONSTANT_VALUE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_ATTRIBUTE_CONSTANT_VALUE))
#define JAG_BYT_IS_ATTRIBUTE_CONSTANT_VALUE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_ATTRIBUTE_CONSTANT_VALUE))
#define JAG_BYT_ATTRIBUTE_CONSTANT_VALUE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_ATTRIBUTE_CONSTANT_VALUE, JagBytAttributeConstantValueClass))
#define JAG_BYT_ATTRIBUTE_CONSTANT_VALUE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_ATTRIBUTE_CONSTANT_VALUE, JagBytAttributeConstantValuePrivate))

typedef struct _JagBytAttributeConstantValue               JagBytAttributeConstantValue;
typedef struct _JagBytAttributeConstantValuePrivate        JagBytAttributeConstantValuePrivate;
typedef struct _JagBytAttributeConstantValueClass          JagBytAttributeConstantValueClass;


struct _JagBytAttributeConstantValue {
	JagBytAttribute parent;
	JagBytAttributeConstantValuePrivate *priv;
};

struct _JagBytAttributeConstantValueClass {
	JagBytAttributeClass parent_class;
};

extern CatS jag_txt_attribute_constant_value;


GType jag_byt_attribute_constant_value_get_type();

JagBytAttributeConstantValue *jag_byt_attribute_constant_value_new(CatStringWo *e_attribute_data);

int jag_byt_attribute_constant_value_get_index(JagBytAttributeConstantValue *constant_value);

G_END_DECLS


#endif /* JAGBYTATTRIBUTECONSTANTVALUE_H_ */
