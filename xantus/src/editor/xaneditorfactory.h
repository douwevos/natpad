/*
   File:    xaneditorfactory.h
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 10, 2017
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

#ifndef EDITOR_XANEDITORFACTORY_H_
#define EDITOR_XANEDITORFACTORY_H_

#include "xaneditorconnector.h"
#include <elk.h>
#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define XAN_TYPE_EDITOR_FACTORY              (xan_editor_factory_get_type())
#define XAN_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), xan_editor_factory_get_type(), XanEditorFactory))
#define XAN_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XAN_TYPE_EDITOR_FACTORY, XanEditorFactoryClass))
#define XAN_IS_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAN_TYPE_EDITOR_FACTORY))
#define XAN_IS_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XAN_TYPE_EDITOR_FACTORY))
#define XAN_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XAN_TYPE_EDITOR_FACTORY, XanEditorFactoryClass))


typedef struct _XanEditorFactory               XanEditorFactory;
typedef struct _XanEditorFactoryPrivate        XanEditorFactoryPrivate;
typedef struct _XanEditorFactoryClass          XanEditorFactoryClass;


struct _XanEditorFactory {
	GObject parent;
};

struct _XanEditorFactoryClass {
	GObjectClass parent_class;
};


GType xan_editor_factory_get_type();

XanEditorFactory *xan_editor_factory_new(LeaIPanelOwner *panel_owner, XanEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file);

G_END_DECLS

#endif /* EDITOR_XANEDITORFACTORY_H_ */
