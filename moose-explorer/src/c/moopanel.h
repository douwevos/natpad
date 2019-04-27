/*
   File:    moopanel.h
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

#ifndef MOOPANEL_H_
#define MOOPANEL_H_

#include "mooexplorer.h"
#include <leafhopper.h>
#include <caterpillar.h>
#include <moose.h>
#include <viper.h>

G_BEGIN_DECLS

#define MOO_TYPE_PANEL              (moo_panel_get_type())
#define MOO_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_panel_get_type(), MooPanel))
#define MOO_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_PANEL, MooPanelClass))
#define MOO_IS_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_PANEL))
#define MOO_IS_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_PANEL))
#define MOO_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_PANEL, MooPanelClass))

typedef struct _MooPanel               MooPanel;
typedef struct _MooPanelPrivate        MooPanelPrivate;
typedef struct _MooPanelClass          MooPanelClass;

struct _MooPanel {
	LeaPanel parent;
};

struct _MooPanelClass {
	LeaPanelClass parent_class;
};

GType moo_panel_get_type();

MooPanel *moo_panel_new(LeaIPanelOwner *panel_owner, LeaFrame *frame, MooService *moo_service, MooExplorer *explorer);

CatArrayWo *moo_panel_get_selection(MooPanel *panel);

VipSnapshot *moo_panel_get_vip_snapshot(MooPanel *panel);

G_END_DECLS

#endif /* MOOPANEL_H_ */
