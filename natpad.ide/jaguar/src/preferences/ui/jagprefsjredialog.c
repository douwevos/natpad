/*
   File:    jagprefsjredialog.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Oct 12, 2014
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

#include "jagprefsjredialog.h"
#include <gtk/gtk.h>

#include "jaggladeprefsjre.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPrefsJreDialog"
#include <logging/catlog.h>

struct _JagPrefsJreDialogPrivate {
	GtkDialog *dialog;
	VipService *vip_service;
	JagPrefsJreWo *e_jre_wo;

	GtkWidget *w_name;
	gulong s_txt_name;
	GtkWidget *w_path;
	gulong s_txt_path;
	GtkWidget *w_but_select_path;
	GtkWidget *w_tv_jre_libs;

	GtkWidget *w_but_sp_ok;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPrefsJreDialog, jag_prefs_jre_dialog, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPrefsJreDialog)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_prefs_jre_dialog_class_init(JagPrefsJreDialogClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_prefs_jre_dialog_init(JagPrefsJreDialog *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPrefsJreDialog *instance = JAG_PREFS_JRE_DIALOG(object);
//	JagPrefsJreDialogPrivate *priv = jag_prefs_jre_dialog_get_instance_private(instance);
	G_OBJECT_CLASS(jag_prefs_jre_dialog_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_prefs_jre_dialog_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static void l_on_txt_name_changed(GtkEntry *entry, gpointer user_data);
static void l_on_txt_path_changed(GtkEntry *entry, gpointer user_data);
static void l_on_select_path(GtkButton *button, gpointer user_data);
static void l_list_jar_col_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector);
static void l_refresh_from(JagPrefsJreDialog *jre_dialog);


JagPrefsJreDialog *jag_prefs_jre_dialog_new(VipService *vip_service, JagPrefsJreWo *e_jre_wo) {
	JagPrefsJreDialog *result = g_object_new(JAG_TYPE_PREFS_JRE_DIALOG, NULL);
	cat_ref_anounce(result);
	JagPrefsJreDialogPrivate *priv = jag_prefs_jre_dialog_get_instance_private(result);

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) jaggladeprefsjre_glade, (gsize) jaggladeprefsjre_glade_len, (GError **) (&gerror));
	priv->dialog = (GtkDialog *) gtk_builder_get_object(builder, "dialog_main");

	gtk_widget_set_size_request((GtkWidget *) priv->dialog, 450, 480);

	priv->vip_service = cat_ref_ptr(vip_service);
	priv->e_jre_wo = cat_ref_ptr(e_jre_wo);


	priv->w_name = (GtkWidget *) gtk_builder_get_object(builder, "txt_name");
	priv->s_txt_name = g_signal_connect(priv->w_name, "changed", G_CALLBACK(l_on_txt_name_changed), result);

	priv->w_path = (GtkWidget *) gtk_builder_get_object(builder, "txt_path");
	priv->s_txt_path = g_signal_connect(priv->w_path, "changed", G_CALLBACK(l_on_txt_path_changed), result);

	priv->w_but_select_path = (GtkWidget *) gtk_builder_get_object(builder, "but_select_path");
	g_signal_connect(priv->w_but_select_path, "clicked", G_CALLBACK(l_on_select_path), result);

	priv->w_tv_jre_libs = (GtkWidget *) gtk_builder_get_object(builder, "tv_jre_libs");

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_cell_renderer_set_padding(renderer, 0, 0);
	gtk_tree_view_insert_column_with_data_func(GTK_TREE_VIEW(priv->w_tv_jre_libs), 0, "Jar-file",renderer, (GtkTreeCellDataFunc) l_list_jar_col_func, result, NULL);

//	GtkWidget *but_ok = (GtkWidget *) gtk_builder_get_object(builder, "but_ok");

	l_refresh_from(result);

	return result;
}


int jag_prefs_jre_dialog_run(JagPrefsJreDialog *dialog) {
	JagPrefsJreDialogPrivate *priv = jag_prefs_jre_dialog_get_instance_private(dialog);
	return gtk_dialog_run(priv->dialog);
}

void jag_prefs_jre_dialog_close(JagPrefsJreDialog *dialog) {
	JagPrefsJreDialogPrivate *priv = jag_prefs_jre_dialog_get_instance_private(dialog);
	if (priv->dialog) {
		gtk_widget_destroy(GTK_WIDGET(priv->dialog));
		priv->dialog = NULL;
	}
}


static void l_list_jar_col_func(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, void *selector) {
	CatStringWo *jar_name = NULL;
	gtk_tree_model_get(model, iter, 0, &jar_name, -1);
	g_object_set(renderer, "text", cat_string_wo_getchars(jar_name), "weight", PANGO_WEIGHT_NORMAL, "foreground_set", FALSE, NULL);
}


static void l_refresh_from(JagPrefsJreDialog *jre_dialog) {
	JagPrefsJreDialogPrivate *priv = jag_prefs_jre_dialog_get_instance_private(jre_dialog);

	g_signal_handler_block(priv->w_name, priv->s_txt_name);
	gtk_entry_set_text(GTK_ENTRY(priv->w_name), cat_string_wo_getchars(jag_prefs_jre_wo_get_name(priv->e_jre_wo)));
	g_signal_handler_unblock(priv->w_name, priv->s_txt_name);

	g_signal_handler_block(priv->w_path, priv->s_txt_path);
	gtk_entry_set_text(GTK_ENTRY(priv->w_path), cat_string_wo_getchars(jag_prefs_jre_wo_get_path(priv->e_jre_wo)));
	g_signal_handler_unblock(priv->w_path, priv->s_txt_path);

	GtkListStore *list = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(priv->w_tv_jre_libs)));
	gtk_list_store_clear(list);

	CatArrayWo *lib_path_list = jag_prefs_jre_wo_get_library_paths(priv->e_jre_wo);
	if (lib_path_list) {
		GtkTreeIter iter;
		CatIIterator *list_iter = cat_array_wo_iterator(lib_path_list);
		while(cat_iiterator_has_next(list_iter)) {
			CatStringWo *jar_name = (CatStringWo *) cat_iiterator_next(list_iter);
			gtk_list_store_append(list, &iter);
			gtk_list_store_set(list, &iter, 0, jar_name, -1);
		}
		cat_unref_ptr(list_iter);
	}
}




static CatS l_s_txt_java = CAT_S_DEF("java");
#ifdef G_OS_WIN32
static CatS l_s_txt_java_exe = CAT_S_DEF("java.exe");
#endif
static CatS l_s_txt_lib = CAT_S_DEF("lib");
static CatS l_s_txt_jar_ext = CAT_S_DEF(".jar");
static CatS l_s_txt_jre = CAT_S_DEF("jre");
static CatS l_s_txt_bin = CAT_S_DEF("bin");
static CatS l_s_txt_base = CAT_S_DEF("jre/lib/");
static CatS jag_s_txt_version_start = CAT_S_DEF("java version \"");

static gboolean l_extract_from_path(JagPrefsJreWo *e_prefs_jre, VipService *vip_service, CatStringWo *java_home2, gboolean complete_extract) {
	gboolean result = FALSE;
	CatStringWo *a_java_home = cat_string_wo_new_with(cat_string_wo_getchars(java_home2));

	VipPath *java_path = vip_path_new(a_java_home);

//	VipResource *java_bin_resource = NULL;
	VipNodePath *java_bin_np = NULL;


	/* Testing for $JAVA_HOME/jre/bin/java */
	VipPath *java_path_jre = vip_path_create_child(java_path, CAT_S(l_s_txt_jre));
	VipPath *java_path_jre_bin = vip_path_create_child(java_path_jre, CAT_S(l_s_txt_bin));
#ifdef G_OS_WIN32
	VipPath *java_path_jre_bin_java = vip_path_create_child(java_path_jre_bin, CAT_S(l_s_txt_java_exe));
#else
	VipPath *java_path_jre_bin_java = vip_path_create_child(java_path_jre_bin, CAT_S(l_s_txt_java));
#endif
	VipCreatePathRequest *cpreq = vip_service_create_path(vip_service, java_path_jre_bin_java);
	VipNodePath *jre_bin_java_np = vip_create_path_request_wait_for_path(cpreq);
	cat_ref_ptr(jre_bin_java_np);
	VipNode *jre_bin_java_node = vip_node_path_get_tail(jre_bin_java_np);
	cat_unref_ptr(java_path_jre_bin_java);
	cat_unref_ptr(java_path_jre_bin);


	VipIResource *jre_bin_java_content = vip_node_get_content(jre_bin_java_node);
	cat_log_trace("jre_bin_java_content=%o", jre_bin_java_content);
	if (!VIP_IS_IMAP(jre_bin_java_content) && !VIP_IS_VIRTUAL_RESOURCE(jre_bin_java_content)) {
		java_bin_np = jre_bin_java_np;
		jre_bin_java_np = NULL;
	}
//	cat_unref_ptr(jre_bin_java_np);

	if (java_bin_np==NULL) {
		/* Testing for $JAVA_HOME/bin/java */
		VipPath *java_path_bin = vip_path_create_child(java_path, CAT_S(l_s_txt_bin));
#ifdef G_OS_WIN32
		VipPath *java_path_bin_java = vip_path_create_child(java_path_bin, CAT_S(l_s_txt_java_exe));
#else
		VipPath *java_path_bin_java = vip_path_create_child(java_path_bin, CAT_S(l_s_txt_java));
#endif
		VipCreatePathRequest *cpreq2 = vip_service_create_path(vip_service, java_path_bin_java);
		VipNodePath *bin_java_np = vip_create_path_request_wait_for_path(cpreq2);
		cat_ref_ptr(bin_java_np);
		VipNode *bin_java_node = vip_node_path_get_tail(bin_java_np);
		cat_unref_ptr(java_path_bin_java);
		cat_unref_ptr(java_path_bin);


		VipIResource *bin_java_content = vip_node_get_content(bin_java_node);
		cat_log_trace("bin_java_content=%o", bin_java_content);
		if (!VIP_IS_IMAP(bin_java_content) && !VIP_IS_VIRTUAL_RESOURCE(bin_java_content)) {
			java_bin_np = bin_java_np;
			bin_java_np = NULL;
		}
	}

	cat_log_debug("java_bin_np= %o", java_bin_np);
	if (java_bin_np!=NULL) {
		CatStringWo *a_name = CAT_S(l_s_txt_java);
		CatArrayWo *jar_list_wo = NULL;
		if (complete_extract) {
			jar_list_wo = cat_array_wo_new();
			VipPath *java_path_jre_lib = vip_path_create_child(java_path_jre, CAT_S(l_s_txt_lib));

			VipCreatePathRequest *cpreq2 = vip_service_create_path(vip_service, java_path_jre_lib);
			VipNodePath *jre_lib_np = vip_create_path_request_wait_for_path(cpreq2);
			VipNode *jre_lib_node = vip_node_path_get_tail(jre_lib_np);

			cat_unref_ptr(java_path_jre_lib);

			VipIResource *resource_node = vip_node_get_content(jre_lib_node);

			CatArrayWo *e_dir_content = NULL;
			if (VIP_IS_IMAP(resource_node)) {
				e_dir_content = vip_imap_enlist((VipIMap *) resource_node);
			}

			if (e_dir_content) {
				CatStringWo *a_jarext = CAT_S(l_s_txt_jar_ext);
				CatIIterator *map_iter = cat_array_wo_iterator(e_dir_content);
				CatStringWo *a_mapbase = CAT_S(l_s_txt_base);
				while(cat_iiterator_has_next(map_iter)) {
					VipIResource *lib_res = (VipIResource *) cat_iiterator_next(map_iter);
					CatStringWo *a_ename = vip_iresource_get_name(lib_res);
					if (cat_string_wo_endswith(a_ename, a_jarext)) {
						CatStringWo *e_fullename = cat_string_wo_clone(a_mapbase, CAT_CLONE_DEPTH_MAIN);
						cat_string_wo_append(e_fullename, a_ename);
						CatStringWo *a_fullename = cat_string_wo_anchor(e_fullename, 0);
						cat_array_wo_append(jar_list_wo, (GObject *) a_fullename);
						cat_unref_ptr(a_fullename);
					}
				}
				cat_unref_ptr(map_iter);
				cat_unref_ptr(e_dir_content);
			}
			if (cat_array_wo_size(jar_list_wo)==0) {
				cat_unref_ptr(jar_list_wo);
			}

//
//			gboolean is_map;
//			CatArrayWo *e_lib_map = ill_resource_enlist_all(resource, &is_map);
//			CatIIterator *map_iter = cat_array_wo_iterator(lib_map);
//			CatStringWo *a_jarext = cat_string_wo_new_with(".jar");
////			CatStringWo *a_zipext = cat_string_wo_new_with(".zip");
//			CatStringWo *a_mapbase = cat_ref_sink_ptr(cat_string_wo_new_with("jre/lib/"));
//			while(cat_iiterator_has_next(map_iter)) {
//				CatStringWo *a_ename = cat_iiterator_next(map_iter);
//				if (cat_string_wo_length(a_ename)>=4) {
//					CatStringWo *a_ename_extension = cat_ref_sink_ptr(cat_string_wo_substring(ename, cat_string_wo_length(a_ename)-4, 4));
//					if (cat_string_wo_equal(ename_extension, jarext)) {
//						CatStringWo *a_fullename = cat_string_wo_append(mapbase, ename);
//						jar_list = cat_array_wo_append(jar_list, (GObject *) fullename);
//					}
//					cat_unref_ptr(ename_extension);
//				}
//			}
//			cat_unref_ptr(map_iter);
//			if (cat_array_wo_size(a_jar_list)==0) {
//				cat_ref_sink_ptr(jar_list);
//				cat_unref_ptr(jar_list);
//			}
//

			/* lets extract version */
			a_name = NULL;
#ifdef G_OS_UNIX
			CatStringWo *e_java_bin = cat_string_wo_clone(a_java_home, CAT_CLONE_DEPTH_MAIN);
			cat_string_wo_append_chars(e_java_bin, "/bin/java");
			const char *java_bin_chars = cat_string_wo_getchars(e_java_bin);
			cat_log_debug("java_bin_chars=%s", java_bin_chars);
			int pfd[2];
			if (pipe(pfd)!=-1) {
				pid_t pid = fork();
				if (pid==0) {
					/* I'm the child process */
					dup2(pfd[1], STDOUT_FILENO);
					dup2(pfd[1], STDERR_FILENO);
					close(pfd[1]);
					close(pfd[0]);
					execl(java_bin_chars, "-version", "-version", NULL);
					write(STDOUT_FILENO, "x", 1);
					exit(0);
				} else if (pid>0) {
					/* I'm the main process */
					close(pfd[1]);
					char *txt_out = g_malloc(4096);

					int count = read(pfd[0], txt_out, 4096);
					if (count>0) {
						const CatStringWo *a_out_string = cat_string_wo_new_data_len(txt_out, count);
						cat_log_debug("txt_out=%o", a_out_string);
						int vidx = cat_string_wo_index_of_string(a_out_string, CAT_S(jag_s_txt_version_start));
						cat_log_debug("%d", vidx);
						if (vidx>=0) {
							vidx+=cat_string_wo_length(CAT_S(jag_s_txt_version_start));
							int veidx = cat_string_wo_index_of_from(a_out_string, '"', vidx);
							cat_log_debug("vidx=%d, veidx=%d", vidx,veidx);
							if (veidx>=0) {
								a_name = cat_string_wo_new_sub(a_out_string, vidx, veidx-vidx);
								a_name = cat_string_wo_anchor(a_name, 0);
							}
						}
					}
					close(pfd[0]);
				}
			} else {
				cat_log_error("can not create pipe");
			}
			cat_unref_ptr(e_java_bin);
#endif
			if (a_name==NULL) {
				a_name = CAT_S(l_s_txt_java);
			}
		}

		jag_prefs_jre_wo_set_name(e_prefs_jre, a_name);
		cat_unref_ptr(a_name);

		jag_prefs_jre_wo_set_path(e_prefs_jre, java_home2);

		jag_prefs_jre_wo_set_library_paths(e_prefs_jre, jar_list_wo);
		cat_unref_ptr(jar_list_wo);

		result = TRUE;
	}
	cat_unref_ptr(java_path_jre);
	return result;
}



static void l_on_choose_selection_changed(GtkFileChooser *chooser, gpointer user_data) {
	JagPrefsJreDialog *dialog2 = JAG_PREFS_JRE_DIALOG(user_data);
	JagPrefsJreDialogPrivate *priv = jag_prefs_jre_dialog_get_instance_private(dialog2);
	GtkWidget *w_but_ok = priv->w_but_sp_ok;
	gchar *fname = gtk_file_chooser_get_filename(chooser);


	JagPrefsJreWo *e_tmp_prefs_jre = jag_prefs_jre_wo_new();
	CatStringWo *pos_java_home = cat_string_wo_new_with(fname);
	cat_log_debug("fname=%s, pos_java_home=%o", fname, pos_java_home);
	gboolean succesfull_extraction = l_extract_from_path(e_tmp_prefs_jre, priv->vip_service, pos_java_home, FALSE);
	cat_log_debug("succesfull_extraction=%d", succesfull_extraction);
	cat_unref_ptr(pos_java_home);
	gtk_widget_set_sensitive(w_but_ok, succesfull_extraction);
	cat_unref_ptr(e_tmp_prefs_jre);
//	cat_log_debug("fname=%s", fname);
}


static void l_on_select_path(GtkButton *button, gpointer user_data) {
	JagPrefsJreDialog *dialog2 = JAG_PREFS_JRE_DIALOG(user_data);
	JagPrefsJreDialogPrivate *priv = jag_prefs_jre_dialog_get_instance_private(dialog2);

	GtkWindow *top_window = (GtkWindow *) gtk_widget_get_toplevel((GtkWidget *) priv->dialog);
	GtkWidget *dialog = gtk_file_chooser_dialog_new((const gchar *) "Select path of JRE or JDK", top_window,
					(GtkFileChooserAction) GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
					"_Cancel", GTK_RESPONSE_CANCEL,
//					GTK_STOCK_OK, GTK_RESPONSE_OK,
					NULL);
	GtkWidget *w_but_ok = gtk_dialog_add_button(GTK_DIALOG(dialog), "Ok", GTK_RESPONSE_OK);
	gtk_widget_set_sensitive(w_but_ok, FALSE);
//	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), "/home/superman/work/");
	gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(dialog), FALSE);

	GFile *file = g_file_new_for_path("/usr/lib/jvm/");
	if (g_file_query_exists(file, NULL)) {
		gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), "/usr/lib/jvm/");
	}
	g_object_unref(file);

	priv->w_but_sp_ok = w_but_ok;

	g_signal_connect(GTK_FILE_CHOOSER(dialog), "selection-changed", G_CALLBACK(l_on_choose_selection_changed), user_data);

	gtk_widget_show_all(dialog);

	gint response = gtk_dialog_run(GTK_DIALOG(dialog));
	if (response==GTK_RESPONSE_OK) {
		gchar *fname = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(dialog));

		CatStringWo *pos_java_home = cat_string_wo_new_with(fname);
//		gboolean succesfull_extraction =
				l_extract_from_path(priv->e_jre_wo, priv->vip_service, pos_java_home, TRUE);
		cat_unref_ptr(pos_java_home);


		l_refresh_from(dialog2);
	}
	gtk_widget_destroy(dialog);
}




static void l_on_txt_name_changed(GtkEntry *entry, gpointer user_data) {
	JagPrefsJreDialog *dialog = JAG_PREFS_JRE_DIALOG(user_data);
	JagPrefsJreDialogPrivate *priv = jag_prefs_jre_dialog_get_instance_private(dialog);

	CatStringWo *new_name = cat_string_wo_new_with(gtk_entry_get_text(entry));
	jag_prefs_jre_wo_set_name(priv->e_jre_wo, new_name);
	cat_log_debug("after name change: priv->e_jre_wo=%o", priv->e_jre_wo);
}

static void l_on_txt_path_changed(GtkEntry *entry, gpointer user_data) {
	JagPrefsJreDialog *dialog = JAG_PREFS_JRE_DIALOG(user_data);
	JagPrefsJreDialogPrivate *priv = jag_prefs_jre_dialog_get_instance_private(dialog);

	CatStringWo *new_path = cat_string_wo_new_with(gtk_entry_get_text(entry));
	jag_prefs_jre_wo_set_path(priv->e_jre_wo, new_path);
	cat_unref_ptr(new_path);



}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPrefsJreDialog *instance = JAG_PREFS_JRE_DIALOG(self);
	JagPrefsJreDialogPrivate *priv = jag_prefs_jre_dialog_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: e_jre_wo=%o]", iname, self, priv->e_jre_wo);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

