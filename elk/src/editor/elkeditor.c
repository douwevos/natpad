/*
   File:    elkeditor.c
   Project: elk
   Author:  Douwe Vos
   Date:    Jun 25, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#include "elkeditor.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "ElkEditor"
#include <logging/catlog.h>

struct _ElkEditorPrivate {
	void *dummy;
};

G_DEFINE_TYPE_WITH_CODE(ElkEditor, elk_editor, DRA_TYPE_EDITOR,  // @suppress("Unused static function")
	G_ADD_PRIVATE(ElkEditor)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void l_toggle_word_wrap(ChaEditor *editor);
static void l_toggle_show_whitespace(ChaEditor *editor);
static void l_toggle_mark_occurrences(ChaEditor *editor);
static void l_toggle_spelling(DraEditor *editor);

static void elk_editor_class_init(ElkEditorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaEditorClass *cha_class = CHA_EDITOR_CLASS(clazz);
	cha_class->toggleWordWrap = l_toggle_word_wrap;
	cha_class->toggleShowWhitespace = l_toggle_show_whitespace;
	cha_class->toggleMarkOccurrences = l_toggle_mark_occurrences;


	DraEditorClass *dra_class = DRA_EDITOR_CLASS(clazz);
	dra_class->toggleSpelling = l_toggle_spelling;

}

static void elk_editor_init(ElkEditor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ElkEditor *instance = ELK_EDITOR(object);
//	ElkEditorPrivate *priv = elk_editor_get_instance_private(instance);
	G_OBJECT_CLASS(elk_editor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_editor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkEditor *elk_editor_new(ChaDocument *document, DraConnectorMap *connector_map, DraIConnectorRequestFactory *connector_factory, WorService *wor_service) {
	ElkEditor *result = g_object_new(ELK_TYPE_EDITOR, NULL);
	cat_ref_anounce(result);
	ElkEditorPrivate *priv = elk_editor_get_instance_private(result);
	dra_editor_construct(result, document, connector_map, connector_factory, wor_service);
	return result;
}

static void l_toggle_word_wrap(ChaEditor *editor) {
	DraEditorPanel *editor_panel = dra_editor_get_panel((DraEditor *) editor);
	LeaIPanelOwner *panel_owner = lea_panel_get_panel_owner(editor_panel);
	elk_panel_owner_toggle_word_wrap(panel_owner);
}

static void l_toggle_show_whitespace(ChaEditor *editor) {
	DraEditorPanel *editor_panel = dra_editor_get_panel((DraEditor *) editor);
	LeaIPanelOwner *panel_owner = lea_panel_get_panel_owner(editor_panel);
	elk_panel_owner_toggle_show_whitespace(panel_owner);
}

static void l_toggle_mark_occurrences(ChaEditor *editor) {
	DraEditorPanel *editor_panel = dra_editor_get_panel((DraEditor *) editor);
	LeaIPanelOwner *panel_owner = lea_panel_get_panel_owner(editor_panel);
	elk_panel_owner_toggle_mark_occurrences(panel_owner);
}

static void l_toggle_spelling(DraEditor *editor) {
	DraEditorPanel *editor_panel = dra_editor_get_panel((DraEditor *) editor);
	LeaIPanelOwner *panel_owner = lea_panel_get_panel_owner(editor_panel);
	elk_panel_owner_toggle_spelling(panel_owner);
}
