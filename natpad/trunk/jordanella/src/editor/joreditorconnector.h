/*
   File:    joreditorconnector.h
   Project: jordanella
   Author:  Douwe Vos
   Date:    Dec 27, 2016
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

#ifndef EDITOR_JOREDITORCONNECTOR_H_
#define EDITOR_JOREDITORCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JOR_TYPE_EDITOR_CONNECTOR              (jor_editor_connector_get_type())
#define JOR_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jor_editor_connector_get_type(), JorEditorConnector))
#define JOR_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JOR_TYPE_EDITOR_CONNECTOR, JorEditorConnectorClass))
#define JOR_IS_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JOR_TYPE_EDITOR_CONNECTOR))
#define JOR_IS_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JOR_TYPE_EDITOR_CONNECTOR))
#define JOR_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JOR_TYPE_EDITOR_CONNECTOR, JorEditorConnectorClass))


typedef struct _JorEditorConnector               JorEditorConnector;
typedef struct _JorEditorConnectorPrivate        JorEditorConnectorPrivate;
typedef struct _JorEditorConnectorClass          JorEditorConnectorClass;


struct _JorEditorConnector {
	GObject parent;
};

struct _JorEditorConnectorClass {
	GObjectClass parent_class;
};


GType jor_editor_connector_get_type();

JorEditorConnector *jor_editor_connector_new();

G_END_DECLS

#endif /* EDITOR_JOREDITORCONNECTOR_H_ */
