/*
   File:    drapanelowner.h
   Project: elk
   Author:  Douwe Vos
   Date:    Jun 20, 2015
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

#ifndef DRAPANELOWNER_H_
#define DRAPANELOWNER_H_

#include "draeditorpanel.h"
#include "spell/draspellhelper.h"
#include "document/draconnectormap.h"
#include "macros/dramacromanager.h"
#include <chameleon.h>
#include <caterpillar.h>
#include <leafhopper.h>
#include <worm.h>

G_BEGIN_DECLS

#define DRA_TYPE_PANEL_OWNER              (dra_panel_owner_get_type())
#define DRA_PANEL_OWNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_panel_owner_get_type(), DraPanelOwner))
#define DRA_PANEL_OWNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_PANEL_OWNER, DraPanelOwnerClass))
#define DRA_IS_PANEL_OWNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_PANEL_OWNER))
#define DRA_IS_PANEL_OWNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_PANEL_OWNER))
#define DRA_PANEL_OWNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_PANEL_OWNER, DraPanelOwnerClass))


typedef struct _DraPanelOwner               DraPanelOwner;
typedef struct _DraPanelOwnerPrivate        DraPanelOwnerPrivate;
typedef struct _DraPanelOwnerClass          DraPanelOwnerClass;


struct _DraPanelOwner {
	GObject parent;
};

struct _DraPanelOwnerClass {
	GObjectClass parent_class;
	void (*addPanel)(DraPanelOwner *panel_owner, DraEditorPanel *panel);
	void (*removePanel)(DraPanelOwner *panel_owner, DraEditorPanel *panel);
};


GType dra_panel_owner_get_type();

void dra_panel_owner_construct(DraPanelOwner *panel_owner, LeaFrame *frame, WorService *wor_service);

DraPanelOwner *dra_panel_owner_new(LeaFrame *frame, WorService *wor_service);

void dra_panel_owner_set_configuration(DraPanelOwner *panel_owner, DraPreferencesWo *a_prefs);

CatArrayWo *dra_panel_owner_enlist_all(const DraPanelOwner *panel_owner, CatArrayWo *e_enlist_in);

gboolean dra_panel_owner_close_editors(DraPanelOwner *panel_owner, CatArrayWo *editors_to_close, gboolean close_unsaved);
void dra_panel_owner_save_editors(DraPanelOwner *panel_owner, CatArrayWo *editor_panel_list);

LeaFrame *dra_panel_owner_get_frame(DraPanelOwner *panel_owner);
gboolean dra_panel_owner_get_request_before_close(const DraPanelOwner *panel_owner);

void dra_panel_owner_browse_forward(DraPanelOwner *panel_owner);
void dra_panel_owner_browse_backward(DraPanelOwner *panel_owner);
int dra_panel_owner_stop_browsing(DraPanelOwner *panel_owner);

ChaSearchService *dra_panel_owner_get_search_service(DraPanelOwner *panel_owner);

WorService *dra_panel_owner_get_wor_service(DraPanelOwner *panel_owner);

void dra_panel_owner_refresh_clipboard(DraPanelOwner *panel_owner);

DraMacroManager *dra_panel_owner_get_macro_manager(DraPanelOwner *panel_owner);
DraConnectorMap *dra_panel_owner_get_connector_map(DraPanelOwner *panel_owner);
DraSpellHelper *dra_panel_owner_get_spell_helper(DraPanelOwner *panel_owner);

void dra_panel_owner_setup_context_menu(DraPanelOwner *panel_owner, LeaActionGroup *ctx_menu_grp, DraEditor *editor, ChaCursorWo *cursor);


G_END_DECLS

#endif /* DRAPANELOWNER_H_ */
