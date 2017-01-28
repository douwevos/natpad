/*
   File:    jagprefspanelfactory.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 28, 2014
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

#ifndef JAGPREFSPANELFACTORY_H_
#define JAGPREFSPANELFACTORY_H_

#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_PREFS_PANEL_FACTORY              (jag_prefs_panel_factory_get_type())
#define JAG_PREFS_PANEL_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_prefs_panel_factory_get_type(), JagPrefsPanelFactory))
#define JAG_PREFS_PANEL_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PREFS_PANEL_FACTORY, JagPrefsPanelFactoryClass))
#define JAG_IS_PREFS_PANEL_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PREFS_PANEL_FACTORY))
#define JAG_IS_PREFS_PANEL_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PREFS_PANEL_FACTORY))
#define JAG_PREFS_PANEL_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PREFS_PANEL_FACTORY, JagPrefsPanelFactoryClass))


typedef struct _JagPrefsPanelFactory               JagPrefsPanelFactory;
typedef struct _JagPrefsPanelFactoryPrivate        JagPrefsPanelFactoryPrivate;
typedef struct _JagPrefsPanelFactoryClass          JagPrefsPanelFactoryClass;


struct _JagPrefsPanelFactory {
	GObject parent;
};

struct _JagPrefsPanelFactoryClass {
	GObjectClass parent_class;
};


GType jag_prefs_panel_factory_get_type();

JagPrefsPanelFactory *jag_prefs_panel_factory_new(VipService *vip_service);

G_END_DECLS

#endif /* JAGPREFSPANELFACTORY_H_ */
