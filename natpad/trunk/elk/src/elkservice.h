/*
   File:    elkservice.h
   Project: elk
   Author:  Douwe Vos
   Date:    Apr 11, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#ifndef ELKSERVICE_H_
#define ELKSERVICE_H_

#include <caterpillar.h>
#include <leafhopper.h>
#include <chameleon.h>
#include <viper.h>
#include <moose.h>
#include <dragonfly.h>
#include <mooseexplorer.h>

#include "dialog/elkdialogs.h"
#include "editor/elkpanelowner.h"
#include "preferences/elkpreferencesservice.h"
#include "editor/elkiresourcehandler.h"
#include "action/elkgroupmain.h"

#include "documentio/elkdocumentio.h"


G_BEGIN_DECLS

#define ELK_TYPE_SERVICE            (elk_service_get_type())
#define ELK_SERVICE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_service_get_type(), ElkService))
#define ELK_SERVICE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_SERVICE, ElkServiceClass))
#define ELK_IS_SERVICE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_SERVICE))
#define ELK_IS_SERVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_SERVICE))
#define ELK_SERVICE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_SERVICE, ElkServiceClass))


typedef struct _ElkService	   		ElkService;
typedef struct _ElkServiceClass	  	ElkServiceClass;


struct _ElkService {
	GObject parent;
	LeaFrame *frame;

	ElkPanelOwner *panel_owner;

	VipService *vip_service;

	MooService *moo_service;
	MooExplorerService *moo_explorer_service;

	ElkDocumentIO *document_io;

	CatArrayWo *e_resource_handlers;

	ElkPreferencesService *preferences_service;

	ElkDialogs *dialogs;
};

struct _ElkServiceClass {
	GObjectClass parent_class;
};


GType elk_service_get_type();

ElkService *elk_service_new(LeaFrame *frame, MooService *moo_service, ElkPreferencesService *prefences_service);


void elk_service_add_resource_handler(ElkService *service, ElkIResourceHandler *resource_handler);

void elk_service_remove_resource_handler(ElkService *service, ElkIResourceHandler *resource_handler);

//ElkIResourceHandler *elk_service_match_resource_handler(ElkService *service, VipResource *resource);


ElkPreferencesService *elk_service_get_preferences_service(ElkService *service);

G_END_DECLS

#endif /* ELKSERVICE_H_ */
