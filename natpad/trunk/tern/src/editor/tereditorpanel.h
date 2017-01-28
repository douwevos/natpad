/*
   File:    tereditorpanel.h
   Project: tern
   Author:  Douwe Vos
   Date:    Jul 14, 2015
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

#ifndef EDITOR_TEREDITORPANEL_H_
#define EDITOR_TEREDITORPANEL_H_

#include "tereditorconnector.h"
#include <caterpillar.h>
#include <elk.h>

G_BEGIN_DECLS

#define TER_TYPE_EDITOR_PANEL              (ter_editor_panel_get_type())
#define TER_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_editor_panel_get_type(), TerEditorPanel))
#define TER_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_EDITOR_PANEL, TerEditorPanelClass))
#define TER_IS_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_EDITOR_PANEL))
#define TER_IS_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_EDITOR_PANEL))
#define TER_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_EDITOR_PANEL, TerEditorPanelClass))


typedef struct _TerEditorPanel               TerEditorPanel;
typedef struct _TerEditorPanelPrivate        TerEditorPanelPrivate;
typedef struct _TerEditorPanelClass          TerEditorPanelClass;


struct _TerEditorPanel {
	ElkEditorPanel parent;
};

struct _TerEditorPanelClass {
	ElkEditorPanelClass parent_class;
};


GType ter_editor_panel_get_type();

TerEditorPanel *ter_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, CatStringWo *a_clazz_name, TerEditorConnector *editor_connector);

//void ter_editor_panel_set_highlighter_map(TerEditorPanel *editor_panel, TerHighlighterMap *highlighter_map);

G_END_DECLS

#endif /* EDITOR_TEREDITORPANEL_H_ */
