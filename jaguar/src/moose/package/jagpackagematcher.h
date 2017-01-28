/*
   File:    jagpackagematcher.h
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

#ifndef JAGPACKAGEMATCHER_H_
#define JAGPACKAGEMATCHER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_PACKAGE_MATCHER              (jag_package_matcher_get_type())
#define JAG_PACKAGE_MATCHER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_package_matcher_get_type(), JagPackageMatcher))
#define JAG_PACKAGE_MATCHER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PACKAGE_MATCHER, JagPackageMatcherClass))
#define JAG_IS_PACKAGE_MATCHER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PACKAGE_MATCHER))
#define JAG_IS_PACKAGE_MATCHER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PACKAGE_MATCHER))
#define JAG_PACKAGE_MATCHER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PACKAGE_MATCHER, JagPackageMatcherClass))
#define JAG_PACKAGE_MATCHER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_PACKAGE_MATCHER, JagPackageMatcherPrivate))


typedef struct _JagPackageMatcher               JagPackageMatcher;
typedef struct _JagPackageMatcherPrivate        JagPackageMatcherPrivate;
typedef struct _JagPackageMatcherClass          JagPackageMatcherClass;


struct _JagPackageMatcher {
	GObject parent;
	JagPackageMatcherPrivate *priv;
};

struct _JagPackageMatcherClass {
	GObjectClass parent_class;
};


GType jag_package_matcher_get_type();

JagPackageMatcher *jag_package_matcher_new();

void jag_package_matcher_set_match_name(JagPackageMatcher *package_matcher, CatStringWo *a_newname);


G_END_DECLS

#endif /* JAGPACKAGEMATCHER_H_ */
