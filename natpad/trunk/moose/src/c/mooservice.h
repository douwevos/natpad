/*
   File:    mooservice.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 10, 2013
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


#ifndef MOOSERVICE_H_
#define MOOSERVICE_H_

#include "model/properties/moocowpropertiescontainer.h"
#include "model/properties/moocowpropertiespanelfactory.h"
#include "transaction/mootransaction.h"
#include "transaction/mootransactiondispatcher.h"
#include <cow.h>
#include <caterpillar.h>
#include <viper.h>
#include <worm.h>

G_BEGIN_DECLS

#define MOO_TYPE_SERVICE              (moo_service_get_type())
#define MOO_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_service_get_type(), MooService))
#define MOO_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_SERVICE, MooServiceClass))
#define MOO_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_SERVICE))
#define MOO_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_SERVICE))
#define MOO_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_SERVICE, MooServiceClass))
#define MOO_SERVICE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_SERVICE, MooServicePrivate))


typedef struct _MooService               MooService;
typedef struct _MooServicePrivate        MooServicePrivate;
typedef struct _MooServiceClass          MooServiceClass;


struct _MooService {
	GObject parent;
	MooServicePrivate *priv;
};

struct _MooServiceClass {
	GObjectClass parent_class;
};


GType moo_service_get_type();

MooService *moo_service_new(WorService *wor_service, VipService *vip_service);

MooTransaction *moo_service_create_transaction(GObject *owner, MooService *moo_service);

gboolean moo_service_add_node(MooService *moo_service, MooNodeWo *module);

//gboolean moo_service_add_module(MooService *moo_service, VipPath *module_base_path, CatStringWo *a_module_name);

MooNodeWo *moo_service_get_root_node_ref(MooService *moo_service);

MooTransactionDispatcher *moo_service_get_transaction_dispatcher(MooService *moo_service);

WorService *moo_service_get_worm_service(MooService *moo_service);

VipService *moo_service_get_viper_service(MooService *moo_service);

MooCowPropertiesContainer *moo_service_get_cow_properties_container(MooService *moo_service);

CowPanelModel *moo_service_get_properties_panel_model(MooService *moo_service);

G_END_DECLS

#endif /* MOOSERVICE_H_ */
