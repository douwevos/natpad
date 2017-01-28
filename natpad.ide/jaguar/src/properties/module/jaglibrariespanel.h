/*
   File:    jaglibrariespanel.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 5, 2014
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

#ifndef JAGLIBRARIESPANEL_H_
#define JAGLIBRARIESPANEL_H_

#include <cow.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_LIBRARIES_PANEL              (jag_libraries_panel_get_type())
#define JAG_LIBRARIES_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_libraries_panel_get_type(), JagLibrariesPanel))
#define JAG_LIBRARIES_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_LIBRARIES_PANEL, JagLibrariesPanelClass))
#define JAG_IS_LIBRARIES_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_LIBRARIES_PANEL))
#define JAG_IS_LIBRARIES_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_LIBRARIES_PANEL))
#define JAG_LIBRARIES_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_LIBRARIES_PANEL, JagLibrariesPanelClass))


typedef struct _JagLibrariesPanel               JagLibrariesPanel;
typedef struct _JagLibrariesPanelPrivate        JagLibrariesPanelPrivate;
typedef struct _JagLibrariesPanelClass          JagLibrariesPanelClass;


struct _JagLibrariesPanel {
	CowPanel parent;
};

struct _JagLibrariesPanelClass {
	CowPanelClass parent_class;
};


GType jag_libraries_panel_get_type();

JagLibrariesPanel *jag_libraries_panel_new();

G_END_DECLS

#endif /* JAGLIBRARIESPANEL_H_ */
