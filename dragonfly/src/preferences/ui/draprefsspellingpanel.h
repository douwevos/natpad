/*
   File:    draprefsspellingpanel.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jan 1, 2017
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

#ifndef PREFERENCES_UI_DRAPREFSSPELLINGPANEL_H_
#define PREFERENCES_UI_DRAPREFSSPELLINGPANEL_H_

#include <chameleon.h>
#include <caterpillar.h>
#include <cow.h>
#include "../../spell/draspellhelper.h"

G_BEGIN_DECLS

#define DRA_TYPE_PREFS_SPELLING_PANEL              (dra_prefs_spelling_panel_get_type())
#define DRA_PREFS_SPELLING_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_prefs_spelling_panel_get_type(), DraPrefsSpellingPanel))
#define DRA_PREFS_SPELLING_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_PREFS_SPELLING_PANEL, DraPrefsSpellingPanelClass))
#define DRA_IS_PREFS_SPELLING_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_PREFS_SPELLING_PANEL))
#define DRA_IS_PREFS_SPELLING_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_PREFS_SPELLING_PANEL))
#define DRA_PREFS_SPELLING_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_PREFS_SPELLING_PANEL, DraPrefsSpellingPanelClass))


typedef struct _DraPrefsSpellingPanel               DraPrefsSpellingPanel;
typedef struct _DraPrefsSpellingPanelPrivate        DraPrefsSpellingPanelPrivate;
typedef struct _DraPrefsSpellingPanelClass          DraPrefsSpellingPanelClass;


struct _DraPrefsSpellingPanel {
	CowPanel parent;
};

struct _DraPrefsSpellingPanelClass {
	CowPanelClass parent_class;
};


GType dra_prefs_spelling_panel_get_type();

DraPrefsSpellingPanel *dra_prefs_spelling_panel_new(CowIEntryAccessor *entry_accessor, DraSpellHelper *spell_helper);

G_END_DECLS

#endif /* PREFERENCES_UI_DRAPREFSSPELLINGPANEL_H_ */
