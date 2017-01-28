/*
   File:    cuseditorconnector.h
   Project: cuscus
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

#ifndef EDITOR_CUSEDITORCONNECTOR_H_
#define EDITOR_CUSEDITORCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define CUS_TYPE_EDITOR_CONNECTOR              (cus_editor_connector_get_type())
#define CUS_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cus_editor_connector_get_type(), CusEditorConnector))
#define CUS_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CUS_TYPE_EDITOR_CONNECTOR, CusEditorConnectorClass))
#define CUS_IS_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CUS_TYPE_EDITOR_CONNECTOR))
#define CUS_IS_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CUS_TYPE_EDITOR_CONNECTOR))
#define CUS_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CUS_TYPE_EDITOR_CONNECTOR, CusEditorConnectorClass))


typedef struct _CusEditorConnector               CusEditorConnector;
typedef struct _CusEditorConnectorPrivate        CusEditorConnectorPrivate;
typedef struct _CusEditorConnectorClass          CusEditorConnectorClass;


struct _CusEditorConnector {
	GObject parent;
};

struct _CusEditorConnectorClass {
	GObjectClass parent_class;
};


GType cus_editor_connector_get_type();

CusEditorConnector *cus_editor_connector_new();

G_END_DECLS

#endif /* EDITOR_CUSEDITORCONNECTOR_H_ */
