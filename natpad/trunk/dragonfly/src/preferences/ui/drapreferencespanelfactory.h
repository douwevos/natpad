/*
   File:    drapreferencespanelfactory.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jun 25, 2015
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

#ifndef DRAPREFERENCESPANELFACTORY_H_
#define DRAPREFERENCESPANELFACTORY_H_

#include <caterpillar.h>
#include <cow.h>

G_BEGIN_DECLS

#define DRA_TYPE_PREFERENCES_PANEL_FACTORY              (dra_preferences_panel_factory_get_type())
#define DRA_PREFERENCES_PANEL_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_preferences_panel_factory_get_type(), DraPreferencesPanelFactory))
#define DRA_PREFERENCES_PANEL_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_PREFERENCES_PANEL_FACTORY, DraPreferencesPanelFactoryClass))
#define DRA_IS_PREFERENCES_PANEL_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_PREFERENCES_PANEL_FACTORY))
#define DRA_IS_PREFERENCES_PANEL_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_PREFERENCES_PANEL_FACTORY))
#define DRA_PREFERENCES_PANEL_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_PREFERENCES_PANEL_FACTORY, DraPreferencesPanelFactoryClass))


typedef struct _DraPreferencesPanelFactory               DraPreferencesPanelFactory;
typedef struct _DraPreferencesPanelFactoryPrivate        DraPreferencesPanelFactoryPrivate;
typedef struct _DraPreferencesPanelFactoryClass          DraPreferencesPanelFactoryClass;


struct _DraPreferencesPanelFactory {
	GObject parent;
};

struct _DraPreferencesPanelFactoryClass {
	GObjectClass parent_class;
};


GType dra_preferences_panel_factory_get_type();

DraPreferencesPanelFactory *dra_preferences_panel_factory_new(CowIEntryAccessor *entry_extractor);

G_END_DECLS

#endif /* DRAPREFERENCESPANELFACTORY_H_ */
