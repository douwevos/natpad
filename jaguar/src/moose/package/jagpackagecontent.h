/*
   File:    jagpackagecontent.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 20, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#ifndef JAGPACKAGECONTENT_H_
#define JAGPACKAGECONTENT_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_PACKAGE_CONTENT              (jag_package_content_get_type())
#define JAG_PACKAGE_CONTENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_package_content_get_type(), JagPackageContent))
#define JAG_PACKAGE_CONTENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PACKAGE_CONTENT, JagPackageContentClass))
#define JAG_IS_PACKAGE_CONTENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PACKAGE_CONTENT))
#define JAG_IS_PACKAGE_CONTENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PACKAGE_CONTENT))
#define JAG_PACKAGE_CONTENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PACKAGE_CONTENT, JagPackageContentClass))
#define JAG_PACKAGE_CONTENT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_PACKAGE_CONTENT, JagPackageContentPrivate))


typedef struct _JagPackageContent               JagPackageContent;
typedef struct _JagPackageContentPrivate        JagPackageContentPrivate;
typedef struct _JagPackageContentClass          JagPackageContentClass;


struct _JagPackageContent {
	GObject parent;
	JagPackageContentPrivate *priv;
};

struct _JagPackageContentClass {
	GObjectClass parent_class;
};


GType jag_package_content_get_type();

JagPackageContent *jag_package_content_new();

CatStringWo *jag_package_content_key();

G_END_DECLS

#endif /* JAGPACKAGECONTENT_H_ */
