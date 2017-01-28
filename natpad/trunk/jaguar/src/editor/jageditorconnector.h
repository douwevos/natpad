/*
   File:    jageditorconnector.h
   Project: jaguar
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

#ifndef SRC_EDITOR_JAGEDITORCONNECTOR_H_
#define SRC_EDITOR_JAGEDITORCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_EDITOR_CONNECTOR              (jag_editor_connector_get_type())
#define JAG_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_editor_connector_get_type(), JagEditorConnector))
#define JAG_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_EDITOR_CONNECTOR, JagEditorConnectorClass))
#define JAG_IS_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_EDITOR_CONNECTOR))
#define JAG_IS_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_EDITOR_CONNECTOR))
#define JAG_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_EDITOR_CONNECTOR, JagEditorConnectorClass))


typedef struct _JagEditorConnector               JagEditorConnector;
typedef struct _JagEditorConnectorPrivate        JagEditorConnectorPrivate;
typedef struct _JagEditorConnectorClass          JagEditorConnectorClass;


struct _JagEditorConnector {
	GObject parent;
};

struct _JagEditorConnectorClass {
	GObjectClass parent_class;
};


GType jag_editor_connector_get_type();

JagEditorConnector *jag_editor_connector_new();

G_END_DECLS

#endif /* SRC_EDITOR_JAGEDITORCONNECTOR_H_ */
