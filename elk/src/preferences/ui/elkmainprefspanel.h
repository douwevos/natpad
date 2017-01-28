/*
   File:    elkmainprefspanel.h
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 25, 2014
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
#ifndef ELKMAINPREFSPANEL_H_
#define ELKMAINPREFSPANEL_H_

#include <caterpillar.h>
#include <cow.h>

G_BEGIN_DECLS

#define ELK_TYPE_MAIN_PREFS_PANEL              (elk_main_prefs_panel_get_type())
#define ELK_MAIN_PREFS_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_main_prefs_panel_get_type(), ElkMainPrefsPanel))
#define ELK_MAIN_PREFS_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_MAIN_PREFS_PANEL, ElkMainPrefsPanelClass))
#define ELK_IS_MAIN_PREFS_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_MAIN_PREFS_PANEL))
#define ELK_IS_MAIN_PREFS_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_MAIN_PREFS_PANEL))
#define ELK_MAIN_PREFS_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_MAIN_PREFS_PANEL, ElkMainPrefsPanelClass))


typedef struct _ElkMainPrefsPanel               ElkMainPrefsPanel;
typedef struct _ElkMainPrefsPanelPrivate        ElkMainPrefsPanelPrivate;
typedef struct _ElkMainPrefsPanelClass          ElkMainPrefsPanelClass;


struct _ElkMainPrefsPanel {
	CowPanel parent;
};

struct _ElkMainPrefsPanelClass {
	CowPanelClass parent_class;
};


GType elk_main_prefs_panel_get_type();

ElkMainPrefsPanel *elk_main_prefs_panel_new();

G_END_DECLS

#endif /* ELKMAINPREFSPANEL_H_ */
