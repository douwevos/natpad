/*
   File:    jagbytconstantstring.h
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

#ifndef JAGBYTCONSTANTSTRING_H_
#define JAGBYTCONSTANTSTRING_H_

#define JAG_BYT_CONSTANT_STRING_ID  8

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CONSTANT_STRING              (jag_byt_constant_string_get_type())
#define JAG_BYT_CONSTANT_STRING(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_constant_string_get_type(), JagBytConstantString))
#define JAG_BYT_CONSTANT_STRING_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CONSTANT_STRING, JagBytConstantStringClass))
#define JAG_BYT_IS_CONSTANT_STRING(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CONSTANT_STRING))
#define JAG_BYT_IS_CONSTANT_STRING_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CONSTANT_STRING))
#define JAG_BYT_CONSTANT_STRING_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CONSTANT_STRING, JagBytConstantStringClass))

typedef struct _JagBytConstantString               JagBytConstantString;
typedef struct _JagBytConstantStringPrivate        JagBytConstantStringPrivate;
typedef struct _JagBytConstantStringClass          JagBytConstantStringClass;


struct _JagBytConstantString {
	GObject parent;
};

struct _JagBytConstantStringClass {
	GObjectClass parent_class;
};


GType jag_byt_constant_string_get_type();

JagBytConstantString *jag_byt_constant_string_new(int string_index);

CatStringWo *jag_byt_constant_string_get_value(JagBytConstantString *constant_string);

G_END_DECLS

#endif /* JAGBYTCONSTANTSTRING_H_ */
