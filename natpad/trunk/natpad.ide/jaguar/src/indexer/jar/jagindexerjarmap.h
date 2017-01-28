/*
   File:    jagindexerjarmap.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 2, 2014
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

#ifndef JAGINDEXEDJARMAP_H_
#define JAGINDEXEDJARMAP_H_

#include "jgijarentry.h"
#include <caterpillar.h>
#include <moose.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_INDEXER_JAR_MAP              (jag_indexer_jar_map_get_type())
#define JAG_INDEXER_JAR_MAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_indexer_jar_map_get_type(), JagIndexerJarMap))
#define JAG_INDEXER_JAR_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_INDEXER_JAR_MAP, JagIndexerJarMapClass))
#define JAG_IS_INDEXER_JAR_MAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_INDEXER_JAR_MAP))
#define JAG_IS_INDEXER_JAR_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_INDEXER_JAR_MAP))
#define JAG_INDEXER_JAR_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_INDEXER_JAR_MAP, JagIndexerJarMapClass))
#define JAG_INDEXER_JAR_MAP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_INDEXER_JAR_MAP, JagIndexerJarMapPrivate))


typedef struct _JagIndexerJarMap               JagIndexerJarMap;
typedef struct _JagIndexerJarMapPrivate        JagIndexerJarMapPrivate;
typedef struct _JagIndexerJarMapClass          JagIndexerJarMapClass;


struct _JagIndexerJarMap {
	GObject parent;
	JagIndexerJarMapPrivate *priv;
};

struct _JagIndexerJarMapClass {
	GObjectClass parent_class;
};


GType jag_indexer_jar_map_get_type();

JagIndexerJarMap *jag_indexer_jar_map_new(MooService *moo_service, VipService *vip_service);

JgiJarEntry *jag_indexer_jar_map_attach(JagIndexerJarMap *jar_map, VipNodePath *full_jar_path);

void jag_indexer_jar_map_detach(JagIndexerJarMap *jar_map, JgiJarEntry *entry);

G_END_DECLS

#endif /* JAGINDEXEDJARMAP_H_ */
