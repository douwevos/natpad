/*
   File:    draprefsbehavourpanel.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jan 22, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef PREFERENCES_UI_DRAPREFSBEHAVIOURPANEL_H_
#define PREFERENCES_UI_DRAPREFSBEHAVIOURPANEL_H_

#include <caterpillar.h>
#include <chameleon.h>
#include <cow.h>

G_BEGIN_DECLS

#define DRA_TYPE_PREFS_BEHAVIOUR_PANEL              (dra_prefs_behaviour_panel_get_type())
#define DRA_PREFS_BEHAVIOUR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_prefs_behaviour_panel_get_type(), DraPrefsBehaviourPanel))
#define DRA_PREFS_BEHAVIOUR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_PREFS_BEHAVIOUR_PANEL, DraPrefsBehaviourPanelClass))
#define DRA_IS_PREFS_BEHAVIOUR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_PREFS_BEHAVIOUR_PANEL))
#define DRA_IS_PREFS_BEHAVIOUR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_PREFS_BEHAVIOUR_PANEL))
#define DRA_PREFS_BEHAVIOUR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_PREFS_BEHAVIOUR_PANEL, DraPrefsBehaviourPanelClass))


typedef struct _DraPrefsBehaviourPanel               DraPrefsBehaviourPanel;
typedef struct _DraPrefsBehaviourPanelPrivate        DraPrefsBehaviourPanelPrivate;
typedef struct _DraPrefsBehaviourPanelClass          DraPrefsBehaviourPanelClass;


struct _DraPrefsBehaviourPanel {
	CowPanel parent;
};

struct _DraPrefsBehaviourPanelClass {
	CowPanelClass parent_class;
};


GType dra_prefs_behaviour_panel_get_type();

DraPrefsBehaviourPanel *dra_prefs_behaviour_panel_new(CowIEntryAccessor *entry_accessor);

G_END_DECLS

#endif /* PREFERENCES_UI_DRAPREFSBEHAVIOURPANEL_H_ */
