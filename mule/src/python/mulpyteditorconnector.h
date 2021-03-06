/*
   File:    mulpyteditorconnector.h
   Project: mule
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

#ifndef PYTHON_MULPYTEDITORCONNECTOR_H_
#define PYTHON_MULPYTEDITORCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MUL_TYPE_PYT_EDITOR_CONNECTOR              (mul_pyt_editor_connector_get_type())
#define MUL_PYT_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_pyt_editor_connector_get_type(), MulPytEditorConnector))
#define MUL_PYT_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_PYT_EDITOR_CONNECTOR, MulPytEditorConnectorClass))
#define MUL_IS_PYT_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_PYT_EDITOR_CONNECTOR))
#define MUL_IS_PYT_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_PYT_EDITOR_CONNECTOR))
#define MUL_PYT_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_PYT_EDITOR_CONNECTOR, MulPytEditorConnectorClass))


typedef struct _MulPytEditorConnector               MulPytEditorConnector;
typedef struct _MulPytEditorConnectorPrivate        MulPytEditorConnectorPrivate;
typedef struct _MulPytEditorConnectorClass          MulPytEditorConnectorClass;


struct _MulPytEditorConnector {
	GObject parent;
};

struct _MulPytEditorConnectorClass {
	GObjectClass parent_class;
};


GType mul_pyt_editor_connector_get_type();

MulPytEditorConnector *mul_pyt_editor_connector_new();

G_END_DECLS

#endif /* PYTHON_MULPYTEDITORCONNECTOR_H_ */
