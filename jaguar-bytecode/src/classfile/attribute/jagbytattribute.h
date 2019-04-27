/*
   File:    jagbytattribute.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Mar 6, 2012
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

#ifndef JAGBYTATTRIBUTE_H_
#define JAGBYTATTRIBUTE_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_ATTRIBUTE              (jag_byt_attribute_get_type())
#define JAG_BYT_ATTRIBUTE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_attribute_get_type(), JagBytAttribute))
#define JAG_BYT_ATTRIBUTE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_ATTRIBUTE, JagBytAttributeClass))
#define JAG_BYT_IS_ATTRIBUTE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_ATTRIBUTE))
#define JAG_BYT_IS_ATTRIBUTE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_ATTRIBUTE))
#define JAG_BYT_ATTRIBUTE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_ATTRIBUTE, JagBytAttributeClass))

typedef struct _JagBytAttribute               JagBytAttribute;
typedef struct _JagBytAttributeClass          JagBytAttributeClass;


struct _JagBytAttribute {
	GObject parent;
};

struct _JagBytAttributeClass {
	GObjectClass parent_class;
};


GType jag_byt_attribute_get_type();

void jag_byt_attribute_construct(JagBytAttribute *attribute);

G_END_DECLS

#endif /* JAGBYTATTRIBUTE_H_ */
