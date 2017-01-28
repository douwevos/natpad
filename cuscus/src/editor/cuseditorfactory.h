/*
   File:    cuseditorfactory.h
   Project: cuscus
   Author:  Douwe Vos
   Date:    May 6, 2016
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

#ifndef EDITOR_CUSEDITORFACTORY_H_
#define EDITOR_CUSEDITORFACTORY_H_

#include <caterpillar.h>
#include <elk.h>
#include <leafhopper.h>
#include <dragonfly.h>
#include <viper.h>
#include <cow.h>

G_BEGIN_DECLS

#define CUS_TYPE_EDITOR_FACTORY              (cus_editor_factory_get_type())
#define CUS_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cus_editor_factory_get_type(), CusEditorFactory))
#define CUS_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CUS_TYPE_EDITOR_FACTORY, CusEditorFactoryClass))
#define CUS_IS_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CUS_TYPE_EDITOR_FACTORY))
#define CUS_IS_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CUS_TYPE_EDITOR_FACTORY))
#define CUS_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CUS_TYPE_EDITOR_FACTORY, CusEditorFactoryClass))


typedef struct _CusEditorFactory               CusEditorFactory;
typedef struct _CusEditorFactoryPrivate        CusEditorFactoryPrivate;
typedef struct _CusEditorFactoryClass          CusEditorFactoryClass;


struct _CusEditorFactory {
	GObject parent;
};

struct _CusEditorFactoryClass {
	GObjectClass parent_class;
};


GType cus_editor_factory_get_type();

CusEditorFactory *cus_editor_factory_new(LeaIPanelOwner *panel_owner, DraHighlighterMap *higlighter_map, ElkDocumentIO *document_io, VipIFile *document_file);

G_END_DECLS

#endif /* EDITOR_CUSEDITORFACTORY_H_ */
