/*
   File:    jagclasseditorfactory.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Mar 19, 2014
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

#ifndef JAGCLASSEDITORFACTORY_H_
#define JAGCLASSEDITORFACTORY_H_

#include "../indexer/jagindexer.h"
#include "jageditorconnector.h"
#include <caterpillar.h>
#include <elk.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_CLASS_EDITOR_FACTORY              (jag_class_editor_factory_get_type())
#define JAG_CLASS_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_class_editor_factory_get_type(), JagClassEditorFactory))
#define JAG_CLASS_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_CLASS_EDITOR_FACTORY, JagClassEditorFactoryClass))
#define JAG_IS_CLASS_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_CLASS_EDITOR_FACTORY))
#define JAG_IS_CLASS_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_CLASS_EDITOR_FACTORY))
#define JAG_CLASS_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_CLASS_EDITOR_FACTORY, JagClassEditorFactoryClass))


typedef struct _JagClassEditorFactory               JagClassEditorFactory;
typedef struct _JagClassEditorFactoryPrivate        JagClassEditorFactoryPrivate;
typedef struct _JagClassEditorFactoryClass          JagClassEditorFactoryClass;


struct _JagClassEditorFactory {
	GObject parent;
};

struct _JagClassEditorFactoryClass {
	GObjectClass parent_class;
};


GType jag_class_editor_factory_get_type();

JagClassEditorFactory *jag_class_editor_factory_new(LeaIPanelOwner *panel_owner, JagEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file/*, JagIndexer *indexer*/);

G_END_DECLS

#endif /* JAGCLASSEDITORFACTORY_H_ */
