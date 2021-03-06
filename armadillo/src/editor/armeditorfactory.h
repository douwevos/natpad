/*
   File:    armeditorfactory.h
   Project: armadillo
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

#ifndef ARMEDITORFACTORY_H_
#define ARMEDITORFACTORY_H_

#include "armeditorconnector.h"
#include <caterpillar.h>
#include <elk.h>
#include <viper.h>
#include <moose.h>

G_BEGIN_DECLS

#define ARM_TYPE_EDITOR_FACTORY              (arm_editor_factory_get_type())
#define ARM_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), arm_editor_factory_get_type(), ArmEditorFactory))
#define ARM_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ARM_TYPE_EDITOR_FACTORY, ArmEditorFactoryClass))
#define ARM_IS_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ARM_TYPE_EDITOR_FACTORY))
#define ARM_IS_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ARM_TYPE_EDITOR_FACTORY))
#define ARM_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ARM_TYPE_EDITOR_FACTORY, ArmEditorFactoryClass))


typedef struct _ArmEditorFactory               ArmEditorFactory;
typedef struct _ArmEditorFactoryPrivate        ArmEditorFactoryPrivate;
typedef struct _ArmEditorFactoryClass          ArmEditorFactoryClass;


struct _ArmEditorFactory {
	GObject parent;
};

struct _ArmEditorFactoryClass {
	GObjectClass parent_class;
};


GType arm_editor_factory_get_type();

ArmEditorFactory *arm_editor_factory_new(LeaIPanelOwner *panel_owner, ArmEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file /*, JagIndexer *indexer */ );

G_END_DECLS

#endif /* ARMEDITORFACTORY_H_ */
