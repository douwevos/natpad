/*
   File:    jagvipjarfile.h
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

#ifndef JAGVIPJARFILE_H_
#define JAGVIPJARFILE_H_

#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_VIP_JAR_FILE              (jag_vip_jar_file_get_type())
#define JAG_VIP_JAR_FILE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_vip_jar_file_get_type(), JagVipJarFile))
#define JAG_VIP_JAR_FILE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_VIP_JAR_FILE, JagVipJarFileClass))
#define JAG_IS_VIP_JAR_FILE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_VIP_JAR_FILE))
#define JAG_IS_VIP_JAR_FILE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_VIP_JAR_FILE))
#define JAG_VIP_JAR_FILE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_VIP_JAR_FILE, JagVipJarFileClass))

typedef struct _JagVipJarFile               JagVipJarFile;
typedef struct _JagVipJarFilePrivate        JagVipJarFilePrivate;
typedef struct _JagVipJarFileClass          JagVipJarFileClass;


struct _JagVipJarFile {
	GObject parent;
};

struct _JagVipJarFileClass {
	GObjectClass parent_class;
};

GType jag_vip_jar_file_get_type();

JagVipJarFile *jag_vip_jar_file_new(VipIFile *main_file, CatStringWo *a_entry_name, int entry_index);

G_END_DECLS

#endif /* JAGVIPJARFILE_H_ */
