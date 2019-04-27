/*
   File:    moopanelowner.h
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

#ifndef MOOPANELOWNER_H_
#define MOOPANELOWNER_H_

#include "render/moonoderenderregistry.h"
#include <moose.h>
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define MOO_TYPE_PANEL_OWNER              (moo_panel_owner_get_type())
#define MOO_PANEL_OWNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_panel_owner_get_type(), MooPanelOwner))
#define MOO_PANEL_OWNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_PANEL_OWNER, MooPanelOwnerClass))
#define MOO_IS_PANEL_OWNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_PANEL_OWNER))
#define MOO_IS_PANEL_OWNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_PANEL_OWNER))
#define MOO_PANEL_OWNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_PANEL_OWNER, MooPanelOwnerClass))

typedef struct _MooPanelOwner               MooPanelOwner;
typedef struct _MooPanelOwnerPrivate        MooPanelOwnerPrivate;
typedef struct _MooPanelOwnerClass          MooPanelOwnerClass;

struct _MooPanelOwner {
	GObject parent;
};

struct _MooPanelOwnerClass {
	GObjectClass parent_class;
};

GType moo_panel_owner_get_type();

MooPanelOwner *moo_panel_owner_new(MooService *moo_service, LeaFrame *frame);

MooNodeRenderRegistry *moo_panel_owner_get_render_registry(MooPanelOwner *panel_owner);

void moo_panel_owner_update_selection(MooPanelOwner *panel_owner);

G_END_DECLS

#endif /* MOOPANELOWNER_H_ */
