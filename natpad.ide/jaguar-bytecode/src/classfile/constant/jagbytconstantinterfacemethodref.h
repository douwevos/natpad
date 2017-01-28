/*
   File:    jagbytconstantinterfacemethodref.h
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


#ifndef JAGBYTCONSTANTINTERFACEMETHODREF_H_
#define JAGBYTCONSTANTINTERFACEMETHODREF_H_

#define JAG_BYT_CONSTANT_INTERFACE_METHODREF_ID  11

#include "../../classfile/jagbytmethodheader.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CONSTANT_INTERFACE_METHODREF              (jag_byt_constant_interface_methodref_get_type())
#define JAG_BYT_CONSTANT_INTERFACE_METHODREF(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_constant_interface_methodref_get_type(), JagBytConstantInterfaceMethodref))
#define JAG_BYT_CONSTANT_INTERFACE_METHODREF_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CONSTANT_INTERFACE_METHODREF, JagBytConstantInterfaceMethodrefClass))
#define JAG_BYT_IS_CONSTANT_INTERFACE_METHODREF(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CONSTANT_INTERFACE_METHODREF))
#define JAG_BYT_IS_CONSTANT_INTERFACE_METHODREF_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CONSTANT_INTERFACE_METHODREF))
#define JAG_BYT_CONSTANT_INTERFACE_METHODREF_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CONSTANT_INTERFACE_METHODREF, JagBytConstantInterfaceMethodrefClass))
#define JAG_BYT_CONSTANT_INTERFACE_METHODREF_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_CONSTANT_INTERFACE_METHODREF, JagBytConstantInterfaceMethodrefPrivate))

typedef struct _JagBytConstantInterfaceMethodref               JagBytConstantInterfaceMethodref;
typedef struct _JagBytConstantInterfaceMethodrefPrivate        JagBytConstantInterfaceMethodrefPrivate;
typedef struct _JagBytConstantInterfaceMethodrefClass          JagBytConstantInterfaceMethodrefClass;


struct _JagBytConstantInterfaceMethodref {
	GObject parent;
	JagBytConstantInterfaceMethodrefPrivate *priv;
};

struct _JagBytConstantInterfaceMethodrefClass {
	GObjectClass parent_class;
};


GType jag_byt_constant_interface_methodref_get_type();

JagBytConstantInterfaceMethodref *jag_byt_constant_interface_methodref_new(int class_index, int name_and_type_index);

G_END_DECLS


#endif /* JAGBYTCONSTANTINTERFACEMETHODREF_H_ */
