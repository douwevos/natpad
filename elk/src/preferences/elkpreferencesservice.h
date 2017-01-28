/*
   File:    elkpreferencesservice.h
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 22, 2014
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
#ifndef ELKPREFERENCESSERVICE_H_
#define ELKPREFERENCESSERVICE_H_

#include "elkpreferencescontainer.h"
#include "elkipreferencesmapper.h"
#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define ELK_TYPE_PREFERENCES_SERVICE              (elk_preferences_service_get_type())
#define ELK_PREFERENCES_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_preferences_service_get_type(), ElkPreferencesService))
#define ELK_PREFERENCES_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_PREFERENCES_SERVICE, ElkPreferencesServiceClass))
#define ELK_IS_PREFERENCES_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_PREFERENCES_SERVICE))
#define ELK_IS_PREFERENCES_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_PREFERENCES_SERVICE))
#define ELK_PREFERENCES_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_PREFERENCES_SERVICE, ElkPreferencesServiceClass))


typedef struct _ElkPreferencesService               ElkPreferencesService;
typedef struct _ElkPreferencesServicePrivate        ElkPreferencesServicePrivate;
typedef struct _ElkPreferencesServiceClass          ElkPreferencesServiceClass;


struct _ElkPreferencesService {
	GObject parent;
};

struct _ElkPreferencesServiceClass {
	GObjectClass parent_class;
};


GType elk_preferences_service_get_type();

ElkPreferencesService *elk_preferences_service_new(VipCdProvider *vip_cd_provider);

ElkPreferencesWo *elk_preferences_service_load(ElkPreferencesService *service);
void elk_preferences_service_save(ElkPreferencesService *service);

ElkPreferencesContainer *elk_preferences_service_get_container(ElkPreferencesService *service);

CowPanelModel *elk_preferences_service_get_panel_model(ElkPreferencesService *service);

void elk_preferences_services_add_mapper(ElkPreferencesService *service, ElkIPreferencesMapper *mapper);

VipPath *elk_preferences_services_get_shared_config_map(ElkPreferencesService *service);
VipPath *elk_preferences_services_get_user_config_map(ElkPreferencesService *service);

CowIEntryAccessor *elk_preferences_services_get_chameleon_prefs_accessor(ElkPreferencesService *service);

G_END_DECLS

#endif /* ELKPREFERENCESSERVICE_H_ */
