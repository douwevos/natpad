/*
   File:    armeditorpanel.h
   Project: armadillo
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

#ifndef SRC_EDITOR_ARMEDITORPANEL_H_
#define SRC_EDITOR_ARMEDITORPANEL_H_

#include "armeditorconnector.h"
#include <elk.h>
#include <chameleon.h>
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define ARM_TYPE_EDITOR_PANEL              (arm_editor_panel_get_type())
#define ARM_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), arm_editor_panel_get_type(), ArmEditorPanel))
#define ARM_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ARM_TYPE_EDITOR_PANEL, ArmEditorPanelClass))
#define ARM_IS_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ARM_TYPE_EDITOR_PANEL))
#define ARM_IS_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ARM_TYPE_EDITOR_PANEL))
#define ARM_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ARM_TYPE_EDITOR_PANEL, ArmEditorPanelClass))


typedef struct _ArmEditorPanel               ArmEditorPanel;
typedef struct _ArmEditorPanelPrivate        ArmEditorPanelPrivate;
typedef struct _ArmEditorPanelClass          ArmEditorPanelClass;


struct _ArmEditorPanel {
	ElkEditorPanel parent;
};

struct _ArmEditorPanelClass {
	ElkEditorPanelClass parent_class;
};


GType arm_editor_panel_get_type();

ArmEditorPanel *arm_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, ArmEditorConnector *connector /*, ArmIndexer *indexer */);

G_END_DECLS

#endif /* SRC_EDITOR_ARMEDITORPANEL_H_ */
