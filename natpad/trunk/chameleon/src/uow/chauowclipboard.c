/*
   File:    chauowclipboard.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    May 9, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "chauowclipboard.h"
#include "chauowinserttext.h"
#include "../selection/chaselection.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowClipboard"
#include <logging/catlog.h>

struct _ChaUowClipboardPrivate {
	ChaUowClipboardAction action;
	gboolean marked_lines_only;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUowClipboard, cha_uow_clipboard, CHA_TYPE_UOW,
		G_ADD_PRIVATE(ChaUowClipboard)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void cha_uow_clipboard_class_init(ChaUowClipboardClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_clipboard_init(ChaUowClipboard *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaUowClipboard *instance = CHA_UOW_CLIPBOARD(object);
//	ChaUowClipboardPrivate *priv = cha_uow_clipboard_get_instance_private(instance);
	G_OBJECT_CLASS(cha_uow_clipboard_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_clipboard_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ChaUowClipboard *cha_uow_clipboard_new(ChaUowClipboardAction action, gboolean marked_lines_only) {
	ChaUowClipboard *result = g_object_new(CHA_TYPE_UOW_CLIPBOARD, NULL);
	cat_ref_anounce(result);
	ChaUowClipboardPrivate *priv = cha_uow_clipboard_get_instance_private(result);
	cha_uow_construct((ChaUow *) result, action==CHA_UOW_CLIPBOARD_COPY);
	priv->action = action;
	priv->marked_lines_only = marked_lines_only;
	return result;
}


static GtkTargetList *l_get_target_list(CatHashMapWo *content) {
	GtkTargetList *target_list = gtk_target_list_new (NULL, 0);

	if (cat_hash_map_wo_get(content, CAT_S(cha_selection_cd_rich_text))!=NULL) {
		gtk_target_list_add(target_list, gdk_atom_intern_static_string ("text/rtf"), GTK_TARGET_OTHER_WIDGET, 1000);
	}
	if (cat_hash_map_wo_get(content, CAT_S(cha_selection_cd_block_text))!=NULL) {
		GdkAtom np_atom = gdk_atom_intern_static_string ("NATPAD_BLOCK");
		cat_log_debug("np_atom=%p", np_atom);
		gtk_target_list_add(target_list, np_atom, GTK_TARGET_SAME_APP, 1001);
	}
	gtk_target_list_add_text_targets(target_list, 0);

	return target_list;
}

static void l_clipboard_get_cb(GtkClipboard *clipboard, GtkSelectionData *selection_data, guint info, gpointer user_data_or_owner) {
	CatHashMapWo *content = (CatHashMapWo *) user_data_or_owner;
	cat_log_debug("content=%p, info=%d", content, info);
	if (info == 1001) {
		CatStringWo *text = (CatStringWo *) cat_hash_map_wo_get(content, (GObject *) CAT_S(cha_selection_cd_block_text));
		cat_log_debug("text=%o", text);
		GdkAtom atom_np_block = gdk_atom_intern_static_string ("NATPAD_BLOCK");
		gtk_selection_data_set(selection_data, atom_np_block, 8, (guchar *) cat_string_wo_getchars(text), cat_string_wo_length(text));
	} else if (info == 1000) {
		CatStringWo *text = (CatStringWo *) cat_hash_map_wo_get(content, (GObject *) CAT_S(cha_selection_cd_rich_text));
		cat_log_debug("text=%o", text);
		GdkAtom tp = gdk_atom_intern_static_string("TEXT/RTF");
		gtk_selection_data_set(selection_data, tp, 8, (guchar *) cat_string_wo_getchars(text), cat_string_wo_length(text));
	} else if (info == 0) {
		CatStringWo *text = (CatStringWo *) cat_hash_map_wo_get(content, (GObject *) CAT_S(cha_selection_cd_plain_text));
		cat_log_debug("text=%o", text);
		gtk_selection_data_set_text(selection_data, (gchar *) cat_string_wo_getchars(text), cat_string_wo_length(text));
	}
}


static void l_clipboard_clear_cb(GtkClipboard *clipboard, gpointer user_data_or_owner) {
	cat_unref_ptr(user_data_or_owner);
}

static void l_gtk_clipboard_copy(ChaSelection *selection, ChaDocumentView *document_view, gboolean marked_lines_only) {

	ChaSelectionClass *sel_class = CHA_SELECTION_GET_CLASS(selection);
	CatHashMapWo *content = sel_class->getForClipboard(selection, document_view, marked_lines_only);
	cat_log_debug("content=%o", content);
	if (content) {

		GtkTargetList *target_list = l_get_target_list(content);

		int n_targets;
		GtkTargetEntry *targets = gtk_target_table_new_from_list (target_list, &n_targets);

		GtkClipboard *clipboard = gtk_clipboard_get(GDK_NONE);

//		CatStringWo *text = (CatStringWo *) cat_hash_map_wo_get(content, (GObject *) CAT_S(cha_selection_cd_plain_text));
//		gtk_clipboard_set_text(clipboard, cat_string_wo_getchars(text), cat_string_wo_length(text));

		GtkTargetList *st_target_list = gtk_target_list_new (NULL, 0);
		gtk_target_list_add_text_targets(st_target_list, 0);
		int n_st_targets;
		GtkTargetEntry *st_targets = gtk_target_table_new_from_list (st_target_list, &n_st_targets);

		gtk_clipboard_set_with_data(clipboard, targets, n_targets, l_clipboard_get_cb, l_clipboard_clear_cb, content);
		gtk_clipboard_set_can_store (clipboard, st_targets, n_st_targets);
		gtk_clipboard_store(clipboard);

	}
}


static void l_clipboard_copy(ChaUowClipboard *clipboard_uow, ChaDocumentView *document_view) {
	ChaUowClipboardPrivate *priv = cha_uow_clipboard_get_instance_private(clipboard_uow);
	ChaSelection *selection = cha_document_view_get_selection(document_view);
	if (selection==NULL) {
		return;
	}

	l_gtk_clipboard_copy(selection, document_view, priv->marked_lines_only);
}

static void l_clipboard_cut(ChaUowClipboard *clipboard_uow, ChaDocumentView *document_view) {
	ChaUowClipboardPrivate *priv = cha_uow_clipboard_get_instance_private(clipboard_uow);
	ChaSelection *selection = cha_document_view_get_selection(document_view);
	if (selection==NULL) {
		return;
	}

	l_gtk_clipboard_copy(selection, document_view, priv->marked_lines_only);

	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);
	cha_document_get_editable_revision(document);

	cha_document_view_remove_selection(document_view);
	if (!is_editable) {
		cha_document_anchor_document(document);
	}
}

static void l_clipboard_paste(ChaUowClipboard *clipboard_uow, ChaDocumentView *document_view) {

	GtkClipboard *clipboard = gtk_clipboard_get(GDK_NONE);

	GdkAtom *targets = NULL;
	gint n_target = 0;
	if (gtk_clipboard_wait_for_targets(clipboard, &targets, &n_target)) {
		GdkAtom atom_np_block = gdk_atom_intern_static_string ("NATPAD_BLOCK");
		cat_log_debug("np_atom=%p", atom_np_block);

		int t_idx;
		for(t_idx=0; t_idx<n_target; t_idx++) {

			if (targets[t_idx] == atom_np_block) {
				cat_log_debug("np1_atom=%p", targets[t_idx]);
				GtkSelectionData *selection_data = gtk_clipboard_wait_for_contents(clipboard, targets[t_idx]);
				if (selection_data) {
					cat_log_debug("pasting as block");
					int length = 0;
					const guchar *data = gtk_selection_data_get_data_with_length(selection_data, &length);

					ChaDocument *document = cha_document_view_get_document(document_view);
					gboolean is_editable = cha_document_is_editable(document);
					ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

					CatStringWo *paste_text = cat_string_wo_new_with_len((const gchar *) data, length);
					cha_block_selection_paste(document_view, e_revision, paste_text);
					if (!is_editable) {
						cha_document_anchor_document(document);
					}
					return;
				}
			}
		}
	}


	cat_log_debug("pasting as plain selection");

	gchar *text = gtk_clipboard_wait_for_text(clipboard);
	if (text) {
//		ChaDocument *document = cha_document_view_get_document(document_view);
//		gboolean is_editable = cha_document_is_editable(document);
//		ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

		CatStringWo *paste_text = cat_string_wo_new_with(text);
//		cha_block_selection_paste(document_view, e_revision, paste_text);

		ChaUow *uow = (ChaUow *) cha_uow_insert_text_new(paste_text);
		cha_uow_run(uow, NULL, document_view);
		cat_unref_ptr(uow);


//		if (!is_editable) {
//			cha_document_anchor_document(document);
//		}
	}
}


static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaUowClipboardPrivate *priv = cha_uow_clipboard_get_instance_private((ChaUowClipboard *) self);
	switch(priv->action) {
		case CHA_UOW_CLIPBOARD_COPY :
			l_clipboard_copy((ChaUowClipboard *) self, document_view);
			break;
		case CHA_UOW_CLIPBOARD_CUT :
			l_clipboard_cut((ChaUowClipboard *) self, document_view);
			break;
		case CHA_UOW_CLIPBOARD_PASTE :
			l_clipboard_paste((ChaUowClipboard *) self, document_view);
			break;
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaUowClipboard *instance = CHA_UOW_CLIPBOARD(self);
	ChaUowClipboardPrivate *priv = cha_uow_clipboard_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p action=%d]", iname, self, priv->action);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

