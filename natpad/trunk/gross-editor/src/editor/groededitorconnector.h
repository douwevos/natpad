/*
   File:    groededitorconnector.h
   Project: gross-editor
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

#ifndef EDITOR_GROEDEDITORCONNECTOR_H_
#define EDITOR_GROEDEDITORCONNECTOR_H_

#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define GROED_TYPE_EDITOR_CONNECTOR              (groed_editor_connector_get_type())
#define GROED_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), groed_editor_connector_get_type(), GroEdEditorConnector))
#define GROED_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GROED_TYPE_EDITOR_CONNECTOR, GroEdEditorConnectorClass))
#define GROED_IS_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GROED_TYPE_EDITOR_CONNECTOR))
#define GROED_IS_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GROED_TYPE_EDITOR_CONNECTOR))
#define GROED_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GROED_TYPE_EDITOR_CONNECTOR, GroEdEditorConnectorClass))


typedef struct _GroEdEditorConnector               GroEdEditorConnector;
typedef struct _GroEdEditorConnectorPrivate        GroEdEditorConnectorPrivate;
typedef struct _GroEdEditorConnectorClass          GroEdEditorConnectorClass;


struct _GroEdEditorConnector {
	GObject parent;
};

struct _GroEdEditorConnectorClass {
	GObjectClass parent_class;
};


GType groed_editor_connector_get_type();

GroEdEditorConnector *groed_editor_connector_new();

G_END_DECLS

#endif /* EDITOR_GROEDEDITORCONNECTOR_H_ */
