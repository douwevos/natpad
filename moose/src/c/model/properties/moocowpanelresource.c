/*
   File:    moocowpanelresource.c
   Project: moose
   Author:  Douwe Vos
   Date:    Aug 31, 2014
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


#include "moocowpanelresource.h"
#include "../moonodewo.h"
#include "../mooresourcecontentwo.h"

#include "moocowgladeresource.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooCowPanelResource"
#include <logging/catlog.h>

struct _MooCowPanelResourcePrivate {
	GtkLabel *w_lab_path;
	GtkLabel *w_lab_size;
	GtkLabel *w_lab_last_modified;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooCowPanelResource, moo_cow_panel_resource, COW_TYPE_PANEL,
		G_ADD_PRIVATE(MooCowPanelResource)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);

static void moo_cow_panel_resource_class_init(MooCowPanelResourceClass *clazz) {

	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_cow_panel_resource_init(MooCowPanelResource *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	MooCowPanelResource *instance = MOO_COW_PANEL_RESOURCE(object);
//	MooCowPanelResourcePrivate *priv = moo_cow_panel_resource_get_instance_private(instance);
	G_OBJECT_CLASS(moo_cow_panel_resource_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_cow_panel_resource_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooCowPanelResource *moo_cow_panel_resource_new() {
	MooCowPanelResource *result = g_object_new(MOO_TYPE_COW_PANEL_RESOURCE, NULL);
	cat_ref_anounce(result);
	cow_panel_construct((CowPanel *) result);
	MooCowPanelResourcePrivate *priv = moo_cow_panel_resource_get_instance_private(result);
	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) moocowgladeresource_glade, (gsize) moocowgladeresource_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);


	gtk_container_add((GtkContainer *) result, frame);

	priv->w_lab_path = (GtkLabel *) gtk_builder_get_object(builder, "txtPath");
	priv->w_lab_size = (GtkLabel *) gtk_builder_get_object(builder, "txtSize");
	priv->w_lab_last_modified = (GtkLabel *) gtk_builder_get_object(builder, "txtLastModified");

	return result;
}

static void l_panel_reconfigure(CowPanel *self, GObject *config) {
	MooCowPanelResource *instance = MOO_COW_PANEL_RESOURCE(self);
	MooCowPanelResourcePrivate *priv = moo_cow_panel_resource_get_instance_private(instance);
	MooNodeWo *e_moo_node = MOO_NODE_WO(config);
	if (e_moo_node) {
		MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content((MooNodeWo *) e_moo_node, moo_resource_content_wo_key());
		VipNode *vip_node = (VipNode *) moo_resource_content_wo_get_viper_node(resource_content);
		if (vip_node) {
			VipIResource *resource = vip_node_get_content(vip_node);
			VipPath *path = vip_iresource_path(resource);
			if (path) {
				CatStringWo *a_fpath = vip_path_to_string(path);
				gtk_label_set_text(priv->w_lab_path, cat_string_wo_getchars(a_fpath));
				cat_unref_ptr(a_fpath);
			} else {
				CatStringWo *a_txt = vip_iresource_get_name(resource);
				gtk_label_set_text(priv->w_lab_path, cat_string_wo_getchars(a_txt));
			}

			if (VIP_IS_IFILE(resource)) {
				long long length = vip_ifile_length((VipIFile *) resource);
				CatStringWo *e_flt = cat_string_wo_new();
				cat_string_wo_format(e_flt, "%ld", length);
				gtk_label_set_text(priv->w_lab_size, cat_string_wo_getchars(e_flt));
				cat_unref_ptr(e_flt);
			} else {
				gtk_label_set_text(priv->w_lab_size, "");
			}


			long long lm = vip_iresource_last_modified(resource);
			GDateTime *dt = g_date_time_new_from_unix_local(lm/1000);
			if (dt!=NULL) {
				gchar *dt_chrs = g_date_time_format(dt, "%d-%m-%Y %H:%M:%S");
				gtk_label_set_text(priv->w_lab_last_modified, dt_chrs);
				cat_free_ptr(dt_chrs);
				g_date_time_unref(dt);
			} else {
				gtk_label_set_text(priv->w_lab_last_modified, "-");
			}
		}

	} else {
		gtk_label_set_text(priv->w_lab_path, "");
		gtk_label_set_text(priv->w_lab_size, "");
		gtk_label_set_text(priv->w_lab_last_modified, "");
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

