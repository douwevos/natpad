/*
   File:    cuseditorpanel.h
   Project: cuscus
   Author:  Douwe Vos
   Date:    May 7, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef EDITOR_CUSEDITORPANEL_H_
#define EDITOR_CUSEDITORPANEL_H_

#include <elk.h>
#include <chameleon.h>
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define CUS_TYPE_EDITOR_PANEL              (cus_editor_panel_get_type())
#define CUS_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cus_editor_panel_get_type(), CusEditorPanel))
#define CUS_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CUS_TYPE_EDITOR_PANEL, CusEditorPanelClass))
#define CUS_IS_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CUS_TYPE_EDITOR_PANEL))
#define CUS_IS_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CUS_TYPE_EDITOR_PANEL))
#define CUS_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CUS_TYPE_EDITOR_PANEL, CusEditorPanelClass))


typedef struct _CusEditorPanel               CusEditorPanel;
typedef struct _CusEditorPanelPrivate        CusEditorPanelPrivate;
typedef struct _CusEditorPanelClass          CusEditorPanelClass;


struct _CusEditorPanel {
	ElkEditorPanel parent;
};

struct _CusEditorPanelClass {
	ElkEditorPanelClass parent_class;
};


GType cus_editor_panel_get_type();

CusEditorPanel *cus_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin);

G_END_DECLS

#endif /* EDITOR_CUSEDITORPANEL_H_ */
