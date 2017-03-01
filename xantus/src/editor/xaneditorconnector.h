/*
   File:    xaneditorconnector.h
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 10, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef EDITOR_XANEDITORCONNECTOR_H_
#define EDITOR_XANEDITORCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define XAN_TYPE_EDITOR_CONNECTOR              (xan_editor_connector_get_type())
#define XAN_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), xan_editor_connector_get_type(), XanEditorConnector))
#define XAN_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XAN_TYPE_EDITOR_CONNECTOR, XanEditorConnectorClass))
#define XAN_IS_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAN_TYPE_EDITOR_CONNECTOR))
#define XAN_IS_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XAN_TYPE_EDITOR_CONNECTOR))
#define XAN_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XAN_TYPE_EDITOR_CONNECTOR, XanEditorConnectorClass))


typedef struct _XanEditorConnector               XanEditorConnector;
typedef struct _XanEditorConnectorPrivate        XanEditorConnectorPrivate;
typedef struct _XanEditorConnectorClass          XanEditorConnectorClass;


struct _XanEditorConnector {
	GObject parent;
};

struct _XanEditorConnectorClass {
	GObjectClass parent_class;
};


GType xan_editor_connector_get_type();

XanEditorConnector *xan_editor_connector_new();

G_END_DECLS

#endif /* EDITOR_XANEDITORCONNECTOR_H_ */
