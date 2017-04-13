/*
   File:    armeditorconnector.h
   Project: armadillo
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

#ifndef SRC_EDITOR_ARMEDITORCONNECTOR_H_
#define SRC_EDITOR_ARMEDITORCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define ARM_TYPE_EDITOR_CONNECTOR              (arm_editor_connector_get_type())
#define ARM_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), arm_editor_connector_get_type(), ArmEditorConnector))
#define ARM_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ARM_TYPE_EDITOR_CONNECTOR, ArmEditorConnectorClass))
#define ARM_IS_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ARM_TYPE_EDITOR_CONNECTOR))
#define ARM_IS_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ARM_TYPE_EDITOR_CONNECTOR))
#define ARM_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ARM_TYPE_EDITOR_CONNECTOR, ArmEditorConnectorClass))


typedef struct _ArmEditorConnector               ArmEditorConnector;
typedef struct _ArmEditorConnectorPrivate        ArmEditorConnectorPrivate;
typedef struct _ArmEditorConnectorClass          ArmEditorConnectorClass;


struct _ArmEditorConnector {
	GObject parent;
};

struct _ArmEditorConnectorClass {
	GObjectClass parent_class;
};


GType arm_editor_connector_get_type();

ArmEditorConnector *arm_editor_connector_new();

G_END_DECLS

#endif /* SRC_EDITOR_ARMEDITORCONNECTOR_H_ */
