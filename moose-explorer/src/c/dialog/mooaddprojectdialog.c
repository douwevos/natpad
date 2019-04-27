/*
   File:    mooaddprojectdialog.c
   Project: moose
   Author:  Douwe Vos
   Date:    Nov 24, 2013
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#include "mooaddprojectdialog.h"
#include <string.h>
#include <viper.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooAddProjectDialog"
#include <logging/catlog.h>

struct _MooAddProjectDialogPrivate {
	GtkWidget *entry_base_path;
	gulong enrtry_base_path_id;
	gboolean base_path_modified;
	GtkWidget *entry_name;
	GtkWidget *ok_but;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooAddProjectDialog, moo_add_project_dialog, GTK_TYPE_DIALOG)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_add_project_dialog_class_init(MooAddProjectDialogClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_add_project_dialog_init(MooAddProjectDialog *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	MooAddProjectDialog *instance = MOO_ADD_PROJECT_DIALOG(object);
//	MooAddProjectDialogPrivate *priv = instance->priv;
	G_OBJECT_CLASS(moo_add_project_dialog_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_add_project_dialog_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static void l_name_changed(GtkEntry *entry, gpointer user_data);
static void l_base_path_changed(GtkEntry *entry, gpointer user_data);
static void l_choose_base_dir(MooAddProjectDialog *add_project_dialog);
static void l_button_file(GtkButton *button, gpointer user_data);


MooAddProjectDialog *moo_add_project_dialog_new() {
	MooAddProjectDialog *result = g_object_new(MOO_TYPE_ADD_PROJECT_DIALOG, NULL);
	cat_ref_anounce(result);
	MooAddProjectDialogPrivate *priv = moo_add_project_dialog_get_instance_private(result);
	priv->base_path_modified = FALSE;

	gtk_window_set_modal(GTK_WINDOW(result), TRUE);
	gtk_widget_set_size_request(GTK_WIDGET(result), 600, 150);

	GtkBox *vboxHandle = (GtkBox *) gtk_bin_get_child(GTK_BIN(result));

	GtkWidget *msg = gtk_label_new_with_mnemonic("Add new project");
	gtk_box_pack_start(vboxHandle, msg, TRUE, TRUE, 2);

	GtkWidget *grid = gtk_grid_new();
	gtk_box_pack_start(vboxHandle, grid, TRUE, TRUE, 2);

	GtkWidget *w_label = gtk_label_new_with_mnemonic("Project _name:");

	GValue gvalue = { 0.f, };
	g_value_init( &gvalue, G_TYPE_FLOAT );
	g_value_set_float( &gvalue, 0.0f );
	g_object_set_property( G_OBJECT(w_label), "xalign", &gvalue );
	g_value_unset( &gvalue );

	g_value_init( &gvalue, G_TYPE_FLOAT );
	g_value_set_float( &gvalue, 0.5f );
	g_object_set_property( G_OBJECT(w_label), "yalign", &gvalue );
	g_value_unset( &gvalue );



	gtk_grid_attach((GtkGrid *) grid, w_label,0,0,1,1);

	GtkWidget *entry = gtk_entry_new();
	priv->entry_name = entry;
	gtk_widget_set_can_default(entry, FALSE);


	g_signal_connect(entry, "changed", G_CALLBACK(l_name_changed), result);

	gtk_entry_set_activates_default(GTK_ENTRY(entry), TRUE);

	gtk_grid_attach((GtkGrid *) grid, entry,1,0,2,1);
	gtk_widget_set_hexpand(entry, TRUE);
	gtk_widget_set_margin_start(entry, 6);


	w_label = gtk_label_new_with_mnemonic("_Base directory:");

	g_value_init( &gvalue, G_TYPE_FLOAT );
	g_value_set_float( &gvalue, 0.0f );
	g_object_set_property( G_OBJECT(w_label), "xalign", &gvalue );
	g_value_unset( &gvalue );

	g_value_init( &gvalue, G_TYPE_FLOAT );
	g_value_set_float( &gvalue, 0.5f );
	g_object_set_property( G_OBJECT(w_label), "yalign", &gvalue );
	g_value_unset( &gvalue );


	gtk_grid_attach((GtkGrid *) grid, w_label,0,1,1,1);

	entry = gtk_entry_new();
	gtk_entry_set_activates_default(GTK_ENTRY(entry), TRUE);
	priv->entry_base_path = entry;
	gtk_grid_attach((GtkGrid *) grid, entry,1,1,1,1);
	gtk_widget_set_hexpand(entry, TRUE);
	gtk_widget_set_margin_start(entry, 6);

	priv->enrtry_base_path_id = g_signal_connect(entry, "changed", G_CALLBACK(l_base_path_changed), result);

	entry = gtk_button_new_with_mnemonic("_File...");
	g_signal_connect(entry, "clicked", G_CALLBACK(l_button_file), result);
	gtk_grid_attach((GtkGrid *) grid, entry,2,1,1,1);


	GtkDialog *diag = (GtkDialog *) result;
	gtk_dialog_add_button(diag, "Cancel", 0);
	priv->ok_but = gtk_dialog_add_button(diag, "OK", 1);
	gtk_widget_set_can_default(priv->ok_but, TRUE);
	gtk_widget_grab_default(priv->ok_but);
	gtk_dialog_set_default_response(diag, 1);

	return result;
}


CatStringWo *moo_add_project_dialog_get_module_name(MooAddProjectDialog *add_project_dialog) {
	MooAddProjectDialogPrivate *priv = moo_add_project_dialog_get_instance_private(add_project_dialog);
	const char *name = gtk_entry_get_text((GtkEntry *) priv->entry_name);
	return cat_string_wo_new_with(name);

}

CatStringWo *moo_add_project_dialog_get_base_path(MooAddProjectDialog *add_project_dialog) {
	MooAddProjectDialogPrivate *priv = moo_add_project_dialog_get_instance_private(add_project_dialog);
	const char *base_path = gtk_entry_get_text((GtkEntry *) priv->entry_base_path);
	return cat_string_wo_new_with(base_path);
}


static void l_name_changed(GtkEntry *entry, gpointer user_data) {
//	MooAddProjectDialog *instance = MOO_ADD_PROJECT_DIALOG(user_data);
//	MooAddProjectDialogPrivate *priv = moo_add_project_dialog_get_instance_private(instance);
//	if (!priv->base_path_modified) {
//		const char *name = gtk_entry_get_text((GtkEntry *) priv->entry_name);
//		CatStringWo *a_full_name = pro_configuration_get_workspace_path(priv->pro_configuration);
//		if (full_name) {
//			if (cat_string_wo_char_at(a_full_name, cat_string_wo_length(a_full_name)-1) != '/') {
//				full_name = cat_string_wo_append_unichar(full_name, '/');
//			}
//	//		CatStringWo *a_full_name = cat_string_wo_new_with("/home/superman/work/cpp-workspace2/");
//			full_name = cat_string_wo_append_chars(full_name, name);
//			const char *full_name_chars = cat_string_wo_getchars(full_name);
//			g_signal_handler_block(priv->entry_base_path, priv->enrtry_base_path_id);
//			gtk_entry_set_text((GtkEntry *) priv->entry_base_path, full_name_chars);
//			g_signal_handler_unblock(priv->entry_base_path, priv->enrtry_base_path_id);
//			cat_unref(cat_ref_sink_ptr(full_name));
//		}
//	}
}

static void l_base_path_changed(GtkEntry *entry, gpointer user_data) {
	MooAddProjectDialog *instance = MOO_ADD_PROJECT_DIALOG(user_data);
	MooAddProjectDialogPrivate *priv = moo_add_project_dialog_get_instance_private(instance);
	const char *name = gtk_entry_get_text((GtkEntry *) priv->entry_base_path);
	if (name==NULL || strlen(name)==0) {
		priv->base_path_modified = FALSE;
	} else {
		priv->base_path_modified = TRUE;
	}
}

static void l_choose_base_dir(MooAddProjectDialog *add_project_dialog) {
	MooAddProjectDialogPrivate *priv = moo_add_project_dialog_get_instance_private(add_project_dialog);
	GtkWindow *top_window = (GtkWindow *) gtk_widget_get_toplevel(GTK_WIDGET(add_project_dialog));
	GtkWidget *dialog = gtk_file_chooser_dialog_new((const gchar *) "Select base directory file of project", top_window,
					(GtkFileChooserAction) GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
					"_Cancel", GTK_RESPONSE_CANCEL,
					"_OK", GTK_RESPONSE_OK,
					NULL);
//	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), "/home/superman/work/");
	gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(dialog), FALSE);
	gtk_widget_show_all(dialog);

	gint response = gtk_dialog_run(GTK_DIALOG(dialog));
	if (response==GTK_RESPONSE_OK) {
		gchar *fname = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(dialog));
		cat_log_on_debug({
			gchar *ffolder = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(dialog));
			cat_log_debug("ffolder=%s, fname=%s",ffolder, fname);
		});
		if (fname!=NULL) {
			gtk_entry_set_text((GtkEntry *) priv->entry_base_path, fname);
			const gchar *txt = gtk_entry_get_text((GtkEntry *) priv->entry_name);
			if (txt==NULL || strlen(txt)==0) {
				VipPath *path = vip_path_new(cat_string_wo_new_with(fname));
				CatStringWo *leaf = vip_path_get_leaf(path);
				if (leaf) {
					cat_log_detail("leaf=%o", leaf);
					gtk_entry_set_text((GtkEntry *) priv->entry_name, cat_string_wo_getchars(leaf));
				}
				cat_unref_ptr(path);
			}
		}
	}
	gtk_widget_destroy(dialog);
}

static void l_button_file(GtkButton *button, gpointer user_data) {
	MooAddProjectDialog *new_dialog = MOO_ADD_PROJECT_DIALOG(user_data);
	l_choose_base_dir(new_dialog);
}


int moo_add_project_dialog_run(MooAddProjectDialog *add_project_dialog) {
	gtk_widget_show_all(GTK_WIDGET(add_project_dialog));
	return gtk_dialog_run(GTK_DIALOG(add_project_dialog));
}
