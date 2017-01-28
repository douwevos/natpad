/*
   File:    mulrusteditorconnector.h
   Project: mule
   Author:  Douwe Vos
   Date:    Jan 9, 2017
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

#ifndef RUST_MULRUSTEDITORCONNECTOR_H_
#define RUST_MULRUSTEDITORCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MUL_TYPE_RUST_EDITOR_CONNECTOR              (mul_rust_editor_connector_get_type())
#define MUL_RUST_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_rust_editor_connector_get_type(), MulRustEditorConnector))
#define MUL_RUST_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_RUST_EDITOR_CONNECTOR, MulRustEditorConnectorClass))
#define MUL_IS_RUST_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_RUST_EDITOR_CONNECTOR))
#define MUL_IS_RUST_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_RUST_EDITOR_CONNECTOR))
#define MUL_RUST_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_RUST_EDITOR_CONNECTOR, MulRustEditorConnectorClass))


typedef struct _MulRustEditorConnector               MulRustEditorConnector;
typedef struct _MulRustEditorConnectorPrivate        MulRustEditorConnectorPrivate;
typedef struct _MulRustEditorConnectorClass          MulRustEditorConnectorClass;


struct _MulRustEditorConnector {
	GObject parent;
};

struct _MulRustEditorConnectorClass {
	GObjectClass parent_class;
};


GType mul_rust_editor_connector_get_type();

MulRustEditorConnector *mul_rust_editor_connector_new();

G_END_DECLS

#endif /* RUST_MULRUSTEDITORCONNECTOR_H_ */
