/*
   File:    leamenuaction.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    2013
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

#ifndef LEAMENUACTION_H_
#define LEAMENUACTION_H_

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "../key/leakeycontext.h"
#include "../action/leaaction.h"
#include "../action/leaactionmenuitem.h"

G_BEGIN_DECLS

#define LEA_TYPE_MENU_ACTION              (lea_menu_action_get_type())
#define LEA_MENU_ACTION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_menu_action_get_type(), LeaMenuAction))
#define LEA_MENU_ACTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_MENU_ACTION, LeaMenuActionClass))
#define LEA_IS_MENU_ACTION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_MENU_ACTION))
#define LEA_IS_MENU_ACTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_MENU_ACTION))
#define LEA_MENU_ACTION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_MENU_ACTION, LeaMenuActionClass))

typedef enum _LeaMenuSetupType            LeaMenuSetupType;

enum _LeaMenuSetupType {
	LEA_MENU_TYPE_BAR,
	LEA_MENU_TYPE_SUB,
	LEA_MENU_TYPE_ITEM
};


typedef struct _LeaMenuAction             LeaMenuAction;
typedef struct _LeaMenuActionClass        LeaMenuActionClass;
typedef struct _LeaMenuActionPrivate      LeaMenuActionPrivate;

struct _LeaMenuAction {
	GObject parent;
};

struct _LeaMenuActionClass {
	GObjectClass parent_class;
};

GType lea_menu_action_get_type(void);

LeaMenuAction *lea_menu_action_new(LeaAction *action, LeaMenuSetupType setupType);

LeaAction *lea_menu_action_get_action(LeaMenuAction *menu_action);

GtkMenuShell *lea_menu_action_get_menu_shell(LeaMenuAction *menu_action);

GtkWidget *lea_menu_action_get_widget(LeaMenuAction *menu_action);

gboolean lea_menu_action_update(LeaMenuAction *action, LeaKeyContext *active_context);

G_END_DECLS

#endif /* LEAMENUACTION_H_ */
