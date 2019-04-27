/*
   File:    elkcompletionpanel.h
   Project: astarte
   Author:  Douwe Vos
   Date:    Sep 18, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#ifndef ELKCOMPLETIONPANEL_H_
#define ELKCOMPLETIONPANEL_H_

#include <caterpillar.h>
#include <leafhopper.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "elkcompletionentry.h"

G_BEGIN_DECLS

#define ELK_TYPE_COMPLETION_PANEL            (elk_completion_panel_get_type())
#define ELK_COMPLETION_PANEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_completion_panel_get_type(), ElkCompletionPanel))
#define ELK_COMPLETION_PANEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_COMPLETION_PANEL, ElkCompletionPanelClass))
#define ELK_IS_COMPLETION_PANEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_COMPLETION_PANEL))
#define ELK_IS_COMPLETION_PANEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_COMPLETION_PANEL))
#define ELK_COMPLETION_PANEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_COMPLETION_PANEL, ElkCompletionPanelClass))

typedef struct _ElkCompletionPanel       ElkCompletionPanel;
typedef struct _ElkCompletionPanelClass  ElkCompletionPanelClass;


struct _ElkCompletionPanel {
	GtkDrawingArea parent;
	CatArrayWo *a_entries;
	int selected_entry_idx;
	LeaPanel *editor_panel;

	GtkAdjustment *hadjustment;
	GtkAdjustment *vadjustment;

	int hscroll_policy : 1;
	int vscroll_policy : 1;

	int xoffset;
	int yoffset;

	int view_width;
	int view_height;
};


struct _ElkCompletionPanelClass {
	GtkDrawingAreaClass parent_class;
};


GType elk_completion_panel_get_type(void);

ElkCompletionPanel *elk_completion_panel_new(LeaPanel *editor_panel, CatArrayWo *entries);


void elk_completion_panel_set_selected_index(ElkCompletionPanel *panel, int index);

G_END_DECLS

#endif /* ELKCOMPLETIONPANEL_H_ */
