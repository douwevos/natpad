/*
   File:    joreditorpanel.h
   Project: jordanella
   Author:  Douwe Vos
   Date:    Feb 6, 2016
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

#ifndef EDITOR_JOREDITORPANEL_H_
#define EDITOR_JOREDITORPANEL_H_

#include "joreditorconnector.h"
#include <elk.h>
#include <chameleon.h>
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JOR_TYPE_EDITOR_PANEL              (jor_editor_panel_get_type())
#define JOR_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jor_editor_panel_get_type(), JorEditorPanel))
#define JOR_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JOR_TYPE_EDITOR_PANEL, JorEditorPanelClass))
#define JOR_IS_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JOR_TYPE_EDITOR_PANEL))
#define JOR_IS_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JOR_TYPE_EDITOR_PANEL))
#define JOR_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JOR_TYPE_EDITOR_PANEL, JorEditorPanelClass))


typedef struct _JorEditorPanel               JorEditorPanel;
typedef struct _JorEditorPanelPrivate        JorEditorPanelPrivate;
typedef struct _JorEditorPanelClass          JorEditorPanelClass;


struct _JorEditorPanel {
	ElkEditorPanel parent;
};

struct _JorEditorPanelClass {
	ElkEditorPanelClass parent_class;
};


GType jor_editor_panel_get_type();


JorEditorPanel *jor_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, JorEditorConnector *connector);

G_END_DECLS

#endif /* EDITOR_JOREDITORPANEL_H_ */
