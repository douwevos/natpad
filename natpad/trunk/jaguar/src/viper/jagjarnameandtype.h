/*
   File:    jagjarnameandtype.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 3, 2013
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


#ifndef JAGJARNAMEANDTYPE_H_
#define JAGJARNAMEANDTYPE_H_

#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_JAR_NAME_AND_TYPE              (jag_jar_name_and_type_get_type())
#define JAG_JAR_NAME_AND_TYPE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_jar_name_and_type_get_type(), JagJarNameAndType))
#define JAG_JAR_NAME_AND_TYPE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_JAR_NAME_AND_TYPE, JagJarNameAndTypeClass))
#define JAG_IS_JAR_NAME_AND_TYPE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_JAR_NAME_AND_TYPE))
#define JAG_IS_JAR_NAME_AND_TYPE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_JAR_NAME_AND_TYPE))
#define JAG_JAR_NAME_AND_TYPE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_JAR_NAME_AND_TYPE, JagJarNameAndTypeClass))
#define JAG_JAR_NAME_AND_TYPE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_JAR_NAME_AND_TYPE, JagJarNameAndTypePrivate))


typedef struct _JagJarNameAndType               JagJarNameAndType;
typedef struct _JagJarNameAndTypePrivate        JagJarNameAndTypePrivate;
typedef struct _JagJarNameAndTypeClass          JagJarNameAndTypeClass;


struct _JagJarNameAndType {
	GObject parent;
	JagJarNameAndTypePrivate *priv;
};

struct _JagJarNameAndTypeClass {
	GObjectClass parent_class;
};


GType jag_jar_name_and_type_get_type();

int jag_jar_name_and_type_hash(JagJarNameAndType *name_and_type);

gboolean jag_jar_name_and_type_equal(JagJarNameAndType *name_and_type_a, JagJarNameAndType *name_and_type_b);


JagJarNameAndType *jag_jar_name_and_type_new(CatStringWo *a_name, gboolean is_map);


JagJarNameAndType *jag_jar_name_and_type_add(JagJarNameAndType *name_and_type, CatStringWo *a_path, gboolean isMap);


VipIResource *jag_jar_name_and_type_get_vip_resource(JagJarNameAndType *name_and_type, VipIFile *mainFile, CatArrayWo *a_parent_path);


void jag_jar_name_and_type_set_index(JagJarNameAndType *name_and_type, int entry_index);

int jag_jar_name_and_type_get_index(JagJarNameAndType *name_and_type);

int jag_jar_name_and_type_child_count(JagJarNameAndType *name_and_type);

CatIIterator *jag_jar_name_and_type_child_iterator(JagJarNameAndType *name_and_type);

gboolean jag_jar_name_and_type_is_map(JagJarNameAndType *name_and_type);

CatStringWo *jag_jar_name_and_type_get_name(JagJarNameAndType *name_and_type);

G_END_DECLS

#endif /* JAGJARNAMEANDTYPE_H_ */
