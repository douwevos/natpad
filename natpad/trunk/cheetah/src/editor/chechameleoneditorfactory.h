/*
   File:    chechameleoneditorfactory.h
   Project: cheetah
   Author:  Douwe Vos
   Date:    Mar 22, 2015
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

#ifndef CHACHAMELEONEDITORFACTORY_H_
#define CHACHAMELEONEDITORFACTORY_H_

#include <elk.h>
#include <caterpillar.h>
#include <dragonfly.h>
#include <chameleon.h>
#include <leafhopper.h>
#include <viper.h>

G_BEGIN_DECLS

#define CHE_TYPE_CHAMELEON_EDITOR_FACTORY              (che_chameleon_editor_factory_get_type())
#define CHE_CHAMELEON_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), che_chameleon_editor_factory_get_type(), CheChameleonEditorFactory))
#define CHE_CHAMELEON_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHE_TYPE_CHAMELEON_EDITOR_FACTORY, CheChameleonEditorFactoryClass))
#define CHE_IS_CHAMELEON_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHE_TYPE_CHAMELEON_EDITOR_FACTORY))
#define CHE_IS_CHAMELEON_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHE_TYPE_CHAMELEON_EDITOR_FACTORY))
#define CHE_CHAMELEON_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHE_TYPE_CHAMELEON_EDITOR_FACTORY, CheChameleonEditorFactoryClass))


typedef struct _CheChameleonEditorFactory               CheChameleonEditorFactory;
typedef struct _CheChameleonEditorFactoryPrivate        CheChameleonEditorFactoryPrivate;
typedef struct _CheChameleonEditorFactoryClass          CheChameleonEditorFactoryClass;


struct _CheChameleonEditorFactory {
	GObject parent;
};

struct _CheChameleonEditorFactoryClass {
	GObjectClass parent_class;
};


GType che_chameleon_editor_factory_get_type();

CheChameleonEditorFactory *che_chameleon_editor_factory_new(LeaIPanelOwner *panel_owner, DraIConnectorRequestFactory *connector, ElkDocumentIO *document_io, VipIFile *document_file);

G_END_DECLS

#endif /* CHACHAMELEONEDITORFACTORY_H_ */
