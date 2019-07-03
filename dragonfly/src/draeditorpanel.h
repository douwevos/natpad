/*
   File:    draeditorpanel.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Mar 22, 2015
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

#ifndef DRAEDITORPANEL_H_
#define DRAEDITORPANEL_H_

#include "context/dracontexteditor.h"
#include "draeditor.h"
#include "document/draconnectormap.h"
#include "document/draiconnectorrequestfactory.h"
#include "preferences/drapreferenceswo.h"
#include <leafhopper.h>
#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_EDITOR_PANEL              (dra_editor_panel_get_type())
#define DRA_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_editor_panel_get_type(), DraEditorPanel))
#define DRA_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_EDITOR_PANEL, DraEditorPanelClass))
#define DRA_IS_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_EDITOR_PANEL))
#define DRA_IS_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_EDITOR_PANEL))
#define DRA_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_EDITOR_PANEL, DraEditorPanelClass))


typedef struct _DraEditorPanel               DraEditorPanel;
typedef struct _DraEditorPanelPrivate        DraEditorPanelPrivate;
typedef struct _DraEditorPanelClass          DraEditorPanelClass;

struct _DraEditorPanel {
	LeaPanel parent;
};

struct _DraEditorPanelClass {
	LeaPanelClass parent_class;
	DraEditor *(*initEditor)(DraEditorPanel *editor_panel, ChaDocument *document, DraConnectorMap *connector_map, DraIConnectorRequestFactory *request_factory, WorService *wor_service);
	DraContextEditor *(*createContextEditor)(DraEditorPanel *editor_panel);
	void (*reconfigure)(DraEditorPanel *editor_panel, DraPreferencesWo *a_preferences);
	void (*reactivated)(DraEditorPanel *editor_panel, gboolean focus_active_and_set);

	GtkResponseType (*doSaveRequest)(DraEditorPanel *editor);
	void (*save)(DraEditorPanel *editor, gboolean do_save_as, ChaIOAsync *async_response);
	void (*revert)(DraEditorPanel *editor);
	gboolean (*isSaved)(const DraEditorPanel *editor);

	const CatStringWo *(*getName)(const DraEditorPanel *editor);
};


GType dra_editor_panel_get_type();

void dra_editor_panel_construct(DraEditorPanel *editor_panel, LeaIPanelOwner *panel_owner, ChaDocument *document, DraIConnectorRequestFactory *request_factory);

DraEditorPanel *dra_editor_panel_new(LeaIPanelOwner *panel_owner, ChaDocument *document, DraIConnectorRequestFactory *request_factory);

DraEditor *dra_editor_panel_get_editor(DraEditorPanel *editor_panel);

void dra_editor_panel_reconfigure(DraEditorPanel *editor_panel, DraPreferencesWo *a_prefs);

const CatStringWo *dra_editor_panel_get_name(const DraEditorPanel *editor_panel);

int dra_editor_panel_get_focus_order_index(const DraEditorPanel *editor_panel);
void dra_editor_panel_set_focus_order_index(DraEditorPanel *editor_panel, int focus_order_index);

G_END_DECLS

#endif /* DRAEDITORPANEL_H_ */
