/*
   File:    draactiontogglemarkoccurrences.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jun 26, 2019
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

#ifndef ACTION_DRAACTIONTOGGLEMARKOCCURRENCES_H_
#define ACTION_DRAACTIONTOGGLEMARKOCCURRENCES_H_

#include "../draeditorpanel.h"
#include "../preferences/drapreferenceswo.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_TOGGLE_MARK_OCCURRENCES              (dra_action_toggle_mark_occurrences_get_type())
#define DRA_ACTION_TOGGLE_MARK_OCCURRENCES(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_toggle_mark_occurrences_get_type(), DraActionToggleMarkOccurrences))
#define DRA_ACTION_TOGGLE_MARK_OCCURRENCES_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_TOGGLE_MARK_OCCURRENCES, DraActionToggleMarkOccurrencesClass))
#define DRA_IS_ACTION_TOGGLE_MARK_OCCURRENCES(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_TOGGLE_MARK_OCCURRENCES))
#define DRA_IS_ACTION_TOGGLE_MARK_OCCURRENCES_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_TOGGLE_MARK_OCCURRENCES))
#define DRA_ACTION_TOGGLE_MARK_OCCURRENCES_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_TOGGLE_MARK_OCCURRENCES, DraActionToggleMarkOccurrencesClass))

struct _DraActionToggleMarkOccurrences {
	LeaAction parent;
};

struct _DraActionToggleMarkOccurrencesClass {
	LeaActionClass parent_class;
};

typedef struct _DraActionToggleMarkOccurrences               DraActionToggleMarkOccurrences;
typedef struct _DraActionToggleMarkOccurrencesPrivate        DraActionToggleMarkOccurrencesPrivate;
typedef struct _DraActionToggleMarkOccurrencesClass          DraActionToggleMarkOccurrencesClass;


GType dra_action_toggle_mark_occurrences_get_type();

DraActionToggleMarkOccurrences *dra_action_toggle_mark_occurrences_new();

void dra_action_toggle_mark_occurrences_set_editor_panel(DraActionToggleMarkOccurrences *action, DraEditorPanel *editor_panel);

void dra_action_toggle_mark_occurrences_reconfigured(DraActionToggleMarkOccurrences *action, DraPreferencesWo *a_prefs);

G_END_DECLS

#endif /* ACTION_DRAACTIONTOGGLEMARKOCCURRENCES_H_ */
