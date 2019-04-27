/*
   File:    mooclipboard.c
   Project: monkey-explorer
   Author:  Douwe Vos
   Date:    Jul 19, 2012
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

#include "mooclipboard.h"
#include "../moonodelayout.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooClipboard"
#include <logging/catlog.h>

struct _MooClipboardPrivate {
	LeaFrame *frame;
	MooService *moo_service;
	GdkAtom copied_files_atom;
	GdkAtom mate_copied_files_atom;
	GdkAtom copied_resource_atom;
	CatArrayWo *clipboard_nodes;

	gboolean is_copy_set;

	CatWeakList *listeners;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooClipboard, moo_clipboard, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static gboolean l_targets_include(MooClipboard *moo_clipboard, GdkAtom *targets, gint n_targets, GdkAtom requested_target);

static void moo_clipboard_class_init(MooClipboardClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_clipboard_init(MooClipboard *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooClipboard *instance = MOO_CLIPBOARD(object);
	MooClipboardPrivate *priv = moo_clipboard_get_instance_private(instance);
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->frame);
	cat_unref_ptr(priv->clipboard_nodes);
	cat_unref_ptr(priv->listeners);
	G_OBJECT_CLASS(moo_clipboard_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_clipboard_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MooClipboard *moo_clipboard_new(MooService *moo_service, LeaFrame *frame) {
	MooClipboard *result = g_object_new(MOO_TYPE_CLIPBOARD, NULL);
	cat_ref_anounce(result);
	MooClipboardPrivate *priv = moo_clipboard_get_instance_private(result);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->frame = cat_ref_ptr(frame);
	priv->clipboard_nodes = NULL;
	priv->copied_files_atom = gdk_atom_intern("x-special/gnome-copied-files", FALSE);
	priv->mate_copied_files_atom = gdk_atom_intern ("x-special/mate-copied-files", FALSE);
	priv->copied_resource_atom = gdk_atom_intern("natpad/copied-resource", FALSE);
	priv->listeners = cat_weak_list_new();
	return result;
}


static void l_get_clipboard(GtkClipboard *clipboard, GtkSelectionData *selection_data, guint info, gpointer user_data) {
	MooClipboard *instance = MOO_CLIPBOARD(user_data);
	MooClipboardPrivate *priv = moo_clipboard_get_instance_private(instance);
	if (priv->clipboard_nodes == NULL) {
		return;
	}
	CatIIterator *iter = cat_array_wo_iterator(priv->clipboard_nodes);
	CatArrayWo *uris = cat_array_wo_new();
	while(cat_iiterator_has_next(iter)) {
		MooNodeLayout *node_layout = (MooNodeLayout *) cat_iiterator_next(iter);
		MooNodeWo *node = moo_node_layout_get_node(node_layout);
		MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
		if (resource_content) {
			VipNode *vip_node = moo_resource_content_wo_get_viper_node(resource_content);
			if (vip_node) {
				VipIResource *content = vip_node_get_content(vip_node);
				if (VIP_IS_FS_FILE(content)) {
					VipFSFile *vip_fs_file = VIP_FS_FILE(content);
					VipPath *path = vip_fs_file_get_path(vip_fs_file);
					CatStringWo *uri = vip_path_as_uri(path);
					cat_array_wo_append(uris, (GObject *) uri);
					cat_unref_ptr(uri);
				} else if (VIP_IS_FS_MAP(content)) {
					VipFSMap *vip_fs_map = VIP_FS_MAP(content);
					VipPath *path = vip_fs_map_get_path(vip_fs_map);
					CatStringWo *uri = vip_path_as_uri(path);
					cat_array_wo_append(uris, (GObject *) uri);
					cat_unref_ptr(uri);
				} else {
					cat_unref_ptr(uris);
					break;
				}
			}
		}
	}
	cat_unref_ptr(iter);

	GdkAtom target = gtk_selection_data_get_target(selection_data);
	cat_log_debug("uris=%o", uris);
	if (uris) {
		if (gtk_targets_include_uri(&target, 1)) {
			cat_log_debug("request uris");
			char **flat_uris = g_malloc((1+cat_array_wo_size(uris))*sizeof(char *));
			int idx = 0;
			iter = cat_array_wo_iterator(uris);
			while(cat_iiterator_has_next(iter)) {
				CatStringWo *uri = (CatStringWo *) cat_iiterator_next(iter);
				flat_uris[idx] = strdup(cat_string_wo_getchars(uri));
				idx++;
			}
			cat_unref_ptr(iter);
			flat_uris[idx] = NULL;
			idx++;
			gtk_selection_data_set_uris(selection_data, flat_uris);
			g_strfreev(flat_uris);
		} else if (target == priv->copied_files_atom || target == priv->mate_copied_files_atom) {
			cat_log_debug("request file_atom");
			CatStringWo *buf = cat_string_wo_new_with(priv->is_copy_set ? "copy" : "cut");
			iter = cat_array_wo_iterator(uris);
			while(cat_iiterator_has_next(iter)) {
				CatStringWo *uri = (CatStringWo *) cat_iiterator_next(iter);
				cat_string_wo_append_char(buf, '\n');
				cat_string_wo_append(buf, uri);
			}
			cat_unref_ptr(iter);
			gtk_selection_data_set(selection_data, target, 8, (const guchar *) cat_string_wo_getchars(buf), (gint) cat_string_wo_length(buf));
			cat_unref_ptr(buf);
		} else if (gtk_targets_include_text(&target, 1)) {
			cat_log_debug("request text");
			CatStringWo *buf = cat_string_wo_new_with(priv->is_copy_set ? "copy" : "cut");
			iter = cat_array_wo_iterator(uris);
			while(cat_iiterator_has_next(iter)) {
				CatStringWo *uri = (CatStringWo *) cat_iiterator_next(iter);
				cat_string_wo_append_char(buf, '\n');
				cat_string_wo_append(buf, uri);
			}
			cat_unref_ptr(iter);
			gtk_selection_data_set_text(selection_data, cat_string_wo_getchars(buf), cat_string_wo_length(buf));
			cat_unref_ptr(buf);
		}
	} else {
		if (target == priv->copied_resource_atom) {

		} else if (gtk_targets_include_text(&target, 1)) {
			CatStringWo *buf = cat_string_wo_new_with(priv->is_copy_set ? "natpad-copy" : "natpad-cut");
			iter = cat_array_wo_iterator(uris);
			while(cat_iiterator_has_next(iter)) {
				CatStringWo *uri = (CatStringWo *) cat_iiterator_next(iter);
				cat_string_wo_append_char(buf, '\n');
				cat_string_wo_append(buf, uri);
			}
			cat_unref_ptr(iter);
			gtk_selection_data_set_text(selection_data, cat_string_wo_getchars(buf), cat_string_wo_length(buf));
			cat_unref_ptr(buf);
		}
	}
	cat_unref_ptr(uris);
}


static void l_clear_clipboard(GtkClipboard *clipboard, gpointer user_data) {
	MooClipboard *instance = MOO_CLIPBOARD(user_data);
	MooClipboardPrivate *priv = moo_clipboard_get_instance_private(instance);
	cat_unref_ptr(priv->clipboard_nodes);
}


GtkClipboard *moo_clipboard_get_gtk_clipboard(MooClipboard *moo_clipboard) {
	MooClipboardPrivate *priv = moo_clipboard_get_instance_private(moo_clipboard);
	return gtk_clipboard_get_for_display(gtk_widget_get_display(GTK_WIDGET(priv->frame)), GDK_SELECTION_CLIPBOARD);
}

void moo_clipboard_cut_or_copy(MooClipboard *moo_clipboard, CatArrayWo *selected_nodes, gboolean is_copy) {
	cat_log_debug("clipboard_nodes=%o, is_copy=%d", selected_nodes, is_copy);
	MooClipboardPrivate *priv = moo_clipboard_get_instance_private(moo_clipboard);
	guint n_targets;
	GtkTargetList *target_list = gtk_target_list_new (NULL, 0);
	gtk_target_list_add(target_list, priv->mate_copied_files_atom, 0, 0);
	gtk_target_list_add(target_list, priv->copied_files_atom, 0, 0);
	gtk_target_list_add(target_list, priv->copied_resource_atom, 0, 0);
    gtk_target_list_add_uri_targets(target_list, 0);
    gtk_target_list_add_text_targets(target_list, 0);
    GtkTargetEntry *targets = gtk_target_table_new_from_list(target_list, (gint *) (&n_targets));
    gtk_target_list_unref (target_list);

    cat_ref_swap(priv->clipboard_nodes, selected_nodes);
    priv->is_copy_set = is_copy;

    GtkClipboard *display_clipboard = gtk_clipboard_get_for_display(gtk_widget_get_display(GTK_WIDGET(priv->frame)), GDK_SELECTION_CLIPBOARD);
//    gboolean result =
    		gtk_clipboard_set_with_data(display_clipboard, targets, n_targets, (GtkClipboardGetFunc) l_get_clipboard, (GtkClipboardClearFunc) l_clear_clipboard, moo_clipboard);
//    cat_log_debug("clipse-board-set=%s", result ? "yes" : "no");
}


struct _MooPasteInfo {
	MooClipboard *moo_clipboard;
	CatReadableTreeNode *destination_vip;
};

static CatS l_s_txt_cut = CAT_S_DEF("cut");

static void l_paste_clipboard_received(GtkClipboard *clipboard, GtkSelectionData *selection_data, gpointer data) {
	struct _MooPasteInfo *paste_info = (struct _MooPasteInfo *) data;
	MooClipboard *moo_clipboard = paste_info->moo_clipboard;
	MooClipboardPrivate *priv = moo_clipboard_get_instance_private(moo_clipboard);
	CatReadableTreeNode *destination_vip = paste_info->destination_vip;
	VipNode *dest_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) destination_vip);
	VipIResource *dest_resource = vip_node_get_content(dest_vip_node);
	if (VIP_IS_FS_MAP(dest_resource)) {

	}

	if (gtk_selection_data_get_length(selection_data)>0) {
		const CatStringWo *copy_string = cat_string_wo_new_data((const gchar *) gtk_selection_data_get_data(selection_data));

		CatArrayWo *paths_to_copy = cat_array_wo_new();
		CatArrayWo *copy_lines = cat_string_wo_split(copy_string, '\n', FALSE);
		CatIIterator *iter = cat_array_wo_iterator(copy_lines);
		int idx=0;
		gboolean is_cut = FALSE;
		while(cat_iiterator_has_next(iter)) {
			CatStringWo *line = (CatStringWo *) cat_iiterator_next(iter);
			if (idx==0) {
				if (cat_string_wo_equal(CAT_S(l_s_txt_cut), line)) {
					is_cut = TRUE;
				}
				idx++;
			} else {
				VipPath *source_vip_path = vip_path_new_for_uri(line);
				cat_array_wo_append(paths_to_copy, (GObject *) source_vip_path);
			}
		}

		cat_unref_ptr(iter);
		cat_unref_ptr(copy_string);

		VipService *vip_service = moo_service_get_viper_service(priv->moo_service);
		vip_service_copy_or_move_to(vip_service, paths_to_copy, is_cut, destination_vip);

	} else {
		cat_log_info("nothing to paste or move");
	}
}

void moo_clipboard_paste_resources(MooClipboard *moo_clipboard, CatReadableTreeNode *destination_vip) {
	MooClipboardPrivate *priv = moo_clipboard_get_instance_private(moo_clipboard);
	GtkClipboard *display_clipboard = gtk_clipboard_get_for_display(gtk_widget_get_display(GTK_WIDGET(priv->frame)), GDK_SELECTION_CLIPBOARD);
	GtkSelectionData *selection_data = gtk_clipboard_wait_for_contents(display_clipboard, gdk_atom_intern_static_string ("TARGETS"));
	if (selection_data) {
		GdkAtom *targets;
		gint n_targets;
		if (gtk_selection_data_get_targets (selection_data, &targets, &n_targets)) {
			struct _MooPasteInfo *paste_info = g_new(struct _MooPasteInfo, 1);
			paste_info->moo_clipboard = cat_ref_ptr(moo_clipboard);
			paste_info->destination_vip = cat_ref_ptr(destination_vip);

			if (l_targets_include(moo_clipboard, targets, n_targets, priv->copied_files_atom)) {
				gtk_clipboard_request_contents(display_clipboard, priv->copied_files_atom, l_paste_clipboard_received, paste_info);
			} else if (l_targets_include(moo_clipboard, targets, n_targets, priv->mate_copied_files_atom)) {
				gtk_clipboard_request_contents(display_clipboard, priv->mate_copied_files_atom, l_paste_clipboard_received, paste_info);
			} else {
				g_free(paste_info);
			}
		} else {
			cat_log_warn("no targets");
		}
	}
}

static gboolean l_targets_include(MooClipboard *moo_clipboard, GdkAtom *targets, gint n_targets, GdkAtom requested_target) {
	g_return_val_if_fail(targets!=NULL || n_targets==0, FALSE);
	gboolean result = FALSE;

	int idx;
	for(idx = 0; idx<n_targets; idx++) {
		if (targets[idx] == requested_target) {
			result = TRUE;
			break;
		}
	}
	return result;
}

gboolean moo_clipboard_contains_resources(MooClipboard *moo_clipboard) {
	MooClipboardPrivate *priv = moo_clipboard_get_instance_private(moo_clipboard);
	gboolean result = FALSE;
	GtkClipboard *display_clipboard = gtk_clipboard_get_for_display(gtk_widget_get_display(GTK_WIDGET(priv->frame)), GDK_SELECTION_CLIPBOARD);
	GtkSelectionData *selection_data = gtk_clipboard_wait_for_contents(display_clipboard, gdk_atom_intern_static_string ("TARGETS"));
	if (selection_data) {
		GdkAtom *targets;
		gint n_targets;

		if (gtk_selection_data_get_targets (selection_data, &targets, &n_targets)) {
		      if (gtk_targets_include_uri(targets, n_targets)) {
		    	  result = TRUE;
		      } else if (l_targets_include(moo_clipboard, targets, n_targets, priv->copied_files_atom)) {
		    	  result = TRUE;
		      } else if (l_targets_include(moo_clipboard, targets, n_targets, priv->mate_copied_files_atom)) {
		    	  result = TRUE;
		      } else if (l_targets_include(moo_clipboard, targets, n_targets, priv->copied_resource_atom)) {
		    	  result = TRUE;
		      }
		      g_free (targets);
		}
		gtk_selection_data_free (selection_data);
	}
	return result;
}
