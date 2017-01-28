/*
   File:    jagnamepackage.h
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


#ifndef JAGNAMEPACKAGE_H_
#define JAGNAMEPACKAGE_H_

#include "jagnametype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_NAME_PACKAGE              (jag_name_package_get_type())
#define JAG_NAME_PACKAGE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_name_package_get_type(), JagNamePackage))
#define JAG_NAME_PACKAGE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_NAME_PACKAGE, JagNamePackageClass))
#define JAG_IS_NAME_PACKAGE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_NAME_PACKAGE))
#define JAG_IS_NAME_PACKAGE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_NAME_PACKAGE))
#define JAG_NAME_PACKAGE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_NAME_PACKAGE, JagNamePackageClass))
#define JAG_NAME_PACKAGE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_NAME_PACKAGE, JagNamePackagePrivate))

typedef struct _JagNamePackage               JagNamePackage;
typedef struct _JagNamePackagePrivate        JagNamePackagePrivate;
typedef struct _JagNamePackageClass          JagNamePackageClass;


struct _JagNamePackage {
	GObject parent;
	JagNamePackagePrivate *priv;
};

struct _JagNamePackageClass {
	GObjectClass parent_class;
};


GType jag_name_package_get_type();

JagNamePackage *jag_name_package_new(CatStringWo *a_package_name);

CatStringWo *jag_name_package_get_name(JagNamePackage *name_package);


JagNameType *jag_name_package_get_or_create_type_ref(JagNamePackage *name_package, CatStringWo *a_type_name);

G_END_DECLS


#endif /* JAGNAMEPACKAGE_H_ */
