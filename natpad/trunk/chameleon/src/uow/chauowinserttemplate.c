/*
   File:    chauowinserttemplate.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 4, 2015
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

#include "chauowinserttemplate.h"
#include "../document/chaformwo.h"
#include "../document/chaformfieldwo.h"
#include "../preferences/chaprefstemplatewo.h"
#include "../preferences/chapreferenceswo.h"
#include "../chaeditor.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowInsertTemplate"
#include <logging/catlog.h>

struct _ChaUowInsertTemplatePrivate {
	CatStringWo *template_label;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUowInsertTemplate, cha_uow_insert_template, CHA_TYPE_UOW,
		G_ADD_PRIVATE(ChaUowInsertTemplate)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void cha_uow_insert_template_class_init(ChaUowInsertTemplateClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_insert_template_init(ChaUowInsertTemplate *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaUowInsertTemplate *instance = CHA_UOW_INSERT_TEMPLATE(object);
	ChaUowInsertTemplatePrivate *priv = cha_uow_insert_template_get_instance_private(instance);
	cat_unref_ptr(priv->template_label);
	G_OBJECT_CLASS(cha_uow_insert_template_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_insert_template_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ChaUowInsertTemplate *cha_uow_insert_template_new(CatStringWo *template_label) {
	ChaUowInsertTemplate *result = g_object_new(CHA_TYPE_UOW_INSERT_TEMPLATE, NULL);
	cat_ref_anounce(result);
	ChaUowInsertTemplatePrivate *priv = cha_uow_insert_template_get_instance_private(result);
	cha_uow_construct((ChaUow *) result, FALSE);
	priv->template_label = cat_ref_ptr(template_label);
	return result;
}


static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaUowInsertTemplatePrivate *priv = cha_uow_insert_template_get_instance_private(CHA_UOW_INSERT_TEMPLATE(self));

	ChaPreferencesWo *prefs = cha_editor_get_preferences(editor);
	CatHashMapWo *template_map = cha_preferences_wo_get_template_map(prefs);
	ChaPrefsTemplateWo *a_template = (ChaPrefsTemplateWo *) cat_hash_map_wo_get(template_map, priv->template_label);
	if (a_template==NULL) {
		return;
	}
	CatStringWo *template_text = cha_prefs_template_wo_get_template(a_template);

	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
	cha_document_view_remove_selection(document_view);

	CatArrayWo *fields_list = cat_array_wo_new();
	const CatStringWo *txt_form_start = cat_string_wo_new_with("[%");
	const CatStringWo *txt_form_end = cat_string_wo_new_with("%]");
	ChaFormWo *e_form = cha_form_wo_new();
	CatStringWo *e_out = cat_string_wo_new();
	int idx=0;
	int last_idx = 0;

	ChaLineLocationWo *a_ll_0 = cha_line_location_wo_new();
	a_ll_0 = cha_line_location_wo_anchor(a_ll_0, 0);

	while(idx<cat_string_wo_length(template_text)) {
		int form_start_idx = cat_string_wo_index_of_string_from(template_text, txt_form_start, idx);
		cat_log_debug("form_start_idx=%d", form_start_idx);
		if (form_start_idx>0) {
			cat_log_debug("adding");
			cat_string_wo_append_chars_len(e_out, cat_string_wo_getchars(template_text)+last_idx, form_start_idx-last_idx);
			last_idx = form_start_idx;
			cat_log_debug("added");
			int form_end_idx = cat_string_wo_index_of_string_from(template_text, txt_form_end, form_start_idx+1);
			cat_log_debug("form_end_idx=%d", form_end_idx);
			if (form_end_idx>=0) {
				if (form_end_idx==form_start_idx+1) {
					cat_string_wo_append_chars_len(e_out, "[%", 2);
					idx = form_end_idx+2;
					last_idx = idx;
				} else {
					/* we really are inside a form field now check if it is a calculated field */

					int next_form_start_idx =  form_start_idx+2;
					gboolean is_valid = TRUE;
					gboolean is_calculated = FALSE;
					while(is_valid) {
						next_form_start_idx = cat_string_wo_index_of_string_from(template_text, txt_form_start, next_form_start_idx);
						cat_log_debug("next_form_start_idx=%d", next_form_start_idx);
						if (next_form_start_idx>=0 && next_form_start_idx<form_end_idx) {
							next_form_start_idx+=2;
							is_calculated = TRUE;
							form_end_idx = cat_string_wo_index_of_string_from(template_text, txt_form_end, form_end_idx+2);
							if (form_end_idx<0) {
								is_valid = FALSE;
							}
						} else {
							break;
						}
					}

					if (is_valid) {
						form_start_idx+=2;
						int form_start_col = cat_string_wo_length(e_out);
						CatStringWo *field_content = cat_string_wo_new_with_len(cat_string_wo_getchars(template_text)+form_start_idx, form_end_idx-form_start_idx);
						cat_string_wo_append(e_out, field_content);
						int form_end_col = cat_string_wo_length(e_out);

						int field_index = cat_array_wo_find_index_ext(fields_list, field_content, -1, (GEqualFunc) cat_string_wo_equal);
						if (field_index<0) {
							field_index = cat_array_wo_size(fields_list);
							cat_array_wo_append(fields_list, (GObject *) field_content);
						}
						cat_log_debug("field_index=%d", field_index);


						ChaCursorMWo *ff_start_cur = cha_cursorm_wo_new(0, form_start_col);
						ChaCursorMWo *ff_end_cur = cha_cursorm_wo_new(0, form_end_col);
						cha_form_wo_create_field(e_form, ff_start_cur, ff_end_cur, field_index, !is_calculated);
						cat_unref_ptr(field_content);
						idx = form_end_idx+2;
						last_idx = idx;
					} else {
						cat_string_wo_append_chars_len(e_out, cat_string_wo_getchars(template_text)+last_idx, cat_string_wo_length(template_text)-last_idx);
						break;
					}
				}
			} else {
				cat_string_wo_append_chars_len(e_out, cat_string_wo_getchars(template_text)+last_idx, cat_string_wo_length(template_text)-last_idx);
				break;
			}
		} else {
			cat_string_wo_append_chars_len(e_out, cat_string_wo_getchars(template_text)+last_idx, cat_string_wo_length(template_text)-last_idx);
			break;
		}
	}

	cat_unref_ptr(fields_list);
	cat_unref_ptr(txt_form_start);
	cat_unref_ptr(txt_form_end);



	cat_log_debug("out-length=%d", cat_string_wo_length(e_out));


	ChaCursorMWo *e_s_cursor = cha_cursorm_wo_new(0,0);
	ChaCursorMWo *e_nl_cursor = cha_cursorm_wo_new(0, -1);
	for(idx=0; idx<cat_string_wo_length(e_out); idx++) {
		gchar ch = cat_string_wo_char_at(e_out, idx);
		if ((ch==10) || (ch==13)) {
			cha_cursorm_wo_set_row(e_nl_cursor, cha_cursorm_wo_get_row(e_nl_cursor)+1);
			cha_form_wo_insert(e_form, e_s_cursor, e_nl_cursor);
			cha_cursorm_wo_set_column(e_s_cursor, 0);
			cha_cursorm_wo_set_row(e_s_cursor, cha_cursorm_wo_get_row(e_s_cursor)+1);
		} else {
			cha_cursorm_wo_set_column(e_s_cursor, cha_cursorm_wo_get_column(e_s_cursor)+1);
		}
	}
	cat_unref_ptr(e_s_cursor);
	cat_unref_ptr(e_nl_cursor);

	ChaCursorWo *a_ref_cur = cha_revision_wo_get_cursor(e_revision);
	e_nl_cursor = cha_revision_wo_to_cursorm(e_revision, a_ref_cur);
	e_s_cursor = cha_cursorm_wo_new(0,0);
	cat_log_debug("nl_cursor=%o", e_nl_cursor);
	cha_form_wo_insert(e_form, e_s_cursor, e_nl_cursor);



	cat_log_debug("out-length=%d", cat_string_wo_length(e_out));


	CatArrayWo *line_list_out = cha_document_create_line_list(document, e_out);
	cha_revision_wo_insert(e_revision, line_list_out);

	cha_revision_wo_set_form(e_revision, e_form);

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
	ChaUowInsertTemplate *instance = CHA_UOW_INSERT_TEMPLATE(self);
	ChaUowInsertTemplatePrivate *priv = cha_uow_insert_template_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p name=%o]", iname, self, priv->template_label);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

