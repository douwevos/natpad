/*
   File:    jagindexerjremap.h
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

#ifndef JAGINDEXERJREMAP_H_
#define JAGINDEXERJREMAP_H_

#include "../jar/jagindexerjarmap.h"
#include "jgijreentry.h"
#include "jagjrelink.h"
#include <caterpillar.h>
#include <viper.h>
#include <moose.h>
#include <worm.h>

G_BEGIN_DECLS

#define JAG_TYPE_INDEXER_JRE_MAP              (jag_indexer_jre_map_get_type())
#define JAG_INDEXER_JRE_MAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_indexer_jre_map_get_type(), JagIndexerJreMap))
#define JAG_INDEXER_JRE_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_INDEXER_JRE_MAP, JagIndexerJreMapClass))
#define JAG_IS_INDEXER_JRE_MAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_INDEXER_JRE_MAP))
#define JAG_IS_INDEXER_JRE_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_INDEXER_JRE_MAP))
#define JAG_INDEXER_JRE_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_INDEXER_JRE_MAP, JagIndexerJreMapClass))
#define JAG_INDEXER_JRE_MAP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_INDEXER_JRE_MAP, JagIndexerJreMapPrivate))


typedef struct _JagIndexerJreMap               JagIndexerJreMap;
typedef struct _JagIndexerJreMapPrivate        JagIndexerJreMapPrivate;
typedef struct _JagIndexerJreMapClass          JagIndexerJreMapClass;


struct _JagIndexerJreMap {
	GObject parent;
	JagIndexerJreMapPrivate *priv;
};

struct _JagIndexerJreMapClass {
	GObjectClass parent_class;
};


GType jag_indexer_jre_map_get_type();

JagIndexerJreMap *jag_indexer_jre_map_new(JagIndexerJarMap *jar_map, MooService *moo_service, VipService *vip_service, WorService *wor_service);

void jag_indexer_jre_map_set_jre_list(JagIndexerJreMap *jre_map, CatArrayWo *a_jre_list);


JagJreLink *jag_indexer_jre_map_get_jre_link_ref(JagIndexerJreMap *jre_map, CatStringWo *a_jre_name);

G_END_DECLS

#endif /* JAGINDEXERJREMAP_H_ */
