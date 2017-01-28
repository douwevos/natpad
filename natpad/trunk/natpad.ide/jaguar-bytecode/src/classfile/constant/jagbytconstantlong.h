/*
   File:    jagbytconstantlong.h
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


#ifndef JAGBYTCONSTANTLONG_H_
#define JAGBYTCONSTANTLONG_H_

#define JAG_BYT_CONSTANT_LONG_ID  5

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CONSTANT_LONG              (jag_byt_constant_long_get_type())
#define JAG_BYT_CONSTANT_LONG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_constant_long_get_type(), JagBytConstantLong))
#define JAG_BYT_CONSTANT_LONG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CONSTANT_LONG, JagBytConstantLongClass))
#define JAG_BYT_IS_CONSTANT_LONG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CONSTANT_LONG))
#define JAG_BYT_IS_CONSTANT_LONG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CONSTANT_LONG))
#define JAG_BYT_CONSTANT_LONG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CONSTANT_LONG, JagBytConstantLongClass))
#define JAG_BYT_CONSTANT_LONG_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_CONSTANT_LONG, JagBytConstantLongPrivate))

typedef struct _JagBytConstantLong               JagBytConstantLong;
typedef struct _JagBytConstantLongPrivate        JagBytConstantLongPrivate;
typedef struct _JagBytConstantLongClass          JagBytConstantLongClass;


struct _JagBytConstantLong {
	GObject parent;
	JagBytConstantLongPrivate *priv;
};

struct _JagBytConstantLongClass {
	GObjectClass parent_class;
};


GType jag_byt_constant_long_get_type();

JagBytConstantLong *jag_byt_constant_long_new(int64_t value);

int64_t jag_byt_constant_long_get_value(JagBytConstantLong *constant_long);


G_END_DECLS


#endif /* JAGBYTCONSTANTLONG_H_ */
