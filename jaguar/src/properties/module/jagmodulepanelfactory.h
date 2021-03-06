/*
   File:    jagmodulepanelfactory.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 31, 2014
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

#ifndef JAGMODULEPANELFACTORY_H_
#define JAGMODULEPANELFACTORY_H_

#include "../../preferences/jagpreferenceswo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_MODULE_PANEL_FACTORY              (jag_module_panel_factory_get_type())
#define JAG_MODULE_PANEL_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_module_panel_factory_get_type(), JagModulePanelFactory))
#define JAG_MODULE_PANEL_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_MODULE_PANEL_FACTORY, JagModulePanelFactoryClass))
#define JAG_IS_MODULE_PANEL_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_MODULE_PANEL_FACTORY))
#define JAG_IS_MODULE_PANEL_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_MODULE_PANEL_FACTORY))
#define JAG_MODULE_PANEL_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_MODULE_PANEL_FACTORY, JagModulePanelFactoryClass))


typedef struct _JagModulePanelFactory               JagModulePanelFactory;
typedef struct _JagModulePanelFactoryPrivate        JagModulePanelFactoryPrivate;
typedef struct _JagModulePanelFactoryClass          JagModulePanelFactoryClass;


struct _JagModulePanelFactory {
	GObject parent;
};

struct _JagModulePanelFactoryClass {
	GObjectClass parent_class;
};


GType jag_module_panel_factory_get_type();

JagModulePanelFactory *jag_module_panel_factory_new();

void jag_module_panel_factory_set_preferences(JagModulePanelFactory *factory, JagPreferencesWo *a_jag_prefs);

G_END_DECLS

#endif /* JAGMODULEPANELFACTORY_H_ */
