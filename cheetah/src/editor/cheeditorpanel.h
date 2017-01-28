/*
   File:    cheeditorpanel.h
   Project: cheetah
   Author:  Douwe Vos
   Date:    Jun 30, 2015
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

#ifndef CHEEDITORPANEL_H_
#define CHEEDITORPANEL_H_

#include <elk.h>
#include <cow.h>
#include <dragonfly.h>
#include <leafhopper.h>
#include <chameleon.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_EDITOR_PANEL              (che_editor_panel_get_type())
#define CHA_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), che_editor_panel_get_type(), CheEditorPanel))
#define CHA_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_EDITOR_PANEL, CheEditorPanelClass))
#define CHA_IS_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_EDITOR_PANEL))
#define CHA_IS_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_EDITOR_PANEL))
#define CHA_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_EDITOR_PANEL, CheEditorPanelClass))


typedef struct _CheEditorPanel               CheEditorPanel;
typedef struct _CheEditorPanelPrivate        CheEditorPanelPrivate;
typedef struct _CheEditorPanelClass          CheEditorPanelClass;


struct _CheEditorPanel {
	ElkEditorPanel parent;
};

struct _CheEditorPanelClass {
	ElkEditorPanelClass parent_class;
};


GType che_editor_panel_get_type();

CheEditorPanel *che_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, DraIConnectorRequestFactory *connector);

G_END_DECLS

#endif /* CHEEDITORPANEL_H_ */
