/*
   File:    jageditorfactory.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 25, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#ifndef JAGEDITORFACTORY_H_
#define JAGEDITORFACTORY_H_

#include "jageditorconnector.h"
#include "../indexer/jagindexer.h"
#include <caterpillar.h>
#include <elk.h>
#include <viper.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_EDITOR_FACTORY              (jag_editor_factory_get_type())
#define JAG_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_editor_factory_get_type(), JagEditorFactory))
#define JAG_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_EDITOR_FACTORY, JagEditorFactoryClass))
#define JAG_IS_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_EDITOR_FACTORY))
#define JAG_IS_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_EDITOR_FACTORY))
#define JAG_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_EDITOR_FACTORY, JagEditorFactoryClass))


typedef struct _JagEditorFactory               JagEditorFactory;
typedef struct _JagEditorFactoryPrivate        JagEditorFactoryPrivate;
typedef struct _JagEditorFactoryClass          JagEditorFactoryClass;


struct _JagEditorFactory {
	GObject parent;
};

struct _JagEditorFactoryClass {
	GObjectClass parent_class;
};


GType jag_editor_factory_get_type();

JagEditorFactory *jag_editor_factory_new(LeaIPanelOwner *panel_owner, JagEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file, JagIndexer *indexer);

G_END_DECLS

#endif /* JAGEDITORFACTORY_H_ */
