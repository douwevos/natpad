/*
   File:    drauowspellsuggest.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 30, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "drauowspellsuggest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraUowSpellSuggest"
#include <logging/catlog.h>

struct _DraUowSpellSuggestPrivate {
	CatStringWo *suggest_word;
	DraLineTagWo *spell_tag;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraUowSpellSuggest, dra_uow_spell_suggest, CHA_TYPE_UOW,
		G_ADD_PRIVATE(DraUowSpellSuggest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void dra_uow_spell_suggest_class_init(DraUowSpellSuggestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void dra_uow_spell_suggest_init(DraUowSpellSuggest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraUowSpellSuggest *instance = DRA_UOW_SPELL_SUGGEST(object);
	DraUowSpellSuggestPrivate *priv = dra_uow_spell_suggest_get_instance_private(instance);
	cat_unref_ptr(priv->spell_tag);
	cat_unref_ptr(priv->suggest_word);
	G_OBJECT_CLASS(dra_uow_spell_suggest_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_uow_spell_suggest_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraUowSpellSuggest *dra_uow_spell_suggest_new(CatStringWo *suggest_word, DraLineTagWo *spell_tag) {
	DraUowSpellSuggest *result = g_object_new(DRA_TYPE_UOW_SPELL_SUGGEST, NULL);
	cat_ref_anounce(result);
	DraUowSpellSuggestPrivate *priv = dra_uow_spell_suggest_get_instance_private(result);
	cha_uow_construct((ChaUow *) result, FALSE);
	priv->suggest_word = cat_ref_ptr(suggest_word);
	priv->spell_tag = cat_ref_ptr(spell_tag);
	return result;
}



static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	DraUowSpellSuggestPrivate *priv = dra_uow_spell_suggest_get_instance_private(DRA_UOW_SPELL_SUGGEST(self));
	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

	/* clear old selection if present */
	cha_document_view_clear_selection(document_view, e_revision);

	/* move cursor to begin of tag */
	long long row = dra_line_tag_wo_get_row(priv->spell_tag);
	ChaLineLocationWo *ll = cha_revision_wo_calculate_line_location(e_revision, row);
	int col_start;
	int col_end;
	dra_line_tag_wo_get_start_and_end_index(priv->spell_tag, &col_start, &col_end);
	ChaCursorWo *tag_start_cursor = cha_cursor_wo_new_ll_offset(ll, col_start);
	cha_revision_wo_set_cursor(e_revision, tag_start_cursor);

	/* select tag */
	ChaCursorWo *tag_end_cursor = cha_cursor_wo_new_ll_offset(ll, col_end);
	cha_document_view_set_plain_selection(document_view, tag_start_cursor, tag_end_cursor);

	/* clear original text */
	cha_document_view_remove_selection(document_view);

	/* insert suggested text */
	CatArrayWo *line_list_out = cha_document_create_line_list(document, priv->suggest_word);
	cha_revision_wo_insert(e_revision, line_list_out);
	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);

//	cha_revision_wo_apply_form_changes(e_revision);

	cha_document_view_mark_layout_x_cursor(document_view,e_revision, e_cursor);

	if (!is_editable) {
		cha_document_anchor_document(document);
		cha_document_view_move_view_to_focus(document_view, FALSE);
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
