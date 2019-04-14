/*
   File:    leafullmenuitem.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    May 19, 2010
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

#ifndef LEAFULLMENUITEM_H_
#define LEAFULLMENUITEM_H_

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <caterpillar.h>
#include "../key/leakeysequence.h"

G_BEGIN_DECLS

#define LEA_TYPE_FULL_MENU_ITEM            (lea_full_menu_item_get_type())
#define LEA_FULL_MENU_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_full_menu_item_get_type(), LeaFullMenuItem))
#define LEA_FULL_MENU_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_FULL_MENU_ITEM, LeaFullMenuItemClass))
#define LEA_IS_FULL_MENU_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_FULL_MENU_ITEM))
#define LEA_IS_FULL_MENU_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_FULL_MENU_ITEM))
#define LEA_FULL_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_FULL_MENU_ITEM, LeaFullMenuItemClass))


typedef struct _LeaFullMenuItem             LeaFullMenuItem;

typedef struct _LeaFullMenuItemClass        LeaFullMenuItemClass;

struct _LeaFullMenuItem {
	GtkMenuItem parent;
	GtkWidget *label_widget;
	GtkImage *image;
	CatStringWo *a_stock_id;
	LeaKeySequence *key_sequence;
	int key_pixel_width;
	gboolean toggable;
	gboolean is_toggled;
};

struct _LeaFullMenuItemClass {
	GtkMenuItemClass parent_class;
};


GType lea_full_menu_item_get_type(void);

LeaFullMenuItem *lea_full_menu_item_new();

void lea_full_menu_item_construct(LeaFullMenuItem *item);

void lea_full_menu_item_set_key_sequence(LeaFullMenuItem *item, LeaKeySequence *key_sequence);
void lea_full_menu_item_set_stock_id(LeaFullMenuItem *item, CatStringWo *a_stock_id);

void lea_full_menu_item_set_toggable(LeaFullMenuItem *item, gboolean toggable);

gboolean lea_full_menu_item_is_toggled(LeaFullMenuItem *item);
void lea_full_menu_item_set_toggled(LeaFullMenuItem *item, gboolean toggled);


G_END_DECLS

#endif /* LEAFULLMENUITEM_H_ */
