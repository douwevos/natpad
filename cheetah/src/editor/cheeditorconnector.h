/*
   File:    cheeditorconnector.h
   Project: cheetah
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

#ifndef EDITOR_CHEEDITORCONNECTOR_H_
#define EDITOR_CHEEDITORCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define CHE_TYPE_EDITOR_CONNECTOR              (che_editor_connector_get_type())
#define CHE_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), che_editor_connector_get_type(), CheEditorConnector))
#define CHE_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHE_TYPE_EDITOR_CONNECTOR, CheEditorConnectorClass))
#define CHE_IS_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHE_TYPE_EDITOR_CONNECTOR))
#define CHE_IS_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHE_TYPE_EDITOR_CONNECTOR))
#define CHE_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHE_TYPE_EDITOR_CONNECTOR, CheEditorConnectorClass))


typedef struct _CheEditorConnector               CheEditorConnector;
typedef struct _CheEditorConnectorPrivate        CheEditorConnectorPrivate;
typedef struct _CheEditorConnectorClass          CheEditorConnectorClass;


struct _CheEditorConnector {
	GObject parent;
};

struct _CheEditorConnectorClass {
	GObjectClass parent_class;
};


GType che_editor_connector_get_type();

CheEditorConnector *che_editor_connector_new();

G_END_DECLS

#endif /* EDITOR_CHEEDITORCONNECTOR_H_ */
