/*
   File:    mulluaeditorconnector.h
   Project: mule
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

#ifndef LUA_MULLUAEDITORCONNECTOR_H_
#define LUA_MULLUAEDITORCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MUL_TYPE_LUA_EDITOR_CONNECTOR              (mul_lua_editor_connector_get_type())
#define MUL_LUA_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_lua_editor_connector_get_type(), MulLuaEditorConnector))
#define MUL_LUA_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_LUA_EDITOR_CONNECTOR, MulLuaEditorConnectorClass))
#define MUL_IS_LUA_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_LUA_EDITOR_CONNECTOR))
#define MUL_IS_LUA_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_LUA_EDITOR_CONNECTOR))
#define MUL_LUA_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_LUA_EDITOR_CONNECTOR, MulLuaEditorConnectorClass))


typedef struct _MulLuaEditorConnector               MulLuaEditorConnector;
typedef struct _MulLuaEditorConnectorPrivate        MulLuaEditorConnectorPrivate;
typedef struct _MulLuaEditorConnectorClass          MulLuaEditorConnectorClass;


struct _MulLuaEditorConnector {
	GObject parent;
};

struct _MulLuaEditorConnectorClass {
	GObjectClass parent_class;
};


GType mul_lua_editor_connector_get_type();

MulLuaEditorConnector *mul_lua_editor_connector_new();

G_END_DECLS

#endif /* LUA_MULLUAEDITORCONNECTOR_H_ */
