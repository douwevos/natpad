/*
   File:    jagnameresource.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 12, 2012
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


#ifndef JAGNAMERESOURCE_H_
#define JAGNAMERESOURCE_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_NAME_RESOURCE              (jag_name_resource_get_type())
#define JAG_NAME_RESOURCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_name_resource_get_type(), JagNameResource))
#define JAG_NAME_RESOURCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_NAME_RESOURCE, JagNameResourceClass))
#define JAG_IS_NAME_RESOURCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_NAME_RESOURCE))
#define JAG_IS_NAME_RESOURCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_NAME_RESOURCE))
#define JAG_NAME_RESOURCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_NAME_RESOURCE, JagNameResourceClass))
#define JAG_NAME_RESOURCE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_NAME_RESOURCE, JagNameResourcePrivate))

typedef struct _JagNameResource               JagNameResource;
typedef struct _JagNameResourcePrivate        JagNameResourcePrivate;
typedef struct _JagNameResourceClass          JagNameResourceClass;


struct _JagNameResource {
	GObject parent;
	JagNameResourcePrivate *priv;
};

struct _JagNameResourceClass {
	GObjectClass parent_class;
};


GType jag_name_resource_get_type();

JagNameResource *jag_name_resource_new();

G_END_DECLS


#endif /* JAGNAMERESOURCE_H_ */