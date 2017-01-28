/*
   File:    jgijarrefreshviper.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 17, 2014
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

#ifndef JGIJARREFRESHVIPER_H_
#define JGIJARREFRESHVIPER_H_

#include "jgijarentry.h"
#include <caterpillar.h>
#include <worm.h>
#include <viper.h>

G_BEGIN_DECLS

#define JGI_TYPE_JAR_REFRESH_VIPER              (jgi_jar_refresh_viper_get_type())
#define JGI_JAR_REFRESH_VIPER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jgi_jar_refresh_viper_get_type(), JgiJarRefreshViper))
#define JGI_JAR_REFRESH_VIPER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JGI_TYPE_JAR_REFRESH_VIPER, JgiJarRefreshViperClass))
#define JGI_IS_JAR_REFRESH_VIPER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JGI_TYPE_JAR_REFRESH_VIPER))
#define JGI_IS_JAR_REFRESH_VIPER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JGI_TYPE_JAR_REFRESH_VIPER))
#define JGI_JAR_REFRESH_VIPER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JGI_TYPE_JAR_REFRESH_VIPER, JgiJarRefreshViperClass))
#define JGI_JAR_REFRESH_VIPER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JGI_TYPE_JAR_REFRESH_VIPER, JgiJarRefreshViperPrivate))


typedef struct _JgiJarRefreshViper               JgiJarRefreshViper;
typedef struct _JgiJarRefreshViperPrivate        JgiJarRefreshViperPrivate;
typedef struct _JgiJarRefreshViperClass          JgiJarRefreshViperClass;


struct _JgiJarRefreshViper {
	WorRequest parent;
	JgiJarRefreshViperPrivate *priv;
};

struct _JgiJarRefreshViperClass {
	WorRequestClass parent_class;
};


GType jgi_jar_refresh_viper_get_type();

JgiJarRefreshViper *jgi_jar_refresh_viper_new(JgiJarEntry *entry, VipSnapshot *snapshot);

G_END_DECLS


#endif /* JGIJARREFRESHVIPER_H_ */
