/*
   File:    leapanel.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    6-9-2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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

#ifndef LEAPANEL_H_
#define LEAPANEL_H_

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "../surface/leasurface.h"
#include "leaipanellistener.h"
#include "../key/leakeycontext.h"
#include "../action/leaactiongroup.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_PANEL              (lea_panel_get_type())
#define LEA_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_panel_get_type(), LeaPanel))
#define LEA_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_PANEL, LeaPanelClass))
#define LEA_IS_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_PANEL))
#define LEA_IS_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_PANEL))
#define LEA_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_PANEL, LeaPanelClass))

typedef struct _LeaPanel         LeaPanel;
typedef struct _LeaPanelClass    LeaPanelClass;
typedef struct _LeaPanelPrivate  LeaPanelPrivate;

struct _LeaIPanelOwner;

struct _LeaPanel {
	GtkBin parent;
};

struct _LeaPanelClass {
	GtkBinClass parent_class;
	void (*focusActivated)(LeaPanel *panel, gboolean focus_active);
	void (*focusSet)(LeaPanel *panel, gboolean has_focus);
	gboolean (*close)(LeaPanel *panel);
	void (*closing)(LeaPanel *panel);
};


GType lea_panel_get_type(void);

void lea_panel_construct(LeaPanel *panel, struct _LeaIPanelOwner *panel_owner, gpointer frame, const CatStringWo *c_name, GtkWidget *widget);

LeaPanel *lea_panel_new(struct _LeaIPanelOwner *panel_owner, gpointer frame, const CatStringWo *c_name, GtkWidget *widget);

gboolean lea_panel_get_has_focus(LeaPanel *panel);
void lea_panel_set_has_focus(LeaPanel *panel, gboolean nval);

LeaKeyContext *lea_panel_get_key_context(LeaPanel *panel);

gboolean lea_panel_close(LeaPanel *panel);
gboolean lea_panel_close_real(LeaPanel *panel);

LeaSurface *lea_panel_get_surface(LeaPanel *panel);

void lea_panel_set_order_index(LeaPanel *panel, int order_index);
int lea_panel_get_order_index(LeaPanel *panel);

//void lea_panel_set_last_focus(LeaPanel *panel, GtkWidget *last_focus);
//GtkWidget *lea_panel_get_last_focus(LeaPanel *panel);


struct _LeaIPanelOwner *lea_panel_get_panel_owner(LeaPanel *panel);

const CatStringWo *lea_panel_get_label(const LeaPanel *panel);

void lea_panel_set_label(LeaPanel *panel, const CatStringWo *label);

void lea_panel_request_focus(LeaPanel *panel);
void lea_panel_focus_activated(LeaPanel *panel, gboolean is_active);
gboolean lea_panel_is_active(LeaPanel *panel);

LeaActionGroup *lea_panel_get_action_group(LeaPanel *panel);
void lea_panel_set_action_group(LeaPanel *panel, LeaActionGroup *group);

gboolean lea_panel_equal(LeaPanel *panel_a, LeaPanel *panel_b);
int lea_panel_hash(LeaPanel *panel);

void lea_panel_add_listener(LeaPanel *panel, LeaIPanelListener *listener);
void lea_panel_remove_listener(LeaPanel *panel, LeaIPanelListener *listener);

G_END_DECLS

#endif /* LEAPANEL_H_ */
