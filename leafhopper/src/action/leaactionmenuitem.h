/*
   File:    leaactionmenuitem.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    2010
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

#ifndef LEAACTIONMENUITEM_H_
#define LEAACTIONMENUITEM_H_

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "leaaction.h"
#include "../widget/leafullmenuitem.h"

G_BEGIN_DECLS

#define LEA_TYPE_ACTION_MENU_ITEM            (lea_action_menu_item_get_type())
#define LEA_ACTION_MENU_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_action_menu_item_get_type(), LeaActionMenuItem))
#define LEA_ACTION_MENU_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_ACTION_MENU_ITEM, LeaActionMenuItemClass))
#define LEA_IS_ACTION_MENU_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_ACTION_MENU_ITEM))
#define LEA_IS_ACTION_MENU_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_ACTION_MENU_ITEM))
#define LEA_ACTION_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_ACTION_MENU_ITEM, LeaActionMenuItemClass))

typedef struct _LeaActionMenuItem             LeaActionMenuItem;
typedef struct _LeaActionMenuItemClass        LeaActionMenuItemClass;

struct _LeaActionMenuItem {
	LeaFullMenuItem parent;
	GtkWidget *image;
	LeaAction *action;
	gboolean disposed;
};

struct _LeaActionMenuItemClass {
	LeaFullMenuItemClass parent_class;
};


GType lea_action_menu_item_get_type(void);

LeaActionMenuItem *lea_action_menu_item_new(LeaAction *action);

G_END_DECLS

#endif /* LEAACTIONMENUITEM_H_ */
