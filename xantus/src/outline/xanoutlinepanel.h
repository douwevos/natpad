/*
   File:    xanoutlinepanel.h
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 12, 2017
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

#ifndef OUTLINE_XANOUTLINEPANEL_H_
#define OUTLINE_XANOUTLINEPANEL_H_

#include "../editor/xaneditorconnector.h"
#include <caterpillar.h>
#include <leafhopper.h>
#include <elk.h>

G_BEGIN_DECLS

#define XAN_TYPE_OUTLINE_PANEL              (xan_outline_panel_get_type())
#define XAN_OUTLINE_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), xan_outline_panel_get_type(), XanOutlinePanel))
#define XAN_OUTLINE_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XAN_TYPE_OUTLINE_PANEL, XanOutlinePanelClass))
#define XAN_IS_OUTLINE_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAN_TYPE_OUTLINE_PANEL))
#define XAN_IS_OUTLINE_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XAN_TYPE_OUTLINE_PANEL))
#define XAN_OUTLINE_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XAN_TYPE_OUTLINE_PANEL, XanOutlinePanelClass))


typedef struct _XanOutlinePanel               XanOutlinePanel;
typedef struct _XanOutlinePanelPrivate        XanOutlinePanelPrivate;
typedef struct _XanOutlinePanelClass          XanOutlinePanelClass;


struct _XanOutlinePanel {
	LeaPanel parent;
};

struct _XanOutlinePanelClass {
	LeaPanelClass parent_class;
};


GType xan_outline_panel_get_type();

XanOutlinePanel *xan_outline_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, XanEditorConnector *connector);

G_END_DECLS

#endif /* OUTLINE_XANOUTLINEPANEL_H_ */
