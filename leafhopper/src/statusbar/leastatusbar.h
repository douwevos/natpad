/*
   File:    leastatusbar.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Sep 13, 2009
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

#ifndef LEASTATUSBAR_H_
#define LEASTATUSBAR_H_

#include <gdk/gdk.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define LEA_TYPE_STATUS_BAR            (lea_status_bar_get_type())
#define LEA_STATUS_BAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_status_bar_get_type(), LeaStatusBar))
#define LEA_STATUS_BAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_STATUS_BAR, LeaStatusBarClass))
#define LEA_IS_STATUS_BAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_STATUS_BAR))
#define LEA_IS_STATUS_BAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_STATUS_BAR))
#define LEA_STATUS_BAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_STATUS_BAR, LeaStatusBarClass))

typedef struct _LeaStatusBar               LeaStatusBar;
typedef struct _LeaStatusBarPrivate        LeaStatusBarPrivate;
typedef struct _LeaStatusBarClass          LeaStatusBarClass;


struct _LeaStatusBar {
	GtkPaned parent;
};


struct _LeaStatusBarClass {
	GtkPanedClass parent_class;
};


GType lea_status_bar_get_type(void);

LeaStatusBar *lea_status_bar_new();

void lea_status_bar_set_context_widget(LeaStatusBar *status_bar, GtkWidget *widget);

G_END_DECLS

#endif /* LEASTATUSBAR_H_ */
