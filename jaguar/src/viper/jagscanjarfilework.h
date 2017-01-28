/*
   File:    jagscanjarfilework.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 5, 2013
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


#ifndef JAGSCANJARFILEWORK_H_
#define JAGSCANJARFILEWORK_H_

#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_SCAN_JAR_FILE_WORK              (jag_scan_jar_file_work_get_type())
#define JAG_SCAN_JAR_FILE_WORK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_scan_jar_file_work_get_type(), JagScanJarFileWork))
#define JAG_SCAN_JAR_FILE_WORK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_SCAN_JAR_FILE_WORK, JagScanJarFileWorkClass))
#define JAG_IS_SCAN_JAR_FILE_WORK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_SCAN_JAR_FILE_WORK))
#define JAG_IS_SCAN_JAR_FILE_WORK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_SCAN_JAR_FILE_WORK))
#define JAG_SCAN_JAR_FILE_WORK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_SCAN_JAR_FILE_WORK, JagScanJarFileWorkClass))
#define JAG_SCAN_JAR_FILE_WORK_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_SCAN_JAR_FILE_WORK, JagScanJarFileWorkPrivate))


typedef struct _JagScanJarFileWork               JagScanJarFileWork;
typedef struct _JagScanJarFileWorkPrivate        JagScanJarFileWorkPrivate;
typedef struct _JagScanJarFileWorkClass          JagScanJarFileWorkClass;


struct _JagScanJarFileWork {
	GObject parent;
	JagScanJarFileWorkPrivate *priv;
};

struct _JagScanJarFileWorkClass {
	GObjectClass parent_class;
};


GType jag_scan_jar_file_work_get_type();

JagScanJarFileWork *jag_scan_jar_file_work_new(VipNode *main_node, gboolean validated_by_parent);

G_END_DECLS



#endif /* JAGSCANJARFILEWORK_H_ */
