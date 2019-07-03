/*
   File:    dragroupmain.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jun 20, 2015
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

#ifndef ACTION_DRAGROUPMAIN_H_
#define ACTION_DRAGROUPMAIN_H_

#include "../draeditorpanel.h"
#include "../drapanelowner.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_GROUP_MAIN              (dra_group_main_get_type())
#define DRA_GROUP_MAIN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_group_main_get_type(), DraGroupMain))
#define DRA_GROUP_MAIN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_GROUP_MAIN, DraGroupMainClass))
#define DRA_IS_GROUP_MAIN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_GROUP_MAIN))
#define DRA_IS_GROUP_MAIN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_GROUP_MAIN))
#define DRA_GROUP_MAIN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_GROUP_MAIN, DraGroupMainClass))


typedef struct _DraGroupMain               DraGroupMain;
typedef struct _DraGroupMainPrivate        DraGroupMainPrivate;
typedef struct _DraGroupMainClass          DraGroupMainClass;


struct _DraGroupMain {
	LeaActionGroup parent;
};

struct _DraGroupMainClass {
	LeaActionGroupClass parent_class;
};


GType dra_group_main_get_type();

DraGroupMain *dra_group_main_new(LeaKeyContext *key_context, DraPanelOwner *panel_owner);

void dra_group_main_set_editor_panel(DraGroupMain *group_main, DraEditorPanel *editor_panel);

void dra_group_main_refresh_clipboard(DraGroupMain *group_main);

void dra_group_main_context_menu(DraGroupMain *group_main, LeaActionGroup *ctx_menu_grp);

LeaAction *dra_group_main_get_action_toggle_word_wrap(DraGroupMain *group_main);
LeaAction *dra_group_main_get_action_toggle_show_whitespace(DraGroupMain *group_main);
LeaAction *dra_group_main_get_action_toggle_mark_occurrences(DraGroupMain *group_main);
LeaAction *dra_group_main_get_action_toggle_spelling(DraGroupMain *group_main);

void dra_group_main_reconfigure(DraGroupMain *group_main, DraPreferencesWo *a_prefs);


G_END_DECLS

#endif /* ACTION_DRAGROUPMAIN_H_ */
