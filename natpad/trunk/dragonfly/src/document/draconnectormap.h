/*
   File:    draconnectormap.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 26, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef DOCUMENT_DRACONNECTORMAP_H_
#define DOCUMENT_DRACONNECTORMAP_H_

#include "draiconnectorrequestfactory.h"
#include "draconnectorkey.h"
#include "dradocumentconnector.h"
#include "../spell/draspellhelper.h"
#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_CONNECTOR_MAP              (dra_connector_map_get_type())
#define DRA_CONNECTOR_MAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_connector_map_get_type(), DraConnectorMap))
#define DRA_CONNECTOR_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_CONNECTOR_MAP, DraConnectorMapClass))
#define DRA_IS_CONNECTOR_MAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_CONNECTOR_MAP))
#define DRA_IS_CONNECTOR_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_CONNECTOR_MAP))
#define DRA_CONNECTOR_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_CONNECTOR_MAP, DraConnectorMapClass))


typedef struct _DraConnectorMap               DraConnectorMap;
typedef struct _DraConnectorMapPrivate        DraConnectorMapPrivate;
typedef struct _DraConnectorMapClass          DraConnectorMapClass;


struct _DraConnectorMap {
	GObject parent;
};

struct _DraConnectorMapClass {
	GObjectClass parent_class;
};


GType dra_connector_map_get_type();

DraConnectorMap *dra_connector_map_new();

void dra_connector_map_connect_document(DraConnectorMap *map, ChaDocument *document, CatStringWo *a_slot_key, DraIConnectorRequestFactory *factory);
void dra_connector_map_disconnect_document(DraConnectorMap *map, ChaDocument *document, CatStringWo *a_slot_key);

DraSpellHelper *dra_connector_map_get_spell_helper(DraConnectorMap *connector_map);

void dra_connector_map_reconfigure(DraConnectorMap *connector_map, DraPreferencesWo *a_prefs);

G_END_DECLS


#endif /* DOCUMENT_DRACONNECTORMAP_H_ */
