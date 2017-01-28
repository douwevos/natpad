/*
   File:    cowpanel.h
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 29, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef COWPANEL_H_
#define COWPANEL_H_

#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define COW_TYPE_PANEL              (cow_panel_get_type())
#define COW_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cow_panel_get_type(), CowPanel))
#define COW_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), COW_TYPE_PANEL, CowPanelClass))
#define COW_IS_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), COW_TYPE_PANEL))
#define COW_IS_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), COW_TYPE_PANEL))
#define COW_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), COW_TYPE_PANEL, CowPanelClass))


typedef struct _CowPanel               CowPanel;
typedef struct _CowPanelPrivate        CowPanelPrivate;
typedef struct _CowPanelClass          CowPanelClass;

struct _CowDialog;

struct _CowPanel {
	GtkBin parent;
};

struct _CowPanelClass {
	GtkBinClass parent_class;
	void (*addNotify)(CowPanel *self, struct _CowDialog *dialog);
	void (*removeNotify)(CowPanel *self);
	void (*reconfigure)(CowPanel *self, GObject *config);
};

GType cow_panel_get_type();

void cow_panel_construct(CowPanel *cow_panel);

GObject *cow_panel_get_configuration(CowPanel *panel);
void cow_panel_set_configuration(CowPanel *panel, GObject *configuration);

G_END_DECLS

#endif /* COWPANEL_H_ */
