/*
   File:    leastatusbar.c
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

#include "leastatusbar.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaStatusBar"
#include <logging/catlog.h>

struct _LeaStatusBarPrivate {
	int entry_count;
	GtkWidget *w_context_box;
};

static void l_stringable_iface_init(CatIStringableInterface *iface) {}

G_DEFINE_TYPE_WITH_CODE(LeaStatusBar, lea_status_bar, GTK_TYPE_PANED,
		G_ADD_PRIVATE(LeaStatusBar)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);

static void lea_status_bar_class_init(LeaStatusBarClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void lea_status_bar_init(LeaStatusBar *obj) {
}

static void l_dispose(GObject *object) {
	G_OBJECT_CLASS(lea_status_bar_parent_class)->dispose(object);
//	LeaStatusBar *document_view = LEA_STATUS_BAR(object);
}


LeaStatusBar *lea_status_bar_new() {
	LeaStatusBar *result = g_object_new(LEA_TYPE_STATUS_BAR, NULL);
	LeaStatusBarPrivate *priv = lea_status_bar_get_instance_private(result);
	cat_ref_anounce(result);
	GtkWidget *w_lab1 = gtk_label_new("Natpad 2015");
//	gtk_misc_set_alignment((GtkMisc *) w_lab1, 0, 0.5);
	gtk_label_set_xalign((GtkLabel *) w_lab1, 0);


	priv->w_context_box = gtk_grid_new();

	gtk_widget_set_size_request(priv->w_context_box, 400, 20);

	GtkWidget *w_lab2 = gtk_label_new("label 2");

	gtk_container_add((GtkContainer*) priv->w_context_box, w_lab2);



	gtk_paned_pack1((GtkPaned *) result, w_lab1, TRUE, FALSE);
	gtk_paned_pack2((GtkPaned *) result, priv->w_context_box, FALSE, FALSE);
	return result;
}

void lea_status_bar_set_context_widget(LeaStatusBar *status_bar, GtkWidget *widget) {
	LeaStatusBarPrivate *priv = lea_status_bar_get_instance_private(status_bar);
	GtkWidget *cur_child = gtk_paned_get_child2((GtkPaned *) status_bar);
	if (cur_child==widget) {
		return;
	}
	if (cur_child) {
		gtk_container_remove((GtkContainer*) status_bar, cur_child);
	}
	cat_log_debug("widget=%o", widget);
	if (widget) {
		gtk_paned_pack2((GtkPaned *) status_bar, widget, FALSE, FALSE);
	}
	gtk_widget_show_all((GtkWidget *) status_bar);
}
