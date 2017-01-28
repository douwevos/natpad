/*
   File:    groededitorpanel.h
   Project: gross-editor
   Author:  Douwe Vos
   Date:    Oct 12, 2016
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

#ifndef EDITOR_GROEDEDITORPANEL_H_
#define EDITOR_GROEDEDITORPANEL_H_

#include "groededitorconnector.h"
#include <elk.h>
#include <chameleon.h>
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define GROED_TYPE_EDITOR_PANEL              (groed_editor_panel_get_type())
#define GROED_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), groed_editor_panel_get_type(), GroEdEditorPanel))
#define GROED_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GROED_TYPE_EDITOR_PANEL, GroEdEditorPanelClass))
#define GROED_IS_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GROED_TYPE_EDITOR_PANEL))
#define GROED_IS_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GROED_TYPE_EDITOR_PANEL))
#define GROED_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GROED_TYPE_EDITOR_PANEL, GroEdEditorPanelClass))


typedef struct _GroEdEditorPanel               GroEdEditorPanel;
typedef struct _GroEdEditorPanelPrivate        GroEdEditorPanelPrivate;
typedef struct _GroEdEditorPanelClass          GroEdEditorPanelClass;


struct _GroEdEditorPanel {
	ElkEditorPanel parent;
};

struct _GroEdEditorPanelClass {
	ElkEditorPanelClass parent_class;
};


GType groed_editor_panel_get_type();

GroEdEditorPanel *groed_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, GroEdEditorConnector *connector);

G_END_DECLS

#endif /* EDITOR_GROEDEDITORPANEL_H_ */
