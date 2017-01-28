/*
   File:    groededitorfactory.h
   Project: gross-editor
   Author:  Douwe Vos
   Date:    Oct 12, 2016
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

#ifndef EDITOR_GROEDEDITORFACTORY_H_
#define EDITOR_GROEDEDITORFACTORY_H_

#include "groededitorconnector.h"
#include <caterpillar.h>
#include <elk.h>
#include <leafhopper.h>
#include <dragonfly.h>
#include <viper.h>
#include <cow.h>

G_BEGIN_DECLS

#define GROED_TYPE_EDITOR_FACTORY              (groed_editor_factory_get_type())
#define GROED_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), groed_editor_factory_get_type(), GroEdEditorFactory))
#define GROED_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GROED_TYPE_EDITOR_FACTORY, GroEdEditorFactoryClass))
#define GROED_IS_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GROED_TYPE_EDITOR_FACTORY))
#define GROED_IS_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GROED_TYPE_EDITOR_FACTORY))
#define GROED_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GROED_TYPE_EDITOR_FACTORY, GroEdEditorFactoryClass))


typedef struct _GroEdEditorFactory               GroEdEditorFactory;
typedef struct _GroEdEditorFactoryPrivate        GroEdEditorFactoryPrivate;
typedef struct _GroEdEditorFactoryClass          GroEdEditorFactoryClass;


struct _GroEdEditorFactory {
	GObject parent;
};

struct _GroEdEditorFactoryClass {
	GObjectClass parent_class;
};


GType groed_editor_factory_get_type();

GroEdEditorFactory *groed_editor_factory_new(LeaIPanelOwner *panel_owner, GroEdEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file);

G_END_DECLS

#endif /* EDITOR_GROEDEDITORFACTORY_H_ */
