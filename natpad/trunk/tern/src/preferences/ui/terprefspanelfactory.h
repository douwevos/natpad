/*
   File:    terprefspanelfactory.h
   Project: tern
   Author:  Douwe Vos
   Date:    Oct 26, 2014
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

#ifndef TERPREFSPANELFACTORY_H_
#define TERPREFSPANELFACTORY_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define TER_TYPE_PREFS_PANEL_FACTORY              (ter_prefs_panel_factory_get_type())
#define TER_PREFS_PANEL_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_prefs_panel_factory_get_type(), TerPrefsPanelFactory))
#define TER_PREFS_PANEL_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_PREFS_PANEL_FACTORY, TerPrefsPanelFactoryClass))
#define TER_IS_PREFS_PANEL_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_PREFS_PANEL_FACTORY))
#define TER_IS_PREFS_PANEL_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_PREFS_PANEL_FACTORY))
#define TER_PREFS_PANEL_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_PREFS_PANEL_FACTORY, TerPrefsPanelFactoryClass))


typedef struct _TerPrefsPanelFactory               TerPrefsPanelFactory;
typedef struct _TerPrefsPanelFactoryPrivate        TerPrefsPanelFactoryPrivate;
typedef struct _TerPrefsPanelFactoryClass          TerPrefsPanelFactoryClass;


struct _TerPrefsPanelFactory {
	GObject parent;
};

struct _TerPrefsPanelFactoryClass {
	GObjectClass parent_class;
};


GType ter_prefs_panel_factory_get_type();

TerPrefsPanelFactory *ter_prefs_panel_factory_new();

G_END_DECLS

#endif /* TERPREFSPANELFACTORY_H_ */
