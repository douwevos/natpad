/*
   File:    tereditorfactory.h
   Project: tern
   Author:  Douwe Vos
   Date:    Mar 12, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef TEREDITORFACTORY_H_
#define TEREDITORFACTORY_H_

#include "tereditorconnectormap.h"
#include <caterpillar.h>
#include <elk.h>
#include <viper.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define TER_TYPE_EDITOR_FACTORY              (ter_editor_factory_get_type())
#define TER_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_editor_factory_get_type(), TerEditorFactory))
#define TER_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_EDITOR_FACTORY, TerEditorFactoryClass))
#define TER_IS_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_EDITOR_FACTORY))
#define TER_IS_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_EDITOR_FACTORY))
#define TER_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_EDITOR_FACTORY, TerEditorFactoryClass))
#define TER_EDITOR_FACTORY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TER_TYPE_EDITOR_FACTORY, TerEditorFactoryPrivate))


typedef struct _TerEditorFactory               TerEditorFactory;
typedef struct _TerEditorFactoryPrivate        TerEditorFactoryPrivate;
typedef struct _TerEditorFactoryClass          TerEditorFactoryClass;


struct _TerEditorFactory {
	GObject parent;
	TerEditorFactoryPrivate *priv;
};

struct _TerEditorFactoryClass {
	GObjectClass parent_class;
};


GType ter_editor_factory_get_type();

TerEditorFactory *ter_editor_factory_new(LeaIPanelOwner *panel_owner, ElkDocumentIO *document_io, TerEditorConnectorMap *connector_map, TerGrammarMap *grammar_map, VipIFile *document_file, CatHashMapWo *a_editor_prefs_map, CatStringWo *a_clazz_name);

G_END_DECLS

#endif /* TEREDITORFACTORY_H_ */
