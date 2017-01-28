/*
   File:    jagsrcfolderpanel.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 31, 2014
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

#ifndef JAGSRCFOLDERPANEL_H_
#define JAGSRCFOLDERPANEL_H_

#include <caterpillar.h>
#include <cow.h>

G_BEGIN_DECLS

#define JAG_TYPE_SRC_FOLDER_PANEL              (jag_src_folder_panel_get_type())
#define JAG_SRC_FOLDER_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_src_folder_panel_get_type(), JagSrcFolderPanel))
#define JAG_SRC_FOLDER_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_SRC_FOLDER_PANEL, JagSrcFolderPanelClass))
#define JAG_IS_SRC_FOLDER_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_SRC_FOLDER_PANEL))
#define JAG_IS_SRC_FOLDER_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_SRC_FOLDER_PANEL))
#define JAG_SRC_FOLDER_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_SRC_FOLDER_PANEL, JagSrcFolderPanelClass))


typedef struct _JagSrcFolderPanel               JagSrcFolderPanel;
typedef struct _JagSrcFolderPanelPrivate        JagSrcFolderPanelPrivate;
typedef struct _JagSrcFolderPanelClass          JagSrcFolderPanelClass;


struct _JagSrcFolderPanel {
	CowPanel parent;
};

struct _JagSrcFolderPanelClass {
	CowPanelClass parent_class;
};


GType jag_src_folder_panel_get_type();

JagSrcFolderPanel *jag_src_folder_panel_new();

G_END_DECLS

#endif /* JAGSRCFOLDERPANEL_H_ */
