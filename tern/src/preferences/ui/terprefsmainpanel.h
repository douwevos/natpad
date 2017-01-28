/*
   File:    terprefsmainpanel.h
   Project: tern
   Author:  Douwe Vos
   Date:    Oct 26, 2014
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

#ifndef TERPREFSMAINPANEL_H_
#define TERPREFSMAINPANEL_H_

#include <caterpillar.h>
#include <cow.h>

G_BEGIN_DECLS

#define TER_TYPE_PREFS_MAIN_PANEL              (ter_prefs_main_panel_get_type())
#define TER_PREFS_MAIN_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_prefs_main_panel_get_type(), TerPrefsMainPanel))
#define TER_PREFS_MAIN_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_PREFS_MAIN_PANEL, TerPrefsMainPanelClass))
#define TER_IS_PREFS_MAIN_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_PREFS_MAIN_PANEL))
#define TER_IS_PREFS_MAIN_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_PREFS_MAIN_PANEL))
#define TER_PREFS_MAIN_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_PREFS_MAIN_PANEL, TerPrefsMainPanelClass))


typedef struct _TerPrefsMainPanel               TerPrefsMainPanel;
typedef struct _TerPrefsMainPanelPrivate        TerPrefsMainPanelPrivate;
typedef struct _TerPrefsMainPanelClass          TerPrefsMainPanelClass;


struct _TerPrefsMainPanel {
	CowPanel parent;
};

struct _TerPrefsMainPanelClass {
	CowPanelClass parent_class;
};


GType ter_prefs_main_panel_get_type();

TerPrefsMainPanel *ter_prefs_main_panel_new();

G_END_DECLS

#endif /* TERPREFSMAINPANEL_H_ */
