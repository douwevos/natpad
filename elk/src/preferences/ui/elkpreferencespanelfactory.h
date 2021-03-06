/*
   File:    elkpreferencespanelfactory.h
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 26, 2014
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

#ifndef ELKPREFERENCESPANELFACTORY_H_
#define ELKPREFERENCESPANELFACTORY_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_PREFERENCES_PANEL_FACTORY              (elk_preferences_panel_factory_get_type())
#define ELK_PREFERENCES_PANEL_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_preferences_panel_factory_get_type(), ElkPreferencesPanelFactory))
#define ELK_PREFERENCES_PANEL_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_PREFERENCES_PANEL_FACTORY, ElkPreferencesPanelFactoryClass))
#define ELK_IS_PREFERENCES_PANEL_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_PREFERENCES_PANEL_FACTORY))
#define ELK_IS_PREFERENCES_PANEL_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_PREFERENCES_PANEL_FACTORY))
#define ELK_PREFERENCES_PANEL_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_PREFERENCES_PANEL_FACTORY, ElkPreferencesPanelFactoryClass))


typedef struct _ElkPreferencesPanelFactory               ElkPreferencesPanelFactory;
typedef struct _ElkPreferencesPanelFactoryPrivate        ElkPreferencesPanelFactoryPrivate;
typedef struct _ElkPreferencesPanelFactoryClass          ElkPreferencesPanelFactoryClass;


struct _ElkPreferencesPanelFactory {
	GObject parent;
};

struct _ElkPreferencesPanelFactoryClass {
	GObjectClass parent_class;
};


GType elk_preferences_panel_factory_get_type();

ElkPreferencesPanelFactory *elk_preferences_panel_factory_new();

G_END_DECLS


#endif /* ELKPREFERENCESPANELFACTORY_H_ */
