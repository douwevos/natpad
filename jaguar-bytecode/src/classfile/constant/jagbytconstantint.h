/*
   File:    jagbytconstantint.h
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

#ifndef JAGBYTCONSTANTINT_H_
#define JAGBYTCONSTANTINT_H_

#define JAG_BYT_CONSTANT_INT_ID  3

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CONSTANT_INT              (jag_byt_constant_int_get_type())
#define JAG_BYT_CONSTANT_INT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_constant_int_get_type(), JagBytConstantInt))
#define JAG_BYT_CONSTANT_INT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CONSTANT_INT, JagBytConstantIntClass))
#define JAG_BYT_IS_CONSTANT_INT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CONSTANT_INT))
#define JAG_BYT_IS_CONSTANT_INT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CONSTANT_INT))
#define JAG_BYT_CONSTANT_INT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CONSTANT_INT, JagBytConstantIntClass))

typedef struct _JagBytConstantInt               JagBytConstantInt;
typedef struct _JagBytConstantIntPrivate        JagBytConstantIntPrivate;
typedef struct _JagBytConstantIntClass          JagBytConstantIntClass;


struct _JagBytConstantInt {
	GObject parent;
};

struct _JagBytConstantIntClass {
	GObjectClass parent_class;
};


GType jag_byt_constant_int_get_type();

JagBytConstantInt *jag_byt_constant_int_new(int32_t value);

int32_t jag_byt_constant_int_get_value(JagBytConstantInt *constant_int);

G_END_DECLS

#endif /* JAGBYTCONSTANTINT_H_ */
