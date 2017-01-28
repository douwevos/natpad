/*
   File:    draautocompletepanel.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 17, 2015
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

#ifndef AUTOCOMPLETE_DRAAUTOCOMPLETEPANEL_H_
#define AUTOCOMPLETE_DRAAUTOCOMPLETEPANEL_H_

#include "../draeditor.h"
#include "draacentry.h"
#include "draaccontext.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_AUTO_COMPLETE_PANEL              (dra_auto_complete_panel_get_type())
#define DRA_AUTO_COMPLETE_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_auto_complete_panel_get_type(), DraAutoCompletePanel))
#define DRA_AUTO_COMPLETE_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_AUTO_COMPLETE_PANEL, DraAutoCompletePanelClass))
#define DRA_IS_AUTO_COMPLETE_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_AUTO_COMPLETE_PANEL))
#define DRA_IS_AUTO_COMPLETE_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_AUTO_COMPLETE_PANEL))
#define DRA_AUTO_COMPLETE_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_AUTO_COMPLETE_PANEL, DraAutoCompletePanelClass))


typedef struct _DraAutoCompletePanel               DraAutoCompletePanel;
typedef struct _DraAutoCompletePanelPrivate        DraAutoCompletePanelPrivate;
typedef struct _DraAutoCompletePanelClass          DraAutoCompletePanelClass;


struct _DraAutoCompletePanel {
	GtkDrawingArea parent;
};

struct _DraAutoCompletePanelClass {
	GtkDrawingAreaClass parent_class;
};


GType dra_auto_complete_panel_get_type();

DraAutoCompletePanel *dra_auto_complete_panel_new(DraEditor *editor, DraAcContext *ac_context);

void dra_auto_complete_panel_set_selected_index(DraAutoCompletePanel *panel, int index);

gboolean dra_auto_complete_panel_new_revision(DraAutoCompletePanel *panel, ChaRevisionWo *a_new_revision);


G_END_DECLS

#endif /* AUTOCOMPLETE_DRAAUTOCOMPLETEPANEL_H_ */
