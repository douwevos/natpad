/*
   File:    elkactionabout.c
   Project: elk
   Author:  Douwe Vos
   Date:    Jul 31, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "elkactionabout.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkActionAbout"
#include <logging/catlog.h>

struct _ElkActionAboutPrivate {
	LeaFrame *frame;
	VipService *vip_service;
};

G_DEFINE_TYPE_WITH_PRIVATE(ElkActionAbout, elk_action_about, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void elk_action_about_class_init(ElkActionAboutClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
}

static void elk_action_about_init(ElkActionAbout *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkActionAbout *instance = ELK_ACTION_ABOUT(object);
	ElkActionAboutPrivate *priv = elk_action_about_get_instance_private(instance);
	cat_unref_ptr(priv->frame);
	G_OBJECT_CLASS(elk_action_about_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_action_about_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkActionAbout *elk_action_about_new(LeaFrame *frame, VipService *vip_service) {
	ElkActionAbout *result = g_object_new(ELK_TYPE_ACTION_ABOUT, NULL);
	cat_ref_anounce(result);
	ElkActionAboutPrivate *priv = elk_action_about_get_instance_private(result);
	priv->frame = cat_ref_ptr(frame);
	priv->vip_service = cat_ref_ptr(vip_service);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("elk.about"), cat_string_wo_new_with("_About Natpad"), cat_string_wo_new_with("help-about"));
	return result;
}


//static void l_dump_widget_tree(GtkWidget *widget, void *data);


static void l_action_run(LeaAction *self) {
	ElkActionAboutPrivate *priv = elk_action_about_get_instance_private((ElkActionAbout *) self);

	GtkWidget *w_about = gtk_about_dialog_new();
	GtkAboutDialog *about_dlg = GTK_ABOUT_DIALOG(w_about);

	gtk_window_set_modal(GTK_WINDOW(w_about), TRUE);
	GtkWindow *par_wid = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(priv->frame)));
	if (par_wid) {
		gtk_window_set_transient_for(GTK_WINDOW(w_about), par_wid);
	}

	GdkPixbuf *logo = gdk_pixbuf_new_from_file("/usr/share/pixmaps/natpad_48.xpm", NULL);

	gtk_about_dialog_set_logo(about_dlg, logo);
	gtk_about_dialog_set_program_name(about_dlg, "Natpad");
	gtk_about_dialog_set_version(about_dlg, "2.4");
	gtk_about_dialog_set_website(about_dlg, "http://www.natpad.net");
	gtk_about_dialog_set_comments(about_dlg, "Natpad Text-editor");
	gtk_about_dialog_set_license_type(about_dlg, GTK_LICENSE_GPL_2_0);
	gtk_dialog_run(GTK_DIALOG(w_about));
	gtk_widget_destroy(w_about);


//	vip_service_dump_tree(priv->vip_service);
//	l_dump_widget_tree(GTK_WIDGET(priv->frame), NULL);
}


//static void l_dump_widget_tree(GtkWidget *widget, void *data) {
//	GtkAllocation alloc;
//	gtk_widget_get_allocation(widget, &alloc);
//	gboolean has_window = gtk_widget_get_has_window(widget);
//	cat_log_print("WIDGETS", "alloc[%d,%d,%d,%d], %s, widget:%o", alloc.x, alloc.y,alloc.width,alloc.height, has_window ? "has-window" : "", widget);
//	cat_log_indent_level++;
//	if (GTK_IS_CONTAINER(widget)) {
//		gtk_container_forall(GTK_CONTAINER(widget), l_dump_widget_tree, data);
//	}
//	cat_log_indent_level--;
//}

