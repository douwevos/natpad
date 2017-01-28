/*
   File:    moocowpanelresource.h
   Project: moose
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

#ifndef MOOCOWPANELRESOURCE_H_
#define MOOCOWPANELRESOURCE_H_

#include <cow.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_COW_PANEL_RESOURCE              (moo_cow_panel_resource_get_type())
#define MOO_COW_PANEL_RESOURCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_cow_panel_resource_get_type(), MooCowPanelResource))
#define MOO_COW_PANEL_RESOURCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_COW_PANEL_RESOURCE, MooCowPanelResourceClass))
#define MOO_IS_COW_PANEL_RESOURCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_COW_PANEL_RESOURCE))
#define MOO_IS_COW_PANEL_RESOURCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_COW_PANEL_RESOURCE))
#define MOO_COW_PANEL_RESOURCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_COW_PANEL_RESOURCE, MooCowPanelResourceClass))


typedef struct _MooCowPanelResource               MooCowPanelResource;
typedef struct _MooCowPanelResourcePrivate        MooCowPanelResourcePrivate;
typedef struct _MooCowPanelResourceClass          MooCowPanelResourceClass;


struct _MooCowPanelResource {
	CowPanel parent;
};

struct _MooCowPanelResourceClass {
	CowPanelClass parent_class;
};


GType moo_cow_panel_resource_get_type();

MooCowPanelResource *moo_cow_panel_resource_new();

G_END_DECLS



#endif /* MOOCOWPANELRESOURCE_H_ */
