/*
   File:    jagnametype.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 21, 2012
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


#ifndef JAGNAMETYPE_H_
#define JAGNAMETYPE_H_


#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_NAME_TYPE              (jag_name_type_get_type())
#define JAG_NAME_TYPE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_name_type_get_type(), JagNameType))
#define JAG_NAME_TYPE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_NAME_TYPE, JagNameTypeClass))
#define JAG_IS_NAME_TYPE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_NAME_TYPE))
#define JAG_IS_NAME_TYPE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_NAME_TYPE))
#define JAG_NAME_TYPE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_NAME_TYPE, JagNameTypeClass))
#define JAG_NAME_TYPE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_NAME_TYPE, JagNameTypePrivate))

typedef struct _JagNameType               JagNameType;
typedef struct _JagNameTypePrivate        JagNameTypePrivate;
typedef struct _JagNameTypeClass          JagNameTypeClass;


struct _JagNameType {
	GObject parent;
	JagNameTypePrivate *priv;
};

struct _JagNameTypeClass {
	GObjectClass parent_class;
};


GType jag_name_type_get_type();

JagNameType *jag_name_type_new(CatStringWo *a_package_name, CatStringWo *a_type_name);

CatStringWo *jag_name_type_get_name(JagNameType *name_type);

void jag_name_type_set_implementation(JagNameType *name_type, GObject *clazz);

GObject *jag_name_type_get_implementation_ref(JagNameType *name_type, int *version);


G_END_DECLS


#endif /* JAGNAMETYPE_H_ */
