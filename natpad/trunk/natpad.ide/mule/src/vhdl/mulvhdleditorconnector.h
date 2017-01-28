/*
   File:    mulvhdleditorconnector.h
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

#ifndef VHDL_MULVHDLEDITORCONNECTOR_H_
#define VHDL_MULVHDLEDITORCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MUL_TYPE_VHDL_CONNECTOR              (mul_vhdl_editor_connector_get_type())
#define MUL_VHDL_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_vhdl_editor_connector_get_type(), MulVhdlEditorConnector))
#define MUL_VHDL_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_VHDL_CONNECTOR, MulVhdlEditorConnectorClass))
#define MUL_IS_VHDL_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_VHDL_CONNECTOR))
#define MUL_IS_VHDL_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_VHDL_CONNECTOR))
#define MUL_VHDL_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_VHDL_CONNECTOR, MulVhdlEditorConnectorClass))


typedef struct _MulVhdlEditorConnector               MulVhdlEditorConnector;
typedef struct _MulVhdlEditorConnectorPrivate        MulVhdlEditorConnectorPrivate;
typedef struct _MulVhdlEditorConnectorClass          MulVhdlEditorConnectorClass;


struct _MulVhdlEditorConnector {
	GObject parent;
};

struct _MulVhdlEditorConnectorClass {
	GObjectClass parent_class;
};


GType mul_vhdl_editor_connector_get_type();

MulVhdlEditorConnector *mul_vhdl_editor_connector_new();

G_END_DECLS

#endif /* VHDL_MULVHDLEDITORCONNECTOR_H_ */
