/*
   File:    jagbytconstantclazz.h
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

#ifndef JAGBYTCONSTANTCLAZZ_H_
#define JAGBYTCONSTANTCLAZZ_H_

#define JAG_BYT_CONSTANT_CLAZZ_ID  7

#include "../../ast/jagastdeclarationtype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CONSTANT_CLAZZ              (jag_byt_constant_clazz_get_type())
#define JAG_BYT_CONSTANT_CLAZZ(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_constant_clazz_get_type(), JagBytConstantClazz))
#define JAG_BYT_CONSTANT_CLAZZ_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CONSTANT_CLAZZ, JagBytConstantClazzClass))
#define JAG_BYT_IS_CONSTANT_CLAZZ(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CONSTANT_CLAZZ))
#define JAG_BYT_IS_CONSTANT_CLAZZ_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CONSTANT_CLAZZ))
#define JAG_BYT_CONSTANT_CLAZZ_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CONSTANT_CLAZZ, JagBytConstantClazzClass))

typedef struct _JagBytConstantClazz               JagBytConstantClazz;
typedef struct _JagBytConstantClazzPrivate        JagBytConstantClazzPrivate;
typedef struct _JagBytConstantClazzClass          JagBytConstantClazzClass;


struct _JagBytConstantClazz {
	GObject parent;
};

struct _JagBytConstantClazzClass {
	GObjectClass parent_class;
};


GType jag_byt_constant_clazz_get_type();

JagBytConstantClazz *jag_byt_constant_clazz_new(int name_index);

int jag_byt_constant_clazz_get_name_index(JagBytConstantClazz *constant_clazz);

JagBytName *jag_byt_constant_clazz_get_refrence_type_name(JagBytConstantClazz *constant_clazz);

JagAstDeclarationType *jag_byt_constant_clazz_get_parsed_descriptor(JagBytConstantClazz *constant_clazz);

G_END_DECLS

#endif /* JAGBYTCONSTANTCLAZZ_H_ */
