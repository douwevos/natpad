/*
   File:    jagnamemap.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 20, 2012
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


#ifndef JAGNAMEMAP_H_
#define JAGNAMEMAP_H_

#include <caterpillar.h>
#include "jagnamepackage.h"

G_BEGIN_DECLS

#define JAG_TYPE_NAME_MAP              (jag_name_map_get_type())
#define JAG_NAME_MAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_name_map_get_type(), JagNameMap))
#define JAG_NAME_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_NAME_MAP, JagNameMapClass))
#define JAG_IS_NAME_MAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_NAME_MAP))
#define JAG_IS_NAME_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_NAME_MAP))
#define JAG_NAME_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_NAME_MAP, JagNameMapClass))
#define JAG_NAME_MAP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_NAME_MAP, JagNameMapPrivate))

typedef struct _JagNameMap               JagNameMap;
typedef struct _JagNameMapPrivate        JagNameMapPrivate;
typedef struct _JagNameMapClass          JagNameMapClass;


struct _JagNameMap {
	GObject parent;
	JagNameMapPrivate *priv;
};

struct _JagNameMapClass {
	GObjectClass parent_class;
};


GType jag_name_map_get_type();

JagNameMap *jag_name_map_new();


JagNamePackage *jag_name_map_get_or_create_package_ref(JagNameMap *name_map, CatStringWo *a_package_name);

CatArrayWo *jag_name_map_enlist_packages(JagNameMap *name_map);

G_END_DECLS

#endif /* JAGNAMEMAP_H_ */
