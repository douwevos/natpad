/*
   File:    jagjarfilemapper.h
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


#ifndef JAGJARFILEMAPPER_H_
#define JAGJARFILEMAPPER_H_


#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_JAR_FILE_MAPPER              (jag_jar_file_mapper_get_type())
#define JAG_JAR_FILE_MAPPER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_jar_file_mapper_get_type(), JagJarFileMapper))
#define JAG_JAR_FILE_MAPPER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_JAR_FILE_MAPPER, JagJarFileMapperClass))
#define JAG_IS_JAR_FILE_MAPPER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_JAR_FILE_MAPPER))
#define JAG_IS_JAR_FILE_MAPPER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_JAR_FILE_MAPPER))
#define JAG_JAR_FILE_MAPPER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_JAR_FILE_MAPPER, JagJarFileMapperClass))
#define JAG_JAR_FILE_MAPPER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_JAR_FILE_MAPPER, JagJarFileMapperPrivate))


typedef struct _JagJarFileMapper               JagJarFileMapper;
typedef struct _JagJarFileMapperPrivate        JagJarFileMapperPrivate;
typedef struct _JagJarFileMapperClass          JagJarFileMapperClass;


struct _JagJarFileMapper {
	GObject parent;
	JagJarFileMapperPrivate *priv;
};

struct _JagJarFileMapperClass {
	GObjectClass parent_class;
};


GType jag_jar_file_mapper_get_type();

JagJarFileMapper *jag_jar_file_mapper_new(VipISequence *sequence);

G_END_DECLS



#endif /* JAGJARFILEMAPPER_H_ */
