/*
   File:    mooexplorerservice.h
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Nov 24, 2013
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

#ifndef MOOEXPLORERSERVICE_H_
#define MOOEXPLORERSERVICE_H_

#include "moopanelowner.h"
#include <caterpillar.h>
#include <moose.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define MOO_TYPE_EXPLORER_SERVICE              (moo_explorer_service_get_type())
#define MOO_EXPLORER_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_explorer_service_get_type(), MooExplorerService))
#define MOO_EXPLORER_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_EXPLORER_SERVICE, MooExplorerServiceClass))
#define MOO_IS_EXPLORER_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_EXPLORER_SERVICE))
#define MOO_IS_EXPLORER_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_EXPLORER_SERVICE))
#define MOO_EXPLORER_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_EXPLORER_SERVICE, MooExplorerServiceClass))
#define MOO_EXPLORER_SERVICE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_EXPLORER_SERVICE, MooExplorerServicePrivate))


typedef struct _MooExplorerService               MooExplorerService;
typedef struct _MooExplorerServicePrivate        MooExplorerServicePrivate;
typedef struct _MooExplorerServiceClass          MooExplorerServiceClass;


struct _MooExplorerService {
	GObject parent;
	MooExplorerServicePrivate *priv;
};

struct _MooExplorerServiceClass {
	GObjectClass parent_class;
};


GType moo_explorer_service_get_type();

MooExplorerService *moo_explorer_service_new(MooService *moo_service, LeaFrame *frame);

MooPanelOwner *moo_explorer_service_get_panel_owner(MooExplorerService *explorer_service);


G_END_DECLS

#endif /* MOOEXPLORERSERVICE_H_ */
