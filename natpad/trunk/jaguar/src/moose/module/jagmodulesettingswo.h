/*
   File:    jagmodulesettingswo.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 6, 2013
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

#ifndef JAGMODULESETTINGSWO_H_
#define JAGMODULESETTINGSWO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_MODULE_SETTINGS_WO              (jag_module_settings_wo_get_type())
#define JAG_MODULE_SETTINGS_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_module_settings_wo_get_type(), JagModuleSettingsWo))
#define JAG_MODULE_SETTINGS_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_MODULE_SETTINGS_WO, JagModuleSettingsWoClass))
#define JAG_IS_MODULE_SETTINGS_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_MODULE_SETTINGS_WO))
#define JAG_IS_MODULE_SETTINGS_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_MODULE_SETTINGS_WO))
#define JAG_MODULE_SETTINGS_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_MODULE_SETTINGS_WO, JagModuleSettingsWoClass))
#define JAG_MODULE_SETTINGS_WO_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_MODULE_SETTINGS_WO, JagModuleSettingsWoPrivate))


typedef struct _JagModuleSettingsWo               JagModuleSettingsWo;
typedef struct _JagModuleSettingsWoPrivate        JagModuleSettingsWoPrivate;
typedef struct _JagModuleSettingsWoClass          JagModuleSettingsWoClass;


struct _JagModuleSettingsWo {
	GObject parent;
	JagModuleSettingsWoPrivate *priv;
};

struct _JagModuleSettingsWoClass {
	GObjectClass parent_class;
};


GType jag_module_settings_wo_get_type();

JagModuleSettingsWo *jag_module_settings_wo_new();

JagModuleSettingsWo *jag_module_settings_wo_ensure_writable(JagModuleSettingsWo *module_settings);
JagModuleSettingsWo *jag_module_settings_wo_anchor(JagModuleSettingsWo *module_settings, int version);
gboolean jag_module_settings_wo_is_fixed(JagModuleSettingsWo *module_settings);
JagModuleSettingsWo *jag_module_settings_wo_get_original(JagModuleSettingsWo *e_module_settings);

CatArrayWo /* <CatArrayWo<CatStringWo>> */ *jag_module_settings_wo_get_source_folders(JagModuleSettingsWo *module_settings);

void jag_module_settings_wo_set_source_folders(JagModuleSettingsWo *e_module_settings, CatArrayWo *new_source_folders);


CatArrayWo *jag_module_settings_wo_get_libraries(JagModuleSettingsWo *module_settings);

void jag_module_settings_wo_set_libraries(JagModuleSettingsWo *e_module_settings, CatArrayWo *new_source_folders);



void jag_module_settings_wo_set_jre_name(JagModuleSettingsWo *e_module_settings, CatStringWo *new_jre_name);

CatStringWo *jag_module_settings_wo_create_flat_path(CatArrayWo *entries);

CatStringWo *jag_module_settings_wo_get_jre_name(JagModuleSettingsWo *module_settings);

G_END_DECLS

#endif /* JAGMODULESETTINGSWO_H_ */
