/*
   File:    draactiontogglespelling.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    1 Jul 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#ifndef ACTION_DRAACTIONTOGGLESPELLING_H_
#define ACTION_DRAACTIONTOGGLESPELLING_H_

#include "../draeditorpanel.h"
#include "../preferences/drapreferenceswo.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_TOGGLE_SPELLING              (dra_action_toggle_spelling_get_type())
#define DRA_ACTION_TOGGLE_SPELLING(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_toggle_spelling_get_type(), DraActionToggleSpelling))
#define DRA_ACTION_TOGGLE_SPELLING_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_TOGGLE_SPELLING, DraActionToggleSpellingClass))
#define DRA_IS_ACTION_TOGGLE_SPELLING(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_TOGGLE_SPELLING))
#define DRA_IS_ACTION_TOGGLE_SPELLING_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_TOGGLE_SPELLING))
#define DRA_ACTION_TOGGLE_SPELLING_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_TOGGLE_SPELLING, DraActionToggleSpellingClass))

struct _DraActionToggleSpelling {
	LeaAction parent;
};

struct _DraActionToggleSpellingClass {
	LeaActionClass parent_class;
};

typedef struct _DraActionToggleSpelling               DraActionToggleSpelling;
typedef struct _DraActionToggleSpellingPrivate        DraActionToggleSpellingPrivate;
typedef struct _DraActionToggleSpellingClass          DraActionToggleSpellingClass;


GType dra_action_toggle_spelling_get_type();

DraActionToggleSpelling *dra_action_toggle_spelling_new();

void dra_action_toggle_spelling_set_editor_panel(DraActionToggleSpelling *action, DraEditorPanel *editor_panel);

void dra_action_toggle_spelling_reconfigured(DraActionToggleSpelling *action, DraPreferencesWo *a_prefs);

G_END_DECLS

#endif /* ACTION_DRAACTIONTOGGLESPELLING_H_ */
