/*
   File:    tereditorconnectormap.h
   Project: tern
   Author:  Douwe Vos
   Date:    Dec 28, 2016
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

#ifndef EDITOR_TEREDITORCONNECTORMAP_H_
#define EDITOR_TEREDITORCONNECTORMAP_H_

#include "tereditorconnector.h"
#include "../grammar/tergrammarmap.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define TER_TYPE_EDITOR_CONNECTOR_MAP              (ter_editor_connector_map_get_type())
#define TER_EDITOR_CONNECTOR_MAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_editor_connector_map_get_type(), TerEditorConnectorMap))
#define TER_EDITOR_CONNECTOR_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_EDITOR_CONNECTOR_MAP, TerEditorConnectorMapClass))
#define TER_IS_EDITOR_CONNECTOR_MAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_EDITOR_CONNECTOR_MAP))
#define TER_IS_EDITOR_CONNECTOR_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_EDITOR_CONNECTOR_MAP))
#define TER_EDITOR_CONNECTOR_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_EDITOR_CONNECTOR_MAP, TerEditorConnectorMapClass))


typedef struct _TerEditorConnectorMap               TerEditorConnectorMap;
typedef struct _TerEditorConnectorMapPrivate        TerEditorConnectorMapPrivate;
typedef struct _TerEditorConnectorMapClass          TerEditorConnectorMapClass;


struct _TerEditorConnectorMap {
	GObject parent;
};

struct _TerEditorConnectorMapClass {
	GObjectClass parent_class;
};


GType ter_editor_connector_map_get_type();

TerEditorConnectorMap *ter_editor_connector_map_new(TerGrammarMap *grammar_map);

void ter_editor_connector_map_set_names(TerEditorConnectorMap *connector_map, CatArrayWo *class_names);
TerEditorConnector *ter_editor_connector_map_get_connector(TerEditorConnectorMap *connector_map, CatStringWo *clazz_name);

G_END_DECLS

#endif /* EDITOR_TEREDITORCONNECTORMAP_H_ */
