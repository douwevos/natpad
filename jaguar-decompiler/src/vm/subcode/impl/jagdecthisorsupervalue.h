/*
   File:    jagdecthisorsupervalue.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 7, 2012
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


#ifndef JAGDECTHISORSUPERVALUE_H_
#define JAGDECTHISORSUPERVALUE_H_

#include "jagdecmethodframevalue.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_THIS_OR_SUPER_VALUE              (jag_dec_this_or_super_value_get_type())
#define JAG_DEC_THIS_OR_SUPER_VALUE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_this_or_super_value_get_type(), JagDecThisOrSuperValue))
#define JAG_DEC_THIS_OR_SUPER_VALUE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_THIS_OR_SUPER_VALUE, JagDecThisOrSuperValueClass))
#define JAG_DEC_IS_THIS_OR_SUPER_VALUE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_THIS_OR_SUPER_VALUE))
#define JAG_DEC_IS_THIS_OR_SUPER_VALUE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_THIS_OR_SUPER_VALUE))
#define JAG_DEC_THIS_OR_SUPER_VALUE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_THIS_OR_SUPER_VALUE, JagDecThisOrSuperValueClass))
#define JAG_DEC_THIS_OR_SUPER_VALUE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_THIS_OR_SUPER_VALUE, JagDecThisOrSuperValuePrivate))

typedef struct _JagDecThisOrSuperValue               JagDecThisOrSuperValue;
typedef struct _JagDecThisOrSuperValuePrivate        JagDecThisOrSuperValuePrivate;
typedef struct _JagDecThisOrSuperValueClass          JagDecThisOrSuperValueClass;


struct _JagDecThisOrSuperValue {
	JagDecMethodFrameValue parent;
	JagDecThisOrSuperValuePrivate *priv;
};

struct _JagDecThisOrSuperValueClass {
	JagDecMethodFrameValueClass parent_class;
};


GType jag_dec_this_or_super_value_get_type();

JagDecThisOrSuperValue *jag_dec_this_or_super_value_new(int frame_index, JagAstDeclarationType *this_declaration_type);

G_END_DECLS


#endif /* JAGDECTHISORSUPERVALUE_H_ */
