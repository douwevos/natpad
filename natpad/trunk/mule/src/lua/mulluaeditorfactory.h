/*
   File:    mulluaeditorfactory.h
   Project: mule
   Author:  Douwe Vos
   Date:    Jul 11, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef LUA_MULLUAEDITORFACTORY_H_
#define LUA_MULLUAEDITORFACTORY_H_

#include "mulluaeditorconnector.h"
#include <elk.h>
#include <caterpillar.h>
#include <dragonfly.h>
#include <leafhopper.h>
#include <viper.h>
#include <cow.h>

G_BEGIN_DECLS

#define MUL_TYPE_LAU_EDITOR_FACTORY              (mul_lua_editor_factory_get_type())
#define MUL_LAU_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_lua_editor_factory_get_type(), MulLuaEditorFactory))
#define MUL_LAU_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_LAU_EDITOR_FACTORY, MulLuaEditorFactoryClass))
#define MUL_IS_LAU_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_LAU_EDITOR_FACTORY))
#define MUL_IS_LAU_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_LAU_EDITOR_FACTORY))
#define MUL_LAU_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_LAU_EDITOR_FACTORY, MulLuaEditorFactoryClass))


typedef struct _MulLuaEditorFactory               MulLuaEditorFactory;
typedef struct _MulLuaEditorFactoryPrivate        MulLuaEditorFactoryPrivate;
typedef struct _MulLuaEditorFactoryClass          MulLuaEditorFactoryClass;


struct _MulLuaEditorFactory {
	GObject parent;
};

struct _MulLuaEditorFactoryClass {
	GObjectClass parent_class;
};


GType mul_lua_editor_factory_get_type();

MulLuaEditorFactory *mul_lua_editor_factory_new(LeaIPanelOwner *panel_owner, MulLuaEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file);

G_END_DECLS

#endif /* LUA_MULLUAEDITORFACTORY_H_ */
