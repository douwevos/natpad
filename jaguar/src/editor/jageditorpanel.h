/*
   File:    jageditorpanel.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 27, 2015
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

#ifndef SRC_EDITOR_JAGEDITORPANEL_H_
#define SRC_EDITOR_JAGEDITORPANEL_H_

#include "../indexer/jagindexer.h"
#include "jageditorconnector.h"
#include <elk.h>
#include <chameleon.h>
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_EDITOR_PANEL              (jag_editor_panel_get_type())
#define JAG_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_editor_panel_get_type(), JagEditorPanel))
#define JAG_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_EDITOR_PANEL, JagEditorPanelClass))
#define JAG_IS_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_EDITOR_PANEL))
#define JAG_IS_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_EDITOR_PANEL))
#define JAG_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_EDITOR_PANEL, JagEditorPanelClass))


typedef struct _JagEditorPanel               JagEditorPanel;
typedef struct _JagEditorPanelPrivate        JagEditorPanelPrivate;
typedef struct _JagEditorPanelClass          JagEditorPanelClass;


struct _JagEditorPanel {
	ElkEditorPanel parent;
};

struct _JagEditorPanelClass {
	ElkEditorPanelClass parent_class;
};


GType jag_editor_panel_get_type();

JagEditorPanel *jag_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, JagEditorConnector *connector, JagIndexer *indexer);

G_END_DECLS

#endif /* SRC_EDITOR_JAGEDITORPANEL_H_ */
