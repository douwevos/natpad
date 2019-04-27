/*
   File:    jagvipjarmap.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 27, 2013
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

#ifndef JAGVIPJARMAP_H_
#define JAGVIPJARMAP_H_

#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_VIP_JAR_MAP              (jag_vip_jar_map_get_type())
#define JAG_VIP_JAR_MAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_vip_jar_map_get_type(), JagVipJarMap))
#define JAG_VIP_JAR_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_VIP_JAR_MAP, JagVipJarMapClass))
#define JAG_IS_VIP_JAR_MAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_VIP_JAR_MAP))
#define JAG_IS_VIP_JAR_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_VIP_JAR_MAP))
#define JAG_VIP_JAR_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_VIP_JAR_MAP, JagVipJarMapClass))

typedef struct _JagVipJarMap               JagVipJarMap;
typedef struct _JagVipJarMapPrivate        JagVipJarMapPrivate;
typedef struct _JagVipJarMapClass          JagVipJarMapClass;

struct _JagVipJarMap {
	GObject parent;
};

struct _JagVipJarMapClass {
	GObjectClass parent_class;
};

GType jag_vip_jar_map_get_type();

JagVipJarMap *jag_vip_jar_map_new(VipIFile *mainFile, CatStringWo *a_name, CatArrayWo /* <VipIResource *> */ *a_vipchildren);

G_END_DECLS

#endif /* JAGVIPJARMAP_H_ */
