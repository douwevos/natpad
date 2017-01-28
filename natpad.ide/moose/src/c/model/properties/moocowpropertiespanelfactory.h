/*
   File:    moocowpropertiespanelfactory.h
   Project: moose
   Author:  Douwe Vos
   Date:    Aug 30, 2014
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

#ifndef MOOCOWPROPERTIESPANELFACTORY_H_
#define MOOCOWPROPERTIESPANELFACTORY_H_

#include "../moonodewo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_COW_PROPERTIES_PANEL_FACTORY              (moo_cow_properties_panel_factory_get_type())
#define MOO_COW_PROPERTIES_PANEL_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_cow_properties_panel_factory_get_type(), MooCowPropertiesPanelFactory))
#define MOO_COW_PROPERTIES_PANEL_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_COW_PROPERTIES_PANEL_FACTORY, MooCowPropertiesPanelFactoryClass))
#define MOO_IS_COW_PROPERTIES_PANEL_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_COW_PROPERTIES_PANEL_FACTORY))
#define MOO_IS_COW_PROPERTIES_PANEL_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_COW_PROPERTIES_PANEL_FACTORY))
#define MOO_COW_PROPERTIES_PANEL_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_COW_PROPERTIES_PANEL_FACTORY, MooCowPropertiesPanelFactoryClass))


typedef struct _MooCowPropertiesPanelFactory               MooCowPropertiesPanelFactory;
typedef struct _MooCowPropertiesPanelFactoryPrivate        MooCowPropertiesPanelFactoryPrivate;
typedef struct _MooCowPropertiesPanelFactoryClass          MooCowPropertiesPanelFactoryClass;


struct _MooCowPropertiesPanelFactory {
	GObject parent;
};

struct _MooCowPropertiesPanelFactoryClass {
	GObjectClass parent_class;
};


GType moo_cow_properties_panel_factory_get_type();

MooCowPropertiesPanelFactory *moo_cow_properties_panel_factory_new();

G_END_DECLS

#endif /* MOOCOWPROPERTIESPANELFACTORY_H_ */
