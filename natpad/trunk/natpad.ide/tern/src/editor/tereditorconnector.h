/*
   File:    tereditorconnector.h
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

#ifndef EDITOR_TEREDITORCONNECTOR_H_
#define EDITOR_TEREDITORCONNECTOR_H_

#include "../grammar/tergrammarmap.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define TER_TYPE_EDITOR_CONNECTOR              (ter_editor_connector_get_type())
#define TER_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_editor_connector_get_type(), TerEditorConnector))
#define TER_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_EDITOR_CONNECTOR, TerEditorConnectorClass))
#define TER_IS_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_EDITOR_CONNECTOR))
#define TER_IS_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_EDITOR_CONNECTOR))
#define TER_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_EDITOR_CONNECTOR, TerEditorConnectorClass))


typedef struct _TerEditorConnector               TerEditorConnector;
typedef struct _TerEditorConnectorPrivate        TerEditorConnectorPrivate;
typedef struct _TerEditorConnectorClass          TerEditorConnectorClass;


struct _TerEditorConnector {
	GObject parent;
};

struct _TerEditorConnectorClass {
	GObjectClass parent_class;
};


GType ter_editor_connector_get_type();

TerEditorConnector *ter_editor_connector_new(CatStringWo *slot_key, TerGrammarMap *grammar_map);

G_END_DECLS

#endif /* EDITOR_TEREDITORCONNECTOR_H_ */
