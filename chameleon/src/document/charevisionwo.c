/*
   File:    charevisionwo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 24, 2015
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

#include "charevisionwo.h"
#include "chapagelistwo.h"
#include "chalineiterator.h"
#include "page/chafullpagewo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaRevisionWo"
#include <logging/catlog.h>

struct _ChaRevisionWoPrivate {
	int version;
	short enriched_count;
	ChaEnrichmentData *enrichment_data;
	ChaCursorWo *cursor;
	ChaPageListWo *page_list;
	ChaFormWo *form;
	ChaLoadToken *load_token;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaRevisionWo, cha_revision_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaRevisionWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);


static void cha_revision_wo_class_init(ChaRevisionWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_revision_wo_init(ChaRevisionWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaRevisionWo *instance = CHA_REVISION_WO(object);
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(instance);
	if (priv->enriched_count>0) {
		cat_log_warn("enrich and impoverish calls not in balance for %o", instance);
		priv->enriched_count = 1;
		cha_revision_wo_impoverish(instance);
	}
	cat_unref_ptr(priv->enrichment_data);
	cat_unref_ptr(priv->page_list);
	cat_unref_ptr(priv->cursor);
	cat_unref_ptr(priv->load_token);
	cat_unref_ptr(priv->form);
	G_OBJECT_CLASS(cha_revision_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_revision_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaRevisionWo *cha_revision_wo_new() {
	ChaRevisionWo *result = g_object_new(CHA_TYPE_REVISION_WO, NULL);
	cat_ref_anounce(result);
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->page_list = cha_page_list_wo_new();
	priv->enriched_count = 0;
	priv->enrichment_data = NULL;
	priv->cursor = cha_cursor_wo_new();
	priv->load_token = NULL;
	priv->form = NULL;
	ChaPageWo *page = (ChaPageWo *) cha_full_page_wo_new(NULL);
	cha_page_list_wo_add_page(priv->page_list, page);
	ChaLineWo *line = cha_line_wo_new();
	cha_page_wo_add_line(page, line);
	cat_unref_ptr(line);
	cat_unref_ptr(page);
	return result;
}

int cha_revision_wo_get_content_version(ChaRevisionWo *revision) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(revision);
	return cat_wo_get_version((CatWo *) priv->page_list);
}

void cha_revision_wo_enrich(ChaRevisionWo *revision, ChaEnrichmentDataMapWo *a_map) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(revision);
	if (priv->enriched_count==0) {
		if (priv->enrichment_data==NULL) {
			priv->enrichment_data = cha_enrichment_data_new(a_map);
		}
		cha_page_list_wo_enrich(priv->page_list, a_map);
	}
	priv->enriched_count++;

	cat_log_detail("%p: enriched %d", revision, priv->enriched_count);

}

void cha_revision_wo_impoverish(ChaRevisionWo *revision) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(revision);
	if (priv->enriched_count==1) {
		cha_page_list_wo_impoverish(priv->page_list);
		cat_unref_ptr(priv->enrichment_data);
	}
	priv->enriched_count--;
	cat_log_detail("%p: impoverished %d", revision, priv->enriched_count);
}

void cha_revision_wo_enrichment_remap(ChaRevisionWo *revision, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(revision);
	ChaEnrichmentData *enrichment_data = priv->enrichment_data;
	cat_log_debug("remap:enrichment_data=%p", enrichment_data);
	if (enrichment_data) {
		cha_enrichment_data_remap(enrichment_data, a_old_map, a_new_map, add_idx, rem_idx);
	}
	cha_page_list_wo_enrichment_remap(priv->page_list, a_old_map, a_new_map, add_idx, rem_idx);
}

int cha_revision_wo_get_slot_index(ChaRevisionWo *revision, GObject *slot_key, int slot_key_idx) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(revision);
	int result = -1;
	cat_log_debug("enrichment_data=%o", priv->enrichment_data);
	if (priv->enrichment_data) {
		ChaEnrichmentDataMapWo *enrichment_map = cha_enrichment_data_get_map(priv->enrichment_data);
		cat_log_debug("enrichment_map=%o", enrichment_map);
		result = cha_enrichment_data_map_wo_find_slot_index(enrichment_map, slot_key, slot_key_idx);
	}
	return result;
}

int cha_revision_wo_get_page_list_version(const ChaRevisionWo *revision) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private((ChaRevisionWo *) revision);
	return cat_wo_get_version((CatWo *) priv->page_list);
}


ChaPageWo *cha_revision_wo_page_at(ChaRevisionWo *revision, int page_idx) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(revision);
	return cha_page_list_wo_page_at(priv->page_list, page_idx);
}


ChaCursorWo *cha_revision_wo_create_end_of_revision_cursor(ChaRevisionWo *revision) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(revision);
	int page_count = cha_page_list_wo_page_count(priv->page_list);
	ChaPageWo *last_page = cha_page_list_wo_page_at(priv->page_list, page_count-1);
	cha_page_wo_hold_lines(last_page);
	int page_line_count = cha_page_wo_line_count(last_page);
	const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(last_page, page_line_count-1, FALSE);
	ChaLineLocationWo *end_ll = cha_line_location_wo_new_with(page_count-1, page_line_count-1);
	ChaCursorWo *result = cha_cursor_wo_new_ll_offset(end_ll, utf8_text.text_len);
	cat_unref_ptr(end_ll);
	cha_utf8_text_cleanup(&utf8_text);
	cha_page_wo_release_lines(last_page);
	return result;
}


ChaCursorWo *cha_revision_wo_get_cursor(ChaRevisionWo *revision) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(revision);
	return priv->cursor;
}

ChaLoadToken *cha_revision_wo_get_load_token(ChaRevisionWo *revision) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(revision);
	return priv->load_token;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_revision)) { \
			cat_log_error("Object is read only:%o", e_revision); \
			return rval; \
		} \

void cha_revision_wo_set_load_token(ChaRevisionWo *e_revision, ChaLoadToken *load_token) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->load_token, load_token);
}

ChaLineLocationWo *cha_revision_wo_calculate_line_location(const ChaRevisionWo *revision, long long row) {
	const ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private((ChaRevisionWo *) revision);
	return cha_page_list_wo_calculate_line_location(priv->page_list, row);
}



ChaLineWo *cha_revision_wo_line_at_location(const ChaRevisionWo *revision, const ChaLineLocationWo *line_location) {
	const ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private((ChaRevisionWo *) revision);
	int page_idx = cha_line_location_wo_get_page_index(line_location);
	ChaPageWo *page = cha_page_list_wo_page_at(priv->page_list, page_idx);
	return cha_page_wo_line_at(page, cha_line_location_wo_get_page_line_index(line_location));
}

static void l_set_form_field_value(ChaRevisionWo *e_revision, int form_field_index, CatArrayWo *e_field_value) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	cat_log_debug("e_revision=%p, form=%p, form_field_index=%d", e_revision, priv->form, form_field_index);
	ChaFormFieldWo *form_field = cha_form_wo_editable_field_at(priv->form, form_field_index);
	ChaCursorMWo *field_start = cha_form_field_wo_get_start(form_field);
	ChaCursorMWo *field_end = cha_form_field_wo_get_end(form_field);

	cat_log_debug("before remove: form_field=%o", form_field);
//	cat_log_on_trace({ l_dump(snapshot); });

	cha_revision_wo_remove_between_cursors(e_revision, field_start, field_end, FALSE, NULL);

	cat_log_debug("before insert: form_field=%o", form_field);

	ChaCursorMWo *new_field_end = cha_cursorm_wo_clone(field_start, CAT_CLONE_DEPTH_FULL);

	cha_revision_wo_insert_lines(e_revision, new_field_end, e_field_value, FALSE);

//	cat_log_on_trace({ l_dump(result_snapshot); });


	ChaCursorOrder ora = cha_cursorm_wo_order(field_end, new_field_end);
	cat_log_debug("after insert: new_field_end=%o, ora=%d", new_field_end, ora);

	switch(ora) {
		case CHA_CURSOR_ABOVE :
		case CHA_CURSOR_LEFT : {
			cha_form_wo_remove(priv->form, new_field_end, field_end);
		} break;
		case CHA_CURSOR_BELOW :
		case CHA_CURSOR_RIGHT : {
			cha_form_wo_insert(priv->form, field_end, new_field_end);
		} break;
		case CHA_CURSOR_SAME : {
		} break;
	}


//	AstSnapshotForm *new_form = ast_snapshot_form_insert(form, field_end, new_field_end);
	cat_log_debug("form=%o", priv->form);
	cat_unref_ptr(new_field_end);
}

static void l_form_field_changed(ChaRevisionWo *e_revision, int mod_form_field_offset) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	ChaFormWo *form = priv->form;
	ChaFormFieldWo *form_field = cha_form_wo_get_at(form, mod_form_field_offset);
	cat_log_debug("form_field=%o", form_field);

	ChaCursorMWo *field_start = cha_form_field_wo_get_start(form_field);
	ChaCursorMWo *field_end = cha_form_field_wo_get_end(form_field);

	CatArrayWo *e_new_field_text = cha_revision_wo_get_lines_between_cursors(e_revision, field_start, field_end);
	int field_index = cha_form_field_wo_get_field_index(form_field);

	int field_offset;
	for(field_offset=cha_form_wo_field_count(form)-1; field_offset>=0; field_offset--) {
		cat_log_debug("field_offset=%d, mod_form_field_offset=%d, form=%p, e_revision=%p", field_offset, mod_form_field_offset, form, e_revision);
		if (mod_form_field_offset == field_offset) {
			continue;
		}
		ChaFormFieldWo *mod_form_field = cha_form_wo_get_at(form, field_offset);
		cat_log_debug("mod_form_field=%o", mod_form_field);
		if (cha_form_field_wo_get_field_index(mod_form_field) == field_index) {
			l_set_form_field_value(e_revision, field_offset, e_new_field_text);
		}
	}
	cat_unref_ptr(e_new_field_text);
	cat_log_debug("done");
}


void cha_revision_wo_replace_unichar(ChaRevisionWo *e_revision, ChaCursorMWo *e_insert_location, gunichar unichar, gboolean handle_form) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE();

	ChaCursorMWo *original = cha_cursorm_wo_clone(e_insert_location, CAT_CLONE_DEPTH_MAIN);

	ChaLineLocationWo *e_insert_ll = cha_revision_wo_calculate_line_location(e_revision, cha_cursorm_wo_get_row(e_insert_location));

	ChaPageWo *e_page = cha_revision_wo_editable_page_at(e_revision, cha_line_location_wo_get_page_index(e_insert_ll));
	int page_line_index = cha_line_location_wo_get_page_line_index(e_insert_ll);
	ChaLineWo *e_line = cha_page_wo_editable_line_at(e_page, page_line_index);
	cat_log_trace("e_line=%o", e_line);
	CatStringWo *e_text = cha_line_wo_get_text(e_line);
	cat_log_trace("e_text=%o", e_text);
	int x_cursor_left = cha_cursorm_wo_get_column(e_insert_location);
	cat_unref_ptr(e_insert_ll);


	int x_cursor_right = x_cursor_left;
	cat_string_wo_unichar_at(e_text, &x_cursor_right);

	CatStringWo *uni_text = cat_string_wo_new();
	cat_string_wo_append_unichar(uni_text, unichar);

	cat_string_wo_replace_range_with(e_text, x_cursor_left, x_cursor_right-x_cursor_left, uni_text);

	cha_cursorm_wo_set_column(e_insert_location, x_cursor_left + cat_string_wo_length(uni_text));

	gboolean is_replacing = x_cursor_right!=x_cursor_left;

	/* handle form manipulation */
	if (handle_form && priv->form) {
		if (cha_form_wo_is_anchored(priv->form)) {
			ChaFormWo *e_form = cha_form_wo_create_editable(priv->form);
			cat_unref_ptr(priv->form);
			priv->form = e_form;
		}

		int field_idx = 0;
		int field_count = cha_form_wo_field_count(priv->form);
		ChaFormFieldWo *modified_form_field = NULL;
		int modified_form_field_offset = -1;
		for(field_idx=0; field_idx<field_count; field_idx++) {
			ChaFormFieldWo *form_field = cha_form_wo_get_at(priv->form, field_idx);
			cat_log_debug("test fields[%d] : field=%o, original=%o", field_idx, form_field, original);
			if (cha_form_field_wo_test_cursor_in_field(form_field, original)) {

				if (is_replacing) {
					if (!cha_form_field_wo_test_cursor_in_field(form_field, e_insert_location)) {
						continue;
					}
				}

				modified_form_field = form_field;
				modified_form_field_offset = field_idx;
				break;
			}
		}

		int diff = cat_string_wo_length(uni_text)-(x_cursor_right-x_cursor_left);
		if (diff>0) {
			ChaCursorMWo *fake = cha_cursorm_wo_clone(original, CAT_CLONE_DEPTH_MAIN);
			cha_cursorm_wo_move(fake,0,diff);
			cha_form_wo_insert(priv->form, original, fake);
		} else if (diff<0) {
			ChaCursorMWo *fake = cha_cursorm_wo_clone(original, CAT_CLONE_DEPTH_MAIN);
			cha_cursorm_wo_move(fake,0,-diff);
			cha_form_wo_remove(priv->form, fake, original);
		}


		if (modified_form_field) {
			modified_form_field = cha_form_wo_editable_field_at(priv->form, modified_form_field_offset);
			ChaCursorMWo *field_start_cursor = cha_form_field_wo_get_start(modified_form_field);
			ChaCursorMWo *field_end_cursor = cha_form_field_wo_get_end(modified_form_field);
			cat_log_debug("field_start_cursor=%o, field_end_cursor=%o", field_start_cursor, field_end_cursor);
			int rel_pos = 0;
			long long new_row = cha_cursorm_wo_get_row(e_insert_location);
			int new_column = cha_cursorm_wo_get_column(e_insert_location);
			long long rel_row = new_row-cha_cursorm_wo_get_row(field_start_cursor);
			int rel_col = new_column;
			if (new_row==cha_cursorm_wo_get_row(field_start_cursor)) {
				rel_col = new_column-cha_cursorm_wo_get_column(field_start_cursor);
				rel_pos = 1;
			} else if (new_row==cha_cursorm_wo_get_row(field_end_cursor)) {
				rel_col = new_column-cha_cursorm_wo_get_column(field_end_cursor);
				rel_pos = 2;
			}

			cat_log_debug("rel_row=%d, rel_col=%d, rel_pos=%d", rel_row, rel_col, rel_pos);

			l_form_field_changed(e_revision, modified_form_field_offset);


			modified_form_field = cha_form_wo_editable_field_at(priv->form, modified_form_field_offset);

//			if (ptr_new_cursor!=NULL) {
			modified_form_field = cha_form_wo_get_at(priv->form, modified_form_field_offset);
			field_start_cursor = cha_form_field_wo_get_start(modified_form_field);
			field_end_cursor = cha_form_field_wo_get_end(modified_form_field);
			cat_log_debug("field_start_cursor=%o, field_end_cursor=%o", field_start_cursor, field_end_cursor);
			if (rel_pos==0) {
				cha_cursorm_wo_set_column(e_insert_location, rel_col);
				cha_cursorm_wo_set_row(e_insert_location, cha_cursorm_wo_get_row(field_start_cursor)+rel_row);
			} else if (rel_pos==1) {
				cha_cursorm_wo_set_column(e_insert_location, cha_cursorm_wo_get_column(field_start_cursor)+rel_col);
				cha_cursorm_wo_set_row(e_insert_location, cha_cursorm_wo_get_row(field_start_cursor));
			} else if (rel_pos==2) {
				cha_cursorm_wo_set_column(e_insert_location, cha_cursorm_wo_get_column(field_end_cursor)+rel_col);
				cha_cursorm_wo_set_row(e_insert_location, cha_cursorm_wo_get_row(field_end_cursor));
			}
			cat_log_debug("e_insert_location=%o", e_insert_location);
		}
	}
}

void cha_revision_wo_line_multi_replace(ChaRevisionWo *e_revision, long long row, ChaPageWo *e_page, int page_line_index, ChaLineMultiReplace *line_multiple_replace, int *left_result, int *right_result) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);


	CatIntArrayWo *utf8_offset_table = NULL;
	if (priv->form) {
		utf8_offset_table = cat_int_array_wo_new();
	}

	const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(e_page, page_line_index, FALSE);
	const CatStringWo *src_txt = cat_string_wo_new_data_len(utf8_text.text, utf8_text.text_len);
	int src_utf8_offset = 0;
	int dest_utf8_offset = 0;

	CatStringWo *e_buf = cat_string_wo_new_with_len(NULL, utf8_text.text_len);

	int last_out = 0;
	int last_offset = 0;
	int replace_idx;
	int replace_count = cha_line_multi_replace_entry_count(line_multiple_replace);
	for(replace_idx=0; replace_idx<replace_count; replace_idx++) {
		const ChaLineMultiReplaceEntry *entry = cha_line_multi_replace_entry_at(line_multiple_replace, replace_idx);
		int next_offset = entry->start_offset;
		cat_log_debug("last_offset=%d, next_offset=%d", last_offset, next_offset);
		if (last_offset!=next_offset) {
			cat_string_wo_append_chars_len(e_buf, utf8_text.text+last_offset, next_offset-last_offset);
		}

		if (utf8_offset_table) {
			/* handle form */
			while(src_utf8_offset<next_offset) {
				int next_in = cat_string_wo_unichar_next_offset(src_txt, src_utf8_offset);
				int next_out = dest_utf8_offset;
				while(src_utf8_offset<next_in) {
					cat_int_array_wo_append(utf8_offset_table, dest_utf8_offset);
					cat_log_debug("src_utf8_offset=%d, next_in=%d, dest_utf8_offset=%d", src_utf8_offset, next_in, dest_utf8_offset);
					next_out++;
					src_utf8_offset++;
				}
				dest_utf8_offset = next_out;
			}

		}



		if (left_result && replace_idx==0) {
			*left_result = cat_string_wo_length(e_buf);
		}
		if (entry->replacement) {
			cat_string_wo_append(e_buf, entry->replacement);
		}

		if (utf8_offset_table) {

			if (next_offset<entry->end_offset) {
				/* text is replace or removed */

				int rep_offset = 0;
				while(src_utf8_offset<entry->end_offset) {
					int next_in = cat_string_wo_unichar_next_offset(src_txt, src_utf8_offset);
					if (entry->replacement) {
						rep_offset = cat_string_wo_unichar_next_offset(entry->replacement, rep_offset);
					}
					while(src_utf8_offset<next_in) {
						cat_int_array_wo_append(utf8_offset_table, dest_utf8_offset + rep_offset);
						cat_log_debug("src_utf8_offset=%d, next_in=%d, dest_utf8_offset + rep_offset=%d", src_utf8_offset, next_in, dest_utf8_offset + rep_offset);
						src_utf8_offset++;
					}
				}
				if (entry->replacement) {
					dest_utf8_offset += cat_string_wo_length(entry->replacement);
				}
			}

		}



		cat_log_debug("entry->replacement=%o", entry->replacement);
		last_out = cat_string_wo_length(e_buf);
		last_offset = entry->end_offset;
		cat_log_debug("last_offset=%d",last_offset);


	}

	if ((last_offset>0) || (last_out>0)) {
		if (right_result) {
			*right_result = last_out;
		}
		if (last_offset<utf8_text.text_len) {
			cat_log_debug("append rest:left=%d", source_line_length-last_offset);
			cat_string_wo_append_chars_len(e_buf, utf8_text.text+last_offset, utf8_text.text_len-last_offset);


			if (utf8_offset_table) {
				/* handle form */
				while(src_utf8_offset<utf8_text.text_len) {
					int next_in = cat_string_wo_unichar_next_offset(src_txt, src_utf8_offset);
					int next_out = dest_utf8_offset;
					while(src_utf8_offset<next_in) {
						cat_int_array_wo_append(utf8_offset_table, dest_utf8_offset);
						cat_log_debug("src_utf8_offset=%d, next_in=%d, dest_utf8_offset=%d", src_utf8_offset, next_in, dest_utf8_offset);
						next_out++;
						src_utf8_offset++;
					}
					dest_utf8_offset = next_out;
				}

			}
		}
		if (utf8_offset_table) {
			cat_int_array_wo_append(utf8_offset_table, dest_utf8_offset);
		}

		cat_log_debug("buf=%o", e_buf);

		cha_page_wo_hold_lines(e_page);

		ChaLineWo *e_line = cha_page_wo_editable_line_at(e_page, page_line_index);
		cha_line_wo_set_text(e_line, e_buf);
		cha_page_wo_release_lines(e_page);

		if (utf8_offset_table) {
			int fidx = cha_form_wo_get_first_index_of_field_at_row(priv->form, row);
			cat_log_debug("fidx=%d", fidx);
			if (fidx>=0) {
				int field_cnt = cha_form_wo_field_count(priv->form);
				while(fidx<field_cnt) {
					if (cha_form_wo_is_anchored(priv->form)) {
						ChaFormWo *e_form = cha_form_wo_create_editable(priv->form);
						cat_unref_ptr(priv->form);
						priv->form = e_form;
					}

					cat_log_debug("fidx=%d", fidx);
					ChaFormFieldWo *e_field = cha_form_wo_editable_field_at(priv->form, fidx++);
					cat_log_debug("e_field=%o", e_field);
					ChaCursorMWo *fstart = cha_form_field_wo_get_start(e_field);
					if (cha_cursorm_wo_get_row(fstart)>row) {
						break;
					}

					ChaCursorMWo *fend = cha_form_field_wo_get_end(e_field);

					int start_col = cha_cursorm_wo_get_column(fstart);
					int end_col = cha_cursorm_wo_get_column(fend);

					int fstart_row = cha_cursorm_wo_get_row(fstart);

					gboolean endIsOnOtherRow = cha_cursorm_wo_get_row(fend)!=row;

					for(replace_idx=0; replace_idx<replace_count; replace_idx++) {
						const ChaLineMultiReplaceEntry *entry = cha_line_multi_replace_entry_at(line_multiple_replace, replace_idx);
						cat_log_debug("##### entry=%o", entry);
						if ((entry->start_offset>=end_col && fstart_row==row) || (fstart_row>row)) {
							break;
						}
						cat_log_debug("##### endIsOnOtherRow=%d", endIsOnOtherRow);
						if (entry->end_offset>start_col || endIsOnOtherRow) {
							cat_log_debug("##### markin=%d", endIsOnOtherRow);
							cha_form_field_wo_mark_as_modified(e_field);
							break;
						}
					}


					if (cha_cursorm_wo_get_row(fstart)==row) {
						fstart = cha_cursorm_wo_clone(fstart, CAT_CLONE_DEPTH_FULL);
						int new_column = cat_int_array_wo_get(utf8_offset_table, start_col);
						cha_cursorm_wo_set_column(fstart, new_column);
						cat_log_debug("start:cur_col=%d, new_column=%d", start_col, new_column);
					} else {
						cat_ref_ptr(fstart);
					}

					if (cha_cursorm_wo_get_row(fend)==row) {
						fend = cha_cursorm_wo_clone(fend, CAT_CLONE_DEPTH_FULL);
						int new_column = cat_int_array_wo_get(utf8_offset_table, end_col);
						cha_cursorm_wo_set_column(fend, new_column);
						cat_log_debug("end:cur_col=%d, new_column=%d", end_col, new_column);
					} else {
						cat_ref_ptr(fend);
					}



					cha_form_field_wo_set_start_and_end(e_field, fstart, fend);
					cat_unref_ptr(fstart);
					cat_unref_ptr(fend);
				}
			}
		}

	} else {
		if (left_result) {
			*left_result = 0;
		}
		if (right_result) {
			*right_result = utf8_text.text_len;
		}
		cat_unref_ptr(e_buf);
	}
	cha_utf8_text_cleanup(&utf8_text);

}



void cha_revision_wo_insert_lines(ChaRevisionWo *e_revision, ChaCursorMWo *e_insert_location, CatArrayWo *lines_to_insert, gboolean handle_form) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE();

	cat_log_debug("lines_to_insert.size=%d", cat_array_wo_size(lines_to_insert));

	ChaCursorMWo *original = cha_cursorm_wo_clone(e_insert_location, CAT_CLONE_DEPTH_MAIN);

	ChaLineLocationWo *e_insert_ll = cha_revision_wo_calculate_line_location(e_revision, cha_cursorm_wo_get_row(e_insert_location));

	ChaPageWo *e_page = cha_revision_wo_editable_page_at(e_revision, cha_line_location_wo_get_page_index(e_insert_ll));
	int page_line_index = cha_line_location_wo_get_page_line_index(e_insert_ll);
	ChaLineWo *e_line = cha_page_wo_editable_line_at(e_page, page_line_index);
	cat_log_trace("e_line=%o", e_line);
	CatStringWo *e_text = cha_line_wo_get_text(e_line);
	cat_log_trace("e_text=%o", e_text);
	int x_cursor = cha_cursorm_wo_get_column(e_insert_location);
	cat_unref_ptr(e_insert_ll);



	int pi_cnt = cat_array_wo_size(lines_to_insert)-1;
	int pidx;
	cha_cursorm_wo_move(e_insert_location, pi_cnt, 0);

	for(pidx=0; pidx<=pi_cnt; pidx++) {
		ChaLineWo *in_line = (ChaLineWo *) cat_array_wo_get(lines_to_insert, pidx);

		cat_log_debug("in_line=%o, pi_cnt=%d", in_line, pi_cnt);

		if (pi_cnt==0) {
			if (cha_line_wo_get_line_end(in_line)==CHA_LINE_END_NONE) {
				CatStringWo *in_text = cha_line_wo_get_text(in_line);
				cat_log_debug("in_text=%o, e_txt=%o", in_text, e_text);
				cat_string_wo_insert(e_text, in_text, x_cursor);
				cha_cursorm_wo_set_column(e_insert_location, x_cursor + cat_string_wo_length(in_text));
				cat_log_debug("e_text=%o", e_text);

			} else {
				ChaLineWo *e_in_line = cha_line_wo_create_editable(in_line);
				if (x_cursor>0) {
					/* insert existing text left of cursor in front of e_in_line */
					CatStringWo *pre_text = cat_string_wo_new_sub(e_text, 0, x_cursor);
					cat_log_debug("pre_text=%o, x_cursor=%d", pre_text, x_cursor);
					CatStringWo *e_in_text = cha_line_wo_get_text(e_in_line);
					cat_string_wo_insert(e_in_text, pre_text, 0);
					cat_unref_ptr(pre_text);

					cat_log_debug("e_text=%o", e_text);
					cat_string_wo_remove_range(e_text, 0, x_cursor);
					cat_log_debug("e_text=%o", e_text);
				}
				cha_page_wo_insert_line(e_page, e_in_line, page_line_index);
				page_line_index++;
				cha_cursorm_wo_move(e_insert_location, 1, 0);
				cha_cursorm_wo_set_column(e_insert_location, 0);
				cat_unref_ptr(e_in_line);
			}
		} else {
			ChaLineWo *e_in_line = cha_line_wo_create_editable(in_line);
			if (pidx==0) {
				/* first line */
				if (x_cursor>0) {
					/* insert existing text left of cursor in front of e_in_line */
					CatStringWo *pre_text = cat_string_wo_new_sub(e_text, 0, x_cursor);
					CatStringWo *e_in_text = cha_line_wo_get_text(e_in_line);
					cat_string_wo_insert(e_in_text, pre_text, 0);
					cat_unref_ptr(pre_text);
				}
				cha_page_wo_insert_line(e_page, e_in_line, page_line_index);
				page_line_index++;
			} else if (pidx!=pi_cnt) {
				/* middle line */
				cha_page_wo_insert_line(e_page, e_in_line, page_line_index);
				page_line_index++;
			} else {
				/* last line */
				if (x_cursor>0) {
					cat_string_wo_remove_range(e_text, 0, x_cursor);
				}
				if (cha_line_wo_get_line_end(e_in_line)==CHA_LINE_END_NONE) {
					CatStringWo *e_in_text = cha_line_wo_get_text(e_in_line);
					cha_cursorm_wo_set_column(e_insert_location, cat_string_wo_length(e_in_text));
					cat_string_wo_insert(e_text, e_in_text, 0);
				} else {
					cha_page_wo_insert_line(e_page, e_in_line, page_line_index);
					page_line_index++;
					cha_cursorm_wo_move(e_insert_location, 1, 0);
					cha_cursorm_wo_set_column(e_insert_location, 0);
				}
			}
			cat_unref_ptr(e_in_line);
		}
	}


	/* handle form manipulation */
	if (handle_form && priv->form) {
		if (cha_form_wo_is_anchored(priv->form)) {
			ChaFormWo *e_form = cha_form_wo_create_editable(priv->form);
			cat_unref_ptr(priv->form);
			priv->form = e_form;
		}

		int field_idx = 0;
		int field_count = cha_form_wo_field_count(priv->form);
		for(field_idx=0; field_idx<field_count; field_idx++) {
			ChaFormFieldWo *e_form_field = cha_form_wo_editable_field_at(priv->form, field_idx);
			cat_log_on_debug({
				gboolean in_field = cha_form_field_wo_test_cursor_in_field(e_form_field, original);
				cat_log_debug("test fields[%d] %s: field=%o, original=%o", field_idx, in_field ? "POS" : "---", e_form_field, original);
			});
			if (cha_form_field_wo_test_cursor_in_field(e_form_field, original)) {
				cha_form_field_wo_mark_as_modified(e_form_field);
				break;
			}
		}

		cha_form_wo_insert(priv->form, original, e_insert_location);
	}
	cat_unref_ptr(original);
}



void cha_revision_wo_insert(ChaRevisionWo *e_revision, CatArrayWo *ce_lines_2_insert) {
	if (ce_lines_2_insert==NULL) {
		return;
	}
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE();

	cat_log_debug("lines_2_insert.size=%d", cat_array_wo_size(ce_lines_2_insert));

	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	ChaCursorMWo *e_cursorm = cha_revision_wo_to_cursorm(e_revision, e_cursor);
	cha_revision_wo_insert_lines(e_revision, e_cursorm, ce_lines_2_insert, TRUE);
	cat_unref_ptr(ce_lines_2_insert);
	ChaLineLocationWo *nll = cha_page_list_wo_calculate_line_location(priv->page_list, cha_cursorm_wo_get_row(e_cursorm));
	cha_cursor_wo_set_line_location(e_cursor, nll);
	cha_cursor_wo_set_x_cursor_bytes(e_cursor, cha_cursorm_wo_get_column(e_cursorm));
	cat_unref_ptr(e_cursorm);
	cat_unref_ptr(nll);
}


void cha_revision_wo_remove(ChaRevisionWo *e_revision, ChaCursorWo *cursor) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE();
	ChaCursorOrder cursor_order = cha_cursor_wo_compare(priv->cursor, cursor);
	cat_log_trace("priv->cursor=%o, cursor=%o, cursor_order=%d", priv->cursor, cursor, cursor_order);
	if (cursor_order==CHA_CURSOR_SAME) {
		return;
	}

	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
	ChaCursorMWo *e_cursorm = cha_revision_wo_to_cursorm(e_revision, e_cursor);
	ChaCursorMWo *e_cursorm_end = cha_revision_wo_to_cursorm(e_revision, cursor);
	ChaCursorMWo *out_cursor = NULL;
	cha_revision_wo_remove_between_cursors(e_revision, e_cursorm, e_cursorm_end, TRUE, (ChaCursorMWo **) (&out_cursor));
	cat_unref_ptr(e_cursorm_end);
	cat_unref_ptr(e_cursorm);
	cat_log_trace("out_cursor=%o", out_cursor);
	if (out_cursor) {
		ChaLineLocationWo *nll = cha_page_list_wo_calculate_line_location(priv->page_list, cha_cursorm_wo_get_row(out_cursor));
		cha_cursor_wo_set_line_location(e_cursor, nll);
		cha_cursor_wo_set_x_cursor_bytes(e_cursor, cha_cursorm_wo_get_column(out_cursor));
		cat_unref_ptr(out_cursor);
		cat_unref_ptr(nll);
	}
}

void cha_revision_wo_remove_between_cursors(ChaRevisionWo *e_revision, ChaCursorMWo *cursor_a, ChaCursorMWo *cursor_b, gboolean modify_form, ChaCursorMWo **new_cursor) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE();
	int order = cha_cursorm_wo_order(cursor_a, cursor_b);
	if (order==CHA_CURSOR_SAME) {
		if (new_cursor) {
			*new_cursor = cha_cursorm_wo_clone(cursor_a, CAT_CLONE_DEPTH_MAIN);
		}
		return;
	} else if (order==CHA_CURSOR_LEFT) {
		order = CHA_CURSOR_RIGHT;
		ChaCursorMWo *swap = cursor_a;
		cursor_a = cursor_b;
		cursor_b = swap;
	} else if (order==CHA_CURSOR_ABOVE) {
		order = CHA_CURSOR_BELOW;
		ChaCursorMWo *swap = cursor_a;
		cursor_a = cursor_b;
		cursor_b = swap;
	}

	if (new_cursor) {
		*new_cursor = cha_cursorm_wo_clone(cursor_a, CAT_CLONE_DEPTH_MAIN);
	}

	cursor_b = cha_cursorm_wo_clone(cursor_b, CAT_CLONE_DEPTH_MAIN);

	cat_log_debug("cursor_a:%o, cursor_b:%o, order=%d", cursor_a, cursor_b, order);

	if (order==CHA_CURSOR_RIGHT) {
		cat_log_debug("single line remove");
		/* single line remove */
		ChaLineLocationWo *start_location = cha_revision_wo_calculate_line_location(e_revision, cha_cursorm_wo_get_row(cursor_a));
		int start_page_index = cha_line_location_wo_get_page_index(start_location);
		ChaPageWo *e_page = cha_revision_wo_editable_page_at(e_revision, start_page_index);
		ChaLineWo *e_line = cha_page_wo_editable_line_at(e_page, cha_line_location_wo_get_page_line_index(start_location));
		cat_unref_ptr(start_location);
		CatStringWo *e_text = cha_line_wo_get_text(e_line);
		int left = cha_cursorm_wo_get_column(cursor_a);
		int right = cha_cursorm_wo_get_column(cursor_b);
		cat_log_debug("left=%d, right=%d, e_text=%o", left, right, e_text);
		cat_string_wo_remove_range(e_text, left, right-left);
		cat_log_debug("e_line=%o", e_line);
		cat_log_debug("e_text=%o", e_text);
	} else {
		/* multi line remove */
		cat_log_debug("multi line remove");
		ChaLineLocationWo *start_location = cha_revision_wo_calculate_line_location(e_revision, cha_cursorm_wo_get_row(cursor_a));
		int start_page_index = cha_line_location_wo_get_page_index(start_location);
		ChaPageWo *e_start_page = cha_revision_wo_editable_page_at(e_revision, start_page_index);
		int start_page_line_index = cha_line_location_wo_get_page_line_index(start_location);
		ChaLineWo *e_start_line = cha_page_wo_editable_line_at(e_start_page, start_page_line_index);
		cat_unref_ptr(start_location);

		ChaLineLocationWo *end_location = cha_revision_wo_calculate_line_location(e_revision, cha_cursorm_wo_get_row(cursor_b));
		if (end_location==NULL) {
			end_location = cha_page_list_wo_calculate_line_location_end(priv->page_list);
			int eot_page_index = cha_line_location_wo_get_page_index(end_location);
			ChaPageWo *eot_page = cha_page_list_wo_page_at(priv->page_list, eot_page_index);

			int eot_page_line_index = cha_line_location_wo_get_page_line_index(end_location);
			const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(eot_page, eot_page_line_index, FALSE);

			long long lc = cha_page_list_wo_line_count_till_page(priv->page_list, eot_page_index);
			lc += eot_page_line_index;
			cha_cursorm_wo_set_row(cursor_b, lc);
			cha_cursorm_wo_set_column(cursor_b, utf8_text.text_len);
			cha_utf8_text_cleanup(&utf8_text);
		}
		int end_page_index = cha_line_location_wo_get_page_index(end_location);
		ChaPageWo *e_end_page = cha_revision_wo_editable_page_at(e_revision, end_page_index);
		int end_page_line_index = cha_line_location_wo_get_page_line_index(end_location);
		cat_unref_ptr(end_location);

		ChaLineWo *e_end_line = cha_page_wo_editable_line_at(e_end_page, end_page_line_index);
		CatStringWo *e_end_text = cha_line_wo_get_text(e_end_line);

		cat_log_debug("start_page_index=%d, start_page_line_index=%d, end_page_index=%d, end_page_line_index=%d", start_page_index, start_page_line_index, end_page_index, end_page_line_index);
		if (start_page_index<end_page_index) {
			cat_log_debug("remove pages: start_page_index=%d, end_page_index=%d", start_page_index, end_page_index);

			/* remove lines and combine two pages */

			CatStringWo *start_text = cha_line_wo_get_text(e_start_line);
			cat_log_debug("start_text=%O", start_text);
			start_text = cat_string_wo_new_sub(start_text, 0, cha_cursorm_wo_get_column(cursor_a));
			cat_log_debug("start_text=%O", start_text);
			cat_log_debug("e_end_text=%O", e_end_text);
//			cha_page_list_wo_remove_range(priv->page_list, start_page_line_index, cha_page_wo_line_count(e_start_page));

			cat_string_wo_replace_range_with(e_end_text,0, cha_cursorm_wo_get_column(cursor_b), start_text);
			cat_log_debug("e_end_text=%O", e_end_text);

			cha_page_wo_remove_range(e_start_page, start_page_line_index, cha_page_wo_line_count(e_start_page));

			int lidx;
			int e_end_line_count = cha_page_wo_line_count(e_end_page);
			cat_log_debug("e_end_line_count=%d", e_end_line_count);
			for(lidx=end_page_line_index; lidx<e_end_line_count; lidx++) {
				ChaLineWo *a_app_line = cha_page_wo_line_at(e_end_page, lidx);
				cha_page_wo_add_line(e_start_page, a_app_line);
				cat_unref_ptr(a_app_line);
			}
			cat_unref_ptr(start_text);

			/* remove pages */
			int del_page = end_page_index;
			for(; del_page>start_page_index; del_page--) {
				cha_page_list_wo_remove_page_at(priv->page_list, del_page);
			}
			cat_log_debug("r=%o, page=%o", e_revision, e_start_page);
		} else {
			/* remove multiple lines on single page */
			cat_log_debug("remove multiple lines on single page: start_page_line_index=%d, end_page_line_index=%d", start_page_line_index, end_page_line_index);
			CatStringWo *start_text = cha_line_wo_get_text(e_start_line);

			start_text = cat_string_wo_new_sub(start_text, 0, cha_cursorm_wo_get_column(cursor_a));


			cat_string_wo_replace_range_with(e_end_text,0, cha_cursorm_wo_get_column(cursor_b), start_text);

			cha_page_wo_remove_range(e_start_page, start_page_line_index, end_page_line_index-1);
			cat_unref_ptr(start_text);
		}
	}



	if (modify_form && priv->form) {
		if (cha_form_wo_is_anchored(priv->form)) {
			ChaFormWo *e_form = cha_form_wo_create_editable(priv->form);
			cat_unref_ptr(priv->form);
			priv->form = e_form;
		}


//		int field_idx = 0;
//		int field_count = cha_form_wo_field_count(priv->form);
//		ChaFormFieldWo *modified_form_field = NULL;
//		int modified_form_field_idx = -1;
//		for(field_idx=0; field_idx<field_count; field_idx++) {
//			ChaFormFieldWo *form_field = cha_form_wo_get_at(priv->form, field_idx);
//			cat_log_debug("test fields[%d] : field=%o, original=%o", field_idx, form_field, original);
//			if (cha_form_field_wo_test_cursor_in_field(form_field, original)) {
//				modified_form_field = form_field;
//				modified_form_field_idx = field_idx;
//				break;
//			}
//		}
//
//		cha_form_wo_insert(priv->form, original, e_insert_location);
//
//		if (modified_form_field) {
//			if (info->modified_fields==NULL) {
//				info->modified_fields = cat_bit_array_wo_new();
//				cat_bit_array_wo_insert(info->modified_fields, 0, field_count);
//				cat_bit_array_wo_set(info->modified_fields, modified_form_field_idx);
//			}
//		}



		int field_count = cha_form_wo_field_count(priv->form);
		int field_idx;

		int left_mod_field_index = -1;
		int right_mod_field_index = -1;

		int out_field_idx=0;

		for(field_idx=0; field_idx<field_count; field_idx++, out_field_idx++) {
			ChaFormFieldWo *form_field = cha_form_wo_get_at(priv->form, field_idx);
			ChaFormFieldCoverage ffc = cha_form_field_wo_test_coverage(form_field, cursor_a, cursor_b);
			switch(ffc) {
				case CHA_FFC_NO_OVERLAP : {
					continue;
				}
//				case AST_FFC_SAME_SAME : {
//					field_idx = field_count; /* break loop */
//				} break;
				case CHA_FFC_IS_OVERLAPPED_FULL : {
					out_field_idx--;
				} break;
				case CHA_FFC_RIGHT_OVERLAPPED :  {
					ChaCursorMWo *ff_start = cha_form_field_wo_get_start(form_field);
					ChaCursorOrder ftoso = cha_cursorm_wo_order(ff_start, cursor_a);
					if ((ftoso== CHA_CURSOR_RIGHT) || (ftoso==CHA_CURSOR_BELOW)) {
						left_mod_field_index = out_field_idx;
//						e_left_mod_replace_text = cha_revision_wo_get_lines_between_cursors(e_revision, ff_start, cursor_a);
					} else {
						cat_log_error("AST_FFC_RIGHT_OVERLAPPED was incorrect");
					}
				} break;

				case CHA_FFC_LEFT_OVERLAPPED :  {
					ChaCursorMWo *ff_end = cha_form_field_wo_get_end(form_field);
					ChaCursorOrder ftoeo = cha_cursorm_wo_order(ff_end, cursor_b);
					if ((ftoeo == CHA_CURSOR_LEFT) || (ftoeo == CHA_CURSOR_ABOVE)) {
//						right_mod_replace_text = ast_snapshot_get_lines_between_cursors(snapshot, end, ff_end);
						right_mod_field_index = out_field_idx;
					}
					field_idx = field_count; /* break loop */
				} break;

				case CHA_FFC_SAME_SAME :
				case CHA_FFC_OVERLAPS_FULL : {

//					ChaCursorMWo *ff_start = cha_form_field_wo_get_start(form_field);
//					ChaCursorOrder ftoso = cha_cursorm_wo_order(ff_start, cursor_a);
//					if ((ftoso == CHA_CURSOR_RIGHT) || (ftoso == CHA_CURSOR_BELOW)) {
//						e_left_mod_replace_text = cha_revision_wo_get_lines_between_cursors(e_revision, ff_start, cursor_a);
//					}


					right_mod_field_index = out_field_idx;
					field_idx = field_count; /* break loop */
				} break;
			}
		}

		cha_form_wo_remove(priv->form, cursor_a, cursor_b);


		cat_log_debug("right_mod_field_index=%d, left_mod_field_index=%d", right_mod_field_index, left_mod_field_index);

		if (right_mod_field_index>=0) {

			ChaFormFieldWo *e_form_field = cha_form_wo_editable_field_at(priv->form, right_mod_field_index);
			cha_form_field_wo_mark_as_modified(e_form_field);

		}


		if (left_mod_field_index>=0) {
			ChaFormFieldWo *e_form_field = cha_form_wo_editable_field_at(priv->form, left_mod_field_index);
			cha_form_field_wo_mark_as_modified(e_form_field);

		}

	}
	cat_unref_ptr(cursor_b);
}


CatArrayWo *cha_revision_wo_get_lines_between_cursors(ChaRevisionWo *revision, ChaCursorMWo *start_cursor, ChaCursorMWo *end_cursor) {
	CatArrayWo *e_result = cat_array_wo_new();

	unsigned long long start_row = cha_cursorm_wo_get_row(start_cursor);
	unsigned long long end_row = cha_cursorm_wo_get_row(end_cursor);

	unsigned long long row = start_row;
	ChaLineIterator *line_iterator = cha_line_iterator_new(revision, row);
	cat_log_debug("new iter");
	while(cat_iiterator_has_next((CatIIterator *) line_iterator)) {
		ChaLineLocationWo *line_location = (ChaLineLocationWo *) cat_iiterator_next((CatIIterator *) line_iterator);
		cat_log_debug("next iter:%o", line_location);
		ChaLineWo *e_raw_line = cha_revision_wo_line_at_location(revision, line_location);
		e_raw_line = cha_line_wo_clone(e_raw_line, CAT_CLONE_DEPTH_FULL);

		if (row==end_row) {
			int end_column = cha_cursorm_wo_get_column(end_cursor);
			if (cha_line_wo_byte_count_real(e_raw_line)>end_column) {
				CatStringWo *e_text = cha_line_wo_get_text(e_raw_line);
				cat_string_wo_remove_range(e_text,end_column, cat_string_wo_length(e_text)-end_column);
				cha_line_wo_set_line_end(e_raw_line, CHA_LINE_END_NONE);
			} else {
				cat_unref_ptr(e_raw_line);
			}
		}

		if (row==start_row && e_raw_line!=NULL) {
			int start_column = cha_cursorm_wo_get_column(start_cursor);
			if (start_column>0) {

				if (start_column<=cha_line_wo_byte_count(e_raw_line)) {
					CatStringWo *e_text = cha_line_wo_get_text(e_raw_line);
					cat_string_wo_remove_range(e_text, 0, start_column);
				}
			}
		}

		if (e_raw_line) {
			cat_log_debug("adding line:%o, %o", e_raw_line, cha_line_wo_get_text(e_raw_line));
			e_raw_line = cha_line_wo_anchor(e_raw_line, 0);
			cat_array_wo_append(e_result, (GObject *) e_raw_line);
			cat_unref_ptr(e_raw_line);
		}
		if (row==end_row) {
			break;
		}
		row++;
	}
	cat_unref_ptr(line_iterator);

	return e_result;
}


ChaCursorMWo *cha_revision_wo_to_cursorm(const ChaRevisionWo *revision, const ChaCursorWo *cursor) {
	const ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private((ChaRevisionWo *) revision);
	ChaLineLocationWo *ll = cha_cursor_wo_get_line_location(cursor);
	int page_index = cha_line_location_wo_get_page_index(ll);
	long long lc = cha_page_list_wo_line_count_till_page(priv->page_list, page_index);
	lc += cha_line_location_wo_get_page_line_index(ll);
	return cha_cursorm_wo_new(lc, cha_cursor_wo_get_x_cursor_bytes(cursor));
}

int cha_revision_wo_page_count(const ChaRevisionWo *revision) {
	const ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private((ChaRevisionWo *) revision);
	return cha_page_list_wo_page_count(priv->page_list);
}

long long cha_revision_wo_count_lines(const ChaRevisionWo *revision) {
	const ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private((ChaRevisionWo *) revision);
	return cha_page_list_wo_page_line_count(priv->page_list);
}

long long cha_revision_wo_calculate_row(const ChaRevisionWo *revision, const ChaLineLocationWo *line_location) {
	const ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private((ChaRevisionWo *) revision);
	return cha_page_list_wo_calculate_row(priv->page_list, line_location);
}


ChaPageWo *cha_revision_wo_editable_page_at(ChaRevisionWo *e_revision, int page_idx) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE(NULL);
	if (cha_page_list_wo_is_anchored(priv->page_list)) {
		ChaPageListWo *e_list = cha_page_list_wo_create_editable(priv->page_list);
		cat_unref_ptr(priv->page_list);
		priv->page_list = e_list;
	}

	return cha_page_list_wo_editable_page_at(priv->page_list, page_idx);
}

void cha_revision_wo_set_cursor(ChaRevisionWo *e_revision, ChaCursorWo *new_cursor) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->cursor, new_cursor);
}

ChaCursorWo *cha_revision_wo_get_editable_cursor(ChaRevisionWo *e_revision) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE(NULL);
	if (cha_cursor_wo_is_anchored(priv->cursor)) {
		ChaCursorWo *e_cursor = cha_cursor_wo_create_editable(priv->cursor);
		cat_unref_ptr(priv->cursor);
		priv->cursor = e_cursor;
	}
	return priv->cursor;
}


void cha_revision_wo_clear(ChaRevisionWo *e_revision) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE();
	if (cha_page_list_wo_is_anchored(priv->page_list)) {
		ChaPageListWo *e_list = cha_page_list_wo_create_editable(priv->page_list);
		cat_unref_ptr(priv->page_list);
		priv->page_list = e_list;
	}
	cha_page_list_wo_clear(priv->page_list);
}

gboolean cha_revision_wo_move_lines_to(ChaRevisionWo *e_revision, ChaLineLocationWo *ll_first, ChaLineLocationWo *ll_last, int delta) {
//	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE(FALSE);
	ChaLineLocationWo *rem = NULL;
	ChaLineLocationWo *ins = NULL;
	if (delta==1) {
		rem = cha_line_location_wo_clone(ll_last, CAT_CLONE_DEPTH_MAIN);
		cha_line_location_wo_set_page_line_index(rem, cha_line_location_wo_get_page_line_index(rem)+1);
		if (!cha_line_location_wo_bind_to_revision(rem, e_revision)) {
			cat_unref_ptr(rem);
			return FALSE;
		}
		ins = ll_first;

	} else if (delta == -1) {
		rem = cha_line_location_wo_clone(ll_first, CAT_CLONE_DEPTH_MAIN);
		cha_line_location_wo_set_page_line_index(rem, cha_line_location_wo_get_page_line_index(rem)-1);
		if (!cha_line_location_wo_bind_to_revision(rem, e_revision)) {
			cat_unref_ptr(rem);
			return FALSE;
		}
		ins = ll_last;
	} else {
		return FALSE;
	}

	cat_log_debug("rem=%o, ins=%o", rem, ins, delta);

	int rem_pg_idx = cha_line_location_wo_get_page_index(rem);
	ChaPageWo *e_rem_page = cha_revision_wo_editable_page_at(e_revision, rem_pg_idx);
	cha_page_wo_hold_lines(e_rem_page);
	int rem_pg_line_idx = cha_line_location_wo_get_page_line_index(rem);
	ChaLineWo *line = cha_page_wo_line_at(e_rem_page, rem_pg_line_idx);
	cat_log_debug("remove range:e_rem_page=%o", e_rem_page);
	cha_page_wo_remove_range(e_rem_page, rem_pg_line_idx, rem_pg_line_idx);


	int ins_pg_idx = cha_line_location_wo_get_page_index(ins);
	int ins_pg_line_idx = cha_line_location_wo_get_page_line_index(ins);
	if (rem_pg_idx==ins_pg_idx && ins_pg_line_idx>=rem_pg_line_idx) {
//		ins_pg_line_idx--;
	}

	ChaPageWo *e_ins_page = cha_revision_wo_editable_page_at(e_revision, ins_pg_idx);
	cha_page_wo_hold_lines(e_ins_page);
	cat_log_debug("insert");
	cat_log_debug("remove range:e_ins_page=%o", e_ins_page);
	cha_page_wo_insert_line(e_ins_page, line, ins_pg_line_idx);
//	cat_unref_ptr(line);
	cha_page_wo_release_lines(e_ins_page);
	cha_page_wo_release_lines(e_rem_page);

	cat_unref_ptr(line);
	cat_unref_ptr(rem);

	return TRUE;
}


gboolean cha_revision_wo_lines_remove(ChaRevisionWo *e_revision, long long first_row, long long last_row) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE(FALSE);

	ChaCursorMWo *cursor_a = cha_cursorm_wo_new(first_row, 0);
	ChaCursorMWo *cursor_b = cha_cursorm_wo_new(last_row+1, 0);

	ChaCursorMWo *new_cursor = NULL;
	cha_revision_wo_remove_between_cursors(e_revision, cursor_a, cursor_b, TRUE, &new_cursor);

	if (new_cursor) {
		ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
		ChaLineLocationWo *nll = cha_page_list_wo_calculate_line_location(priv->page_list, cha_cursorm_wo_get_row(new_cursor));
		cha_cursor_wo_set_line_location(e_cursor, nll);
		cha_cursor_wo_set_x_cursor_bytes(e_cursor, cha_cursorm_wo_get_column(new_cursor));
		cat_unref_ptr(new_cursor);
		cat_unref_ptr(nll);
	}

	return TRUE;
}




void cha_revision_wo_append_page(ChaRevisionWo *e_revision, ChaPageWo *page) {
	cat_log_debug("settings page:%o", page);
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
//	CatStringWo *e_buf = cat_string_wo_new();
//	cha_page_list_wo_dump(priv->page_list, e_buf);
//	cat_log_trace("%s", cat_string_wo_getchars(e_buf));

	CHECK_IF_WRITABLE();
	if (cha_page_list_wo_is_anchored(priv->page_list)) {
		ChaPageListWo *e_list = cha_page_list_wo_create_editable(priv->page_list);
		cat_unref_ptr(priv->page_list);
		priv->page_list = e_list;
	}

	cha_page_list_wo_add_page(priv->page_list, page);
}


void cha_revision_wo_set_form(ChaRevisionWo *e_revision, ChaFormWo *form) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->form, form);
}

ChaFormWo *cha_revision_wo_get_form(ChaRevisionWo *revision) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(revision);
	return priv->form;
}

ChaFormWo *cha_revision_wo_get_editable_form(ChaRevisionWo *e_revision) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	CHECK_IF_WRITABLE(NULL);
	if (cha_form_wo_is_anchored(priv->form)) {
		ChaFormWo *nf = cha_form_wo_create_editable(priv->form);
		cat_unref_ptr(priv->form);
		priv->form = nf;
	}
	return priv->form;
}

ChaRevisionChangeFlags cha_revision_wo_compare(const ChaRevisionWo *a_revision1, const ChaRevisionWo *a_revision2) {
	ChaRevisionChangeFlags result = 0;
	ChaRevisionWoPrivate *priv1 = cha_revision_wo_get_instance_private((ChaRevisionWo *) a_revision1);
	ChaRevisionWoPrivate *priv2 = cha_revision_wo_get_instance_private((ChaRevisionWo *) a_revision2);
	if (!cha_form_wo_equal(priv1->form,priv2->form)) {
		result |= CHA_REVISION_CF_FORM;
	}
	if (!cha_cursor_wo_equal(priv1->cursor,priv2->cursor)) {
		result |= CHA_REVISION_CF_CURSOR;
	}
	if (!cha_page_list_wo_equal(priv1->page_list,priv2->page_list)) {
		result |= CHA_REVISION_CF_PAGES;
	}
	return result;
}



ChaRevisionWo *cha_revision_wo_create_editable(ChaRevisionWo *instance) {
	return (ChaRevisionWo *) cat_wo_create_editable((CatWo *) instance);
}

ChaRevisionWo *cha_revision_wo_anchor(ChaRevisionWo *instance, int version) {
	return (ChaRevisionWo *) cat_wo_anchor((CatWo *) instance, version);
}

ChaRevisionWo *cha_revision_wo_get_original(ChaRevisionWo *instance) {
	return (ChaRevisionWo *) cat_wo_get_original((CatWo *) instance);
}

gboolean cha_revision_wo_is_anchored(const ChaRevisionWo *instance) {
	return cat_wo_is_anchored((CatWo *) instance);
}

gboolean cha_revision_wo_equal(const ChaRevisionWo *instance_a, const ChaRevisionWo *instance_b) {
	return cat_wo_equal((const CatWo *) instance_a, (const CatWo *) instance_b);
}

ChaRevisionWo *cha_revision_wo_clone(const ChaRevisionWo *source, CatCloneDepth clone_depth) {
	return (ChaRevisionWo *) cat_wo_clone((CatWo *) source, clone_depth);
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {

	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_REVISION_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private((ChaRevisionWo *) e_uninitialized);
	if (original) {
		ChaRevisionWoPrivate *rpriv = cha_revision_wo_get_instance_private((ChaRevisionWo *) original);
		priv->page_list = cat_ref_ptr(rpriv->page_list);
		priv->enriched_count = 0;
		priv->enrichment_data = cat_ref_ptr(rpriv->enrichment_data);
		priv->cursor = cat_ref_ptr(rpriv->cursor);
		priv->load_token = cat_ref_ptr(rpriv->load_token);
		priv->form = cat_ref_ptr(rpriv->form);
	} else {
		priv->page_list = NULL;
		priv->enriched_count = 0;
		priv->enrichment_data = NULL;
		priv->cursor = NULL;
		priv->load_token = NULL;
		priv->form = NULL;
	}
	return CAT_WO_CLASS(cha_revision_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}





static void l_form_apply_set_field_value(ChaRevisionWo *e_revision, int form_field_index, CatArrayWo *e_field_value, ChaCursorMWo *e_cursorm) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	cat_log_debug("e_revision=%p, form=%p, form_field_index=%d", e_revision, priv->form, form_field_index);
	ChaFormFieldWo *form_field = cha_form_wo_editable_field_at(priv->form, form_field_index);
	ChaCursorMWo *field_start = cha_form_field_wo_get_start(form_field);
	ChaCursorMWo *field_end = cha_form_field_wo_get_end(form_field);
	field_start = cha_cursorm_wo_clone(field_start, CAT_CLONE_DEPTH_AS_ANCHORED);
	field_end = cha_cursorm_wo_clone(field_end, CAT_CLONE_DEPTH_AS_ANCHORED);

	cat_log_debug("before remove: form_field=%o, field_end=%o", form_field, field_end);
//	cat_log_on_trace({ l_dump(snapshot); });

	cha_revision_wo_remove_between_cursors(e_revision, field_start, field_end, FALSE, NULL);

	cat_log_debug("before insert: form_field=%o", form_field);

	ChaCursorMWo *new_field_end = cha_cursorm_wo_clone(field_start, CAT_CLONE_DEPTH_FULL);

	cha_revision_wo_insert_lines(e_revision, new_field_end, e_field_value, FALSE);

//	cat_log_on_trace({ l_dump(result_snapshot); });


	ChaCursorOrder ora = cha_cursorm_wo_order(field_end, new_field_end);
	cat_log_debug("after insert: new_field_end=%o, ora=%d, field_end=%o", new_field_end, ora, field_end);

	switch(ora) {
		case CHA_CURSOR_ABOVE :
		case CHA_CURSOR_LEFT : {
			cha_form_wo_remove(priv->form, new_field_end, field_end);
		} break;
		case CHA_CURSOR_BELOW :
		case CHA_CURSOR_RIGHT : {
			cha_form_wo_insert(priv->form, field_end, new_field_end);
		} break;
		case CHA_CURSOR_SAME : {
		} break;
	}

	ChaCursorOrder orc = cha_cursorm_wo_order(field_end, e_cursorm);
	cat_log_debug("orc=%d, e_cursorm=%o, field_end=%o, form_field=%o",orc, e_cursorm, field_end, form_field);
	switch(orc) {
		case CHA_CURSOR_BELOW : {
			long row_dif = cha_cursorm_wo_get_row(new_field_end) - cha_cursorm_wo_get_row(field_end);
			cha_cursorm_wo_move(e_cursorm, row_dif, 0);
		} break;
		case CHA_CURSOR_SAME :
		case CHA_CURSOR_RIGHT : {
			long long row_dif = cha_cursorm_wo_get_row(new_field_end) - cha_cursorm_wo_get_row(field_end);
			int col_dif = cha_cursorm_wo_get_column(new_field_end) - cha_cursorm_wo_get_column(field_end);
			cha_cursorm_wo_move(e_cursorm, row_dif, col_dif);
		} break;
	}

	cat_log_debug("e_cursorm=%o", e_cursorm);



//	AstSnapshotForm *new_form = ast_snapshot_form_insert(form, field_end, new_field_end);
	cat_log_debug("form=%o", priv->form);
	cat_unref_ptr(field_start);
	cat_unref_ptr(field_end);
}

static void l_form_apply_field_changed(ChaRevisionWo *e_revision, int mod_form_field_offset, ChaCursorMWo *e_cursorm) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	ChaFormWo *form = priv->form;
	ChaFormFieldWo *form_field = cha_form_wo_get_at(form, mod_form_field_offset);
	cat_log_debug("form_field=%o", form_field);

	ChaCursorMWo *field_start = cha_form_field_wo_get_start(form_field);
	ChaCursorMWo *field_end = cha_form_field_wo_get_end(form_field);

	field_start = cha_cursorm_wo_clone(field_start, CAT_CLONE_DEPTH_AS_ANCHORED);
	field_end = cha_cursorm_wo_clone(field_end, CAT_CLONE_DEPTH_AS_ANCHORED);


	CatArrayWo *e_new_field_text = cha_revision_wo_get_lines_between_cursors(e_revision, field_start, field_end);
	cat_log_debug("field_start=%o, field_end=%o, e_new_field_text=%o", field_start, field_end, e_new_field_text);

	int field_index = cha_form_field_wo_get_field_index(form_field);

	int field_offset;
	for(field_offset=cha_form_wo_field_count(form)-1; field_offset>=0; field_offset--) {
		cat_log_debug("field_offset=%d, mod_form_field_offset=%d, form=%p, e_revision=%p", field_offset, mod_form_field_offset, form, e_revision);
		if (mod_form_field_offset == field_offset) {
			continue;
		}
		ChaFormFieldWo *mod_form_field = cha_form_wo_get_at(form, field_offset);
		cat_log_debug("mod_form_field=%o", mod_form_field);
		if (cha_form_field_wo_get_field_index(mod_form_field) == field_index) {
			l_form_apply_set_field_value(e_revision, field_offset, e_new_field_text, e_cursorm);
		}
	}

	cat_unref_ptr(field_start);
	cat_unref_ptr(field_end);
	cat_unref_ptr(e_new_field_text);
	cat_log_debug("done");
}

void cha_revision_wo_apply_form_changes(ChaRevisionWo *e_revision) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(e_revision);
	if (priv->form) {
		int field_count = cha_form_wo_field_count(priv->form);
		if (field_count==0) {
			cat_unref_ptr(priv->form);
		} else {

			ChaCursorMWo *e_cursorm = cha_revision_wo_to_cursorm(e_revision, priv->cursor);
			CatHashSet *fields_done = cat_hash_set_new((GHashFunc) cat_integer_hash, (GEqualFunc) cat_integer_equal);
			int field_idx;
			for(field_idx=0; field_idx<field_count; field_idx++) {
				ChaFormFieldWo *field = cha_form_wo_get_at(priv->form, field_idx);
				cat_log_debug("field=%o", field);
				if (cha_form_field_wo_is_marked_as_modified(field)) {
					int field_index = cha_form_field_wo_get_field_index(field);

					CatInteger *ifi = cat_integer_new(field_index);
					if (cat_hash_set_get(fields_done, (GObject *) ifi)!=NULL) {
						continue;
					}
					cat_hash_set_add(fields_done, (GObject *) ifi);
//						cat_unref_ptr(ifi);

					l_form_apply_field_changed(e_revision, field_idx, e_cursorm);
				}
			}

			ChaLineLocationWo *ll = cha_revision_wo_calculate_line_location(e_revision, cha_cursorm_wo_get_row(e_cursorm));
			ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
			cha_cursor_wo_set_line_location(e_cursor, ll);
			cha_cursor_wo_set_x_cursor_bytes(e_cursor, cha_cursorm_wo_get_column(e_cursorm));
		}
	}
}



static void l_anchor_children(CatWo *wo, int version) {
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private((ChaRevisionWo *) wo);
	if (priv->form) {
		cha_revision_wo_apply_form_changes((ChaRevisionWo *) wo);
		priv->form = cha_form_wo_anchor(priv->form, version);
	}

	priv->cursor = cha_cursor_wo_anchor(priv->cursor, version);
	priv->page_list = cha_page_list_wo_anchor(priv->page_list, version);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaRevisionWoPrivate *priv_a = cha_revision_wo_get_instance_private((ChaRevisionWo *) wo_a);
	const ChaRevisionWoPrivate *priv_b = cha_revision_wo_get_instance_private((ChaRevisionWo *) wo_b);
	if (cha_cursor_wo_equal(priv_a->cursor, priv_b->cursor) &&
			cha_page_list_wo_equal(priv_a->page_list, priv_b->page_list)
			&& cha_form_wo_equal(priv_a->form, priv_b->form)) {
		return TRUE;
	}
	return FALSE;
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_PAGE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(CHA_REVISION_WO(e_uninitialized));
	if (wo_source) {
		ChaRevisionWoPrivate *priv_src = cha_revision_wo_get_instance_private(CHA_REVISION_WO(wo_source));
		priv->cursor = cha_cursor_wo_clone(priv_src->cursor, CAT_CLONE_DEPTH_NONE);
		priv->enriched_count = 0;
		priv->page_list = cha_page_list_wo_clone(priv_src->page_list, CAT_CLONE_DEPTH_NONE);
		priv->version = priv_src->version;
		priv->enrichment_data = cat_ref_ptr(priv->enrichment_data);
		priv->form = cha_form_wo_clone(priv_src->form, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->page_list = cha_page_list_wo_new();
		priv->enriched_count = 0;
		priv->enrichment_data = NULL;
		priv->cursor = cha_cursor_wo_new();
		priv->load_token = NULL;
		priv->form = NULL;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_revision_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaRevisionWo *instance = CHA_REVISION_WO(self);
	ChaRevisionWoPrivate *priv = cha_revision_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s enr-cnt=%d, enr-data=%o, cursor=%o, %o]\npage-list", iname, self, cat_wo_get_version((CatWo *) instance), cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->enriched_count, priv->enrichment_data, priv->cursor, priv->page_list);

//	cha_page_list_wo_dump(priv->page_list, append_to);

}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

