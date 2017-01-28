/*
   File:    jagbytconstantfloat.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Feb 1, 2012
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


#ifndef JAGBYTCONSTANTFLOAT_H_
#define JAGBYTCONSTANTFLOAT_H_

#define JAG_BYT_CONSTANT_FLOAT_ID  4


#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CONSTANT_FLOAT              (jag_byt_constant_float_get_type())
#define JAG_BYT_CONSTANT_FLOAT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_constant_float_get_type(), JagBytConstantFloat))
#define JAG_BYT_CONSTANT_FLOAT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CONSTANT_FLOAT, JagBytConstantFloatClass))
#define JAG_BYT_IS_CONSTANT_FLOAT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CONSTANT_FLOAT))
#define JAG_BYT_IS_CONSTANT_FLOAT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CONSTANT_FLOAT))
#define JAG_BYT_CONSTANT_FLOAT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CONSTANT_FLOAT, JagBytConstantFloatClass))
#define JAG_BYT_CONSTANT_FLOAT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_CONSTANT_FLOAT, JagBytConstantFloatPrivate))

typedef struct _JagBytConstantFloat               JagBytConstantFloat;
typedef struct _JagBytConstantFloatPrivate        JagBytConstantFloatPrivate;
typedef struct _JagBytConstantFloatClass          JagBytConstantFloatClass;


struct _JagBytConstantFloat {
	GObject parent;
	JagBytConstantFloatPrivate *priv;
};

struct _JagBytConstantFloatClass {
	GObjectClass parent_class;
};


GType jag_byt_constant_float_get_type();

JagBytConstantFloat *jag_byt_constant_float_new(int32_t value);

float jag_byt_constant_float_get_value(JagBytConstantFloat *constant_float);

G_END_DECLS


#endif /* JAGBYTCONSTANTFLOAT_H_ */
