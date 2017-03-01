/*
   File:    xaneditorpanel.h
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 10, 2017
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

#ifndef EDITOR_XANEDITORPANEL_H_
#define EDITOR_XANEDITORPANEL_H_

#include "xaneditorconnector.h"
#include <elk.h>
#include <chameleon.h>
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define XAN_TYPE_EDITOR_PANEL              (xan_editor_panel_get_type())
#define XAN_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), xan_editor_panel_get_type(), XanEditorPanel))
#define XAN_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XAN_TYPE_EDITOR_PANEL, XanEditorPanelClass))
#define XAN_IS_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAN_TYPE_EDITOR_PANEL))
#define XAN_IS_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XAN_TYPE_EDITOR_PANEL))
#define XAN_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XAN_TYPE_EDITOR_PANEL, XanEditorPanelClass))


typedef struct _XanEditorPanel               XanEditorPanel;
typedef struct _XanEditorPanelPrivate        XanEditorPanelPrivate;
typedef struct _XanEditorPanelClass          XanEditorPanelClass;


struct _XanEditorPanel {
	ElkEditorPanel parent;
};

struct _XanEditorPanelClass {
	ElkEditorPanelClass parent_class;
};


GType xan_editor_panel_get_type();

XanEditorPanel *xan_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, XanEditorConnector *connector);

G_END_DECLS

#endif /* EDITOR_XANEDITORPANEL_H_ */
