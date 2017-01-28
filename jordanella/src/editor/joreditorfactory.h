/*
   File:    joreditorfactory.h
   Project: jordanella
   Author:  Douwe Vos
   Date:    Feb 6, 2016
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

#ifndef EDITOR_JOREDITORFACTORY_H_
#define EDITOR_JOREDITORFACTORY_H_

#include "joreditorconnector.h"
#include <caterpillar.h>
#include <elk.h>
#include <leafhopper.h>
#include <dragonfly.h>
#include <viper.h>
#include <cow.h>

G_BEGIN_DECLS

#define JOR_TYPE_EDITOR_FACTORY              (jor_editor_factory_get_type())
#define JOR_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jor_editor_factory_get_type(), JorEditorFactory))
#define JOR_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JOR_TYPE_EDITOR_FACTORY, JorEditorFactoryClass))
#define JOR_IS_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JOR_TYPE_EDITOR_FACTORY))
#define JOR_IS_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JOR_TYPE_EDITOR_FACTORY))
#define JOR_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JOR_TYPE_EDITOR_FACTORY, JorEditorFactoryClass))


typedef struct _JorEditorFactory               JorEditorFactory;
typedef struct _JorEditorFactoryPrivate        JorEditorFactoryPrivate;
typedef struct _JorEditorFactoryClass          JorEditorFactoryClass;


struct _JorEditorFactory {
	GObject parent;
};

struct _JorEditorFactoryClass {
	GObjectClass parent_class;
};


GType jor_editor_factory_get_type();

JorEditorFactory *jor_editor_factory_new(LeaIPanelOwner *panel_owner, JorEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file);

G_END_DECLS

#endif /* EDITOR_JOREDITORFACTORY_H_ */
