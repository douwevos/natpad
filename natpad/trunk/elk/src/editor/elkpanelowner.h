/*
   File:    elkpanelowner.h
   Project: elk
   Author:  Douwe Vos
   Date:    Aug 29, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef EDITOR_ELKPANELOWNER_H_
#define EDITOR_ELKPANELOWNER_H_

#include "../dialog/elkdialogs.h"
#include "../elkiservice.h"
#include <leafhopper.h>
#include <viper.h>
#include <moose.h>
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_PANEL_OWNER              (elk_panel_owner_get_type())
#define ELK_PANEL_OWNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_panel_owner_get_type(), ElkPanelOwner))
#define ELK_PANEL_OWNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_PANEL_OWNER, ElkPanelOwnerClass))
#define ELK_IS_PANEL_OWNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_PANEL_OWNER))
#define ELK_IS_PANEL_OWNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_PANEL_OWNER))
#define ELK_PANEL_OWNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_PANEL_OWNER, ElkPanelOwnerClass))


typedef struct _ElkPanelOwner               ElkPanelOwner;
typedef struct _ElkPanelOwnerPrivate        ElkPanelOwnerPrivate;
typedef struct _ElkPanelOwnerClass          ElkPanelOwnerClass;


struct _ElkPanelOwner {
	DraPanelOwner parent;
};

struct _ElkPanelOwnerClass {
	DraPanelOwnerClass parent_class;
};


GType elk_panel_owner_get_type();

ElkPanelOwner *elk_panel_owner_new(ElkIService *elk_service, MooService *moo_service, VipService *vip_service, LeaFrame *frame);

ElkDialogs *elk_panel_owner_get_dialogs(ElkPanelOwner *panel_owner);

MooService *elk_panel_owner_get_moose_service(ElkPanelOwner *panel_owner);

G_END_DECLS

#endif /* EDITOR_ELKPANELOWNER_H_ */
