/*
   File:    jagvipjarroot.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Feb 23, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef JAGVIPJARROOT_H_
#define JAGVIPJARROOT_H_

#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_VIP_JAR_ROOT              (jag_vip_jar_root_get_type())
#define JAG_VIP_JAR_ROOT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_vip_jar_root_get_type(), JagVipJarRoot))
#define JAG_VIP_JAR_ROOT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_VIP_JAR_ROOT, JagVipJarRootClass))
#define JAG_IS_VIP_JAR_ROOT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_VIP_JAR_ROOT))
#define JAG_IS_VIP_JAR_ROOT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_VIP_JAR_ROOT))
#define JAG_VIP_JAR_ROOT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_VIP_JAR_ROOT, JagVipJarRootClass))
#define JAG_VIP_JAR_ROOT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_VIP_JAR_ROOT, JagVipJarRootPrivate))


typedef struct _JagVipJarRoot               JagVipJarRoot;
typedef struct _JagVipJarRootPrivate        JagVipJarRootPrivate;
typedef struct _JagVipJarRootClass          JagVipJarRootClass;


struct _JagVipJarRoot {
	GObject parent;
	JagVipJarRootPrivate *priv;
};

struct _JagVipJarRootClass {
	GObjectClass parent_class;
};


GType jag_vip_jar_root_get_type();

JagVipJarRoot *jag_vip_jar_root_new();

G_END_DECLS

#endif /* JAGVIPJARROOT_H_ */
