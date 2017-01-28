/*
   File:    chaformwo.c
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

#include "chaformwo.h"
#include "chaformfieldwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaFormWo"
#include <logging/catlog.h>

struct _ChaFormWoPrivate {
	CatArrayWo *fields;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaFormWo, cha_form_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaFormWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static CatWo *l_anchor(CatWo *wo, int version);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static VARIABLE_IS_NOT_USED void l_dump(const ChaFormWo *form);

static void cha_form_wo_class_init(ChaFormWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->anchor = l_anchor;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_form_wo_init(ChaFormWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaFormWo *instance = CHA_FORM_WO(object);
	ChaFormWoPrivate *priv = cha_form_wo_get_instance_private(instance);
	cat_unref_ptr(priv->fields);
	G_OBJECT_CLASS(cha_form_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_form_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaFormWo *cha_form_wo_new() {
	ChaFormWo *result = g_object_new(CHA_TYPE_FORM_WO, NULL);
	cat_ref_anounce(result);
	ChaFormWoPrivate *priv = cha_form_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->fields = cat_array_wo_new();
	return result;
}

ChaFormFieldWo *cha_form_wo_get_at(const ChaFormWo *form, int index) {
	const ChaFormWoPrivate *priv = cha_form_wo_get_instance_private((ChaFormWo *) form);
	return (ChaFormFieldWo *) cat_array_wo_get(priv->fields, index);
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_form)) { \
			cat_log_error("Object is read only:%o", e_form); \
			return rval; \
		} \

ChaFormFieldWo *cha_form_wo_editable_field_at(ChaFormWo *e_form, int index) {
	ChaFormWoPrivate *priv = cha_form_wo_get_instance_private(e_form);
	CHECK_IF_WRITABLE(NULL);
	if (cat_array_wo_is_anchored(priv->fields)) {
		CatArrayWo *e_fields = cat_array_wo_create_editable(priv->fields);
		cat_unref_ptr(priv->fields);
		priv->fields = e_fields;
	}
	ChaFormFieldWo *e_result = (ChaFormFieldWo *) cat_array_wo_get(priv->fields, index);
	if (cha_form_field_wo_is_anchored(e_result)) {
		e_result = cha_form_field_wo_create_editable(e_result);
		cat_array_wo_set(priv->fields, (GObject *) e_result, index, NULL);
		cat_unref(e_result);
	}
	return e_result;
}


int cha_form_wo_field_count(const ChaFormWo *form) {
	const ChaFormWoPrivate *priv = cha_form_wo_get_instance_private((ChaFormWo *) form);
	return cat_array_wo_size(priv->fields);
}


int cha_form_wo_get_index_of_field_at_location(const ChaFormWo *form, const ChaCursorMWo *cursor) {
	const ChaFormWoPrivate *priv = cha_form_wo_get_instance_private((ChaFormWo *) form);
	int idx;
	int count = cat_array_wo_size(priv->fields);
	for(idx=0; idx<count; idx++) {
		ChaFormFieldWo *field = (ChaFormFieldWo *) cat_array_wo_get(priv->fields, idx);
		if (cha_form_field_wo_test_cursor_in_field(field, cursor)) {
			return idx;
		}
	}
	return -1;
}

int cha_form_wo_get_first_index_of_field_at_row(const ChaFormWo *form, long long row) {
	const ChaFormWoPrivate *priv = cha_form_wo_get_instance_private((ChaFormWo *) form);
	int result = -1;
	int idx;
	int count = cat_array_wo_size(priv->fields);
	for(idx=0; idx<count; idx++) {
		ChaFormFieldWo *field = (ChaFormFieldWo *) cat_array_wo_get(priv->fields, idx);
		ChaCursorMWo *cur_start = cha_form_field_wo_get_start(field);
		ChaCursorMWo *cur_end = cha_form_field_wo_get_end(field);
		cat_log_debug("[%d] row=%ld, cur_start=%o, cur_end=%o", idx, row, cur_start, cur_end);
		if (row<cha_cursorm_wo_get_row(cur_start)) {
			break;
		}
		if (row<=cha_cursorm_wo_get_row(cur_end)) {
			result = idx;
			break;
		}
	}
	return result;
}



int cha_form_wo_get_index_of_next_field(const ChaFormWo *form, int current_index) {
	const ChaFormWoPrivate *priv = cha_form_wo_get_instance_private((ChaFormWo *) form);
	ChaFormFieldWo *field = (ChaFormFieldWo *) cat_array_wo_get(priv->fields, current_index);
	int next_field_order = cha_form_field_wo_get_field_index(field)+1;
	int count = cat_array_wo_size(priv->fields);
	int idx = (current_index+1)%count;
	int best_index = -1;
	int best_order = -1;
	while(idx!=current_index) {
		field = (ChaFormFieldWo *) cat_array_wo_get(priv->fields, idx);
		int field_order = cha_form_field_wo_get_field_index(field);
		cat_log_error("idx=%d, current_index=%d, field_order=%d, next_field_order=%d, best_order=%d, best_index=%d", idx, current_index, field_order, next_field_order, best_order, best_index)
		if (field_order == next_field_order) {
			return idx;
		} else if (field_order > next_field_order && (field_order<best_order || best_order==-1)) {
			best_order = next_field_order;
			best_index = idx;
		}
		idx = (idx+1)%count;
	}
	return best_index;
}



void cha_form_wo_insert(ChaFormWo *e_form, ChaCursorMWo *cursor, ChaCursorMWo *new_cursor_placement) {
	g_return_if_fail(e_form!=NULL);
	g_return_if_fail(cursor!=NULL);
	g_return_if_fail(new_cursor_placement!=NULL);

	cat_log_on_debug({
			l_dump(e_form);
		});


	int aco = cha_cursorm_wo_order(cursor, new_cursor_placement);
	if (aco==CHA_CURSOR_SAME) {
		return;
	}


	ChaFormWoPrivate *priv = cha_form_wo_get_instance_private(e_form);
	CHECK_IF_WRITABLE();

	if (cat_array_wo_is_anchored(priv->fields)) {
		CatArrayWo *nfields = cat_array_wo_create_editable(priv->fields);
		cat_unref_ptr(priv->fields);
		priv->fields = nfields;
	}

	CatArrayWo *new_field_list = priv->fields;


//	cat_log_debug("inserting:cursor=%d:%d to=%d:%d", cursor->row, cursor->column, new_cursor_placement->row, new_cursor_placement->column);
//	cat_log_on_debug({
//		ast_snapshot_form_dump(form);
//	});


	cat_ref_ptr(cursor);
	cat_log_debug("cursor=%o", cursor);


	signed long long int delta_row = cha_cursorm_wo_get_row(new_cursor_placement)-cha_cursorm_wo_get_row(cursor);
	int field_idx;
	for(field_idx=cat_array_wo_size(new_field_list)-1; field_idx>=0; field_idx--) {
		ChaFormFieldWo *field = (ChaFormFieldWo *) cat_array_wo_get(priv->fields, field_idx);
		ChaCursorMWo *field_end = cha_form_field_wo_get_end(field);
		ChaCursorOrder ora = cha_cursorm_wo_order(field_end, cursor);
		if ((ora==CHA_CURSOR_RIGHT) || (ora==CHA_CURSOR_BELOW)) {
			break;
		}
		ChaCursorMWo *field_start = cha_form_field_wo_get_start(field);

		ChaCursorMWo *new_field_start = NULL;
		ChaCursorMWo *new_field_end = NULL;

		ora = cha_cursorm_wo_order(field_start, cursor);


		cat_log_debug(" insert-field[%d] ora=%s field=%o, cursor=%o", field_idx, cha_cursorm_wo_order_text(ora), field, cursor);

		if (ora==CHA_CURSOR_SAME) {
			/* special condition: If the cursor is on the start of the field then there might be a preceding field exactly ending at the given cursor.
			 * If such an field exists then the insert was not meant for this field and we need to shift the start of the field. We do so by
			 * setting ora to CHA_CURSOR_LEFT
			 */
			if (field_idx>0) {
				ChaFormFieldWo *prec_field = (ChaFormFieldWo *) cat_array_wo_get(priv->fields, field_idx-1);
				ChaCursorMWo *prec_field_end = cha_form_field_wo_get_end(prec_field);
//				cat_log_debug("prec-field-end=%d:%d, cursor=%d:%d", prec_field_end->row, prec_field_end->column, cursor->row, cursor->column);
				if (cha_cursorm_wo_equal(prec_field_end, cursor)) {
					cat_log_debug("special case");
					ora = CHA_CURSOR_LEFT;
				}
			}
		}


		unsigned long long int cur_row = cha_cursorm_wo_get_row(cursor);
		int cur_column = cha_cursorm_wo_get_column(cursor);

		if (ora==CHA_CURSOR_LEFT || ora==CHA_CURSOR_ABOVE) {
			/* field-start is past the cursor */
			unsigned long long int fld_row = cha_cursorm_wo_get_row(field_start);
			int fld_column = cha_cursorm_wo_get_column(field_start);
			if (fld_row==cur_row) {
				int ncp_column = cha_cursorm_wo_get_column(new_cursor_placement);
				new_field_start = cha_cursorm_wo_new(fld_row+delta_row, fld_column-cur_column+ncp_column);
			} else if (delta_row>0) {
				new_field_start = cha_cursorm_wo_new(fld_row+delta_row, fld_column);
			}
		}

		unsigned long long int fld_row = cha_cursorm_wo_get_row(field_end);
		int fld_column = cha_cursorm_wo_get_column(field_end);
		if (fld_row==cur_row) {
			int ncp_column = cha_cursorm_wo_get_column(new_cursor_placement);
			new_field_end = cha_cursorm_wo_new(fld_row+delta_row, fld_column-cur_column+ncp_column);
		} else if (delta_row>0) {
			new_field_end = cha_cursorm_wo_new(fld_row+delta_row, fld_column);
		}

		if ((new_field_start!=NULL) || (new_field_end!=NULL)) {
			if (new_field_start==NULL) {
				cat_log_debug("using old field start");
				new_field_start = cha_cursorm_wo_clone(field_start, CAT_CLONE_DEPTH_MAIN);
			}
			if (new_field_end==NULL) {
				cat_log_debug("using old field end");
				new_field_end = cha_cursorm_wo_clone(field_end, CAT_CLONE_DEPTH_MAIN);
			}

			ChaFormFieldWo *e_new_form_field = cha_form_field_wo_create_editable(field);
			new_field_start = cha_cursorm_wo_anchor(new_field_start, 0);
			new_field_end = cha_cursorm_wo_anchor(new_field_end, 0);
			cha_form_field_wo_set_start_and_end(e_new_form_field, new_field_start, new_field_end);
			cat_array_wo_set(new_field_list, (GObject *) e_new_form_field, field_idx, NULL);
			cat_unref_ptr(e_new_form_field);
		}
		cat_unref_ptr(new_field_start);
		cat_unref_ptr(new_field_end);
	}
	cat_log_on_debug({
			l_dump(e_form);
		});

	cat_unref_ptr(cursor);
	cat_log_debug("**** cursor=%o", cursor);

}




void cha_form_wo_remove(ChaFormWo *e_form, ChaCursorMWo *rem_start, ChaCursorMWo *rem_end) {
	cat_log_debug("removing:%o - %o", rem_start, rem_end);
	cat_log_on_debug({
		l_dump(e_form);
	});

	ChaFormWoPrivate *priv = cha_form_wo_get_instance_private(e_form);
	CHECK_IF_WRITABLE();

	if (cat_array_wo_is_anchored(priv->fields)) {
		CatArrayWo *nfields = cat_array_wo_create_editable(priv->fields);
		cat_unref_ptr(priv->fields);
		priv->fields = nfields;
	}

	CatArrayWo *new_field_list = priv->fields;


	long long re_row = cha_cursorm_wo_get_row(rem_end);
	int re_column = cha_cursorm_wo_get_column(rem_end);

	int rs_column = cha_cursorm_wo_get_column(rem_start);

	long long delta_row = re_row-cha_cursorm_wo_get_row(rem_start);
	int field_idx;
	for(field_idx=cat_array_wo_size(new_field_list)-1; field_idx>=0; field_idx--) {
		ChaFormFieldWo *field = (ChaFormFieldWo *) cat_array_wo_get(priv->fields, field_idx);
		ChaCursorMWo *field_end = cha_form_field_wo_get_end(field);
		ChaCursorOrder ora = cha_cursorm_wo_order(field_end, rem_start);
		cat_log_debug("field_idx=%d, ora=%d, field=%o, rem_start=%o", field_idx, ora, field, rem_start);
		if (ora==CHA_CURSOR_BELOW || ora==CHA_CURSOR_RIGHT || ora==CHA_CURSOR_SAME) {
			break;
		}

		ChaCursorMWo *field_start = cha_form_field_wo_get_start(field);
		long long fs_row = cha_cursorm_wo_get_row(field_start);
		long long fe_row = cha_cursorm_wo_get_row(field_end);


		int fe_column = cha_cursorm_wo_get_column(field_end);


		ChaCursorMWo *new_field_start = NULL;
		ChaCursorMWo *new_field_end = NULL;


		ChaFormFieldCoverage ffc = cha_form_field_wo_test_coverage(field, rem_start, rem_end);
		cat_log_debug("field_idx=%d, ffc=%d", field_idx, ffc);
		switch(ffc) {
			case CHA_FFC_IS_OVERLAPPED_FULL : {
				cat_array_wo_remove(new_field_list, field_idx, NULL);
			} break;
			case CHA_FFC_SAME_SAME : {
				new_field_start = cat_ref_ptr(field_start);
				new_field_end = cat_ref_ptr(field_start);
			} break;
			case CHA_FFC_NO_OVERLAP : {
				int fs_column = cha_cursorm_wo_get_column(field_start);
				if (fs_row==re_row) {
					int dif = re_column-rs_column;
					new_field_start = cha_cursorm_wo_new(fs_row-delta_row, fs_column-dif);
				} else {
					new_field_start = cha_cursorm_wo_new(fs_row-delta_row, fs_column);
				}

				if (fe_row==re_row) {
					int dif = re_column-rs_column;
					new_field_end = cha_cursorm_wo_new(fe_row-delta_row, fe_column-dif);
				} else {
					new_field_end = cha_cursorm_wo_new(fe_row-delta_row, fe_column);
				}
			} break;
			case CHA_FFC_OVERLAPS_FULL : {
				new_field_start = cat_ref_ptr(field_start);
				if (fe_row==re_row) {
					int dif = re_column-rs_column;
					new_field_end = cha_cursorm_wo_new(fe_row-delta_row, fe_column-dif);
				} else {
					new_field_end = cha_cursorm_wo_new(fe_row-delta_row, fe_column);
				}
			} break;
			case CHA_FFC_RIGHT_OVERLAPPED : {
				new_field_start = cat_ref_ptr(field_start);
				new_field_end = cha_cursorm_wo_clone(rem_start, CAT_CLONE_DEPTH_NONE);
			} break;
			case CHA_FFC_LEFT_OVERLAPPED : {
				new_field_start = cha_cursorm_wo_clone(rem_start, CAT_CLONE_DEPTH_NONE);
				if (fe_row==re_row) {
					int dif = re_column-rs_column;
					new_field_end = cha_cursorm_wo_new(fe_row-delta_row, fe_column-dif);
				} else {
					new_field_end = cha_cursorm_wo_new(fe_row-delta_row, fe_column);
				}
			} break;
		}


		if ((new_field_start!=NULL) && (new_field_end!=NULL)) {
			ChaFormFieldWo *new_form_field = cha_form_field_wo_create_editable(field);
			new_field_start = cha_cursorm_wo_anchor(new_field_start, 0);
			new_field_end = cha_cursorm_wo_anchor(new_field_end, 0);
			cha_form_field_wo_set_start_and_end(new_form_field, new_field_start, new_field_end);
//			new_form_field = cha_form_field_wo_anchor(new_form_field, 0);
			cat_array_wo_set(priv->fields, (GObject *) new_form_field, field_idx, NULL);
			cat_unref_ptr(new_form_field);
		}

		cat_unref_ptr(new_field_start);
		cat_unref_ptr(new_field_end);
	}

	cat_log_on_debug({
		l_dump(e_form);
	});
}


void cha_form_wo_create_field(ChaFormWo *e_form, ChaCursorMWo *c_start_cursor, ChaCursorMWo *c_end_cursor, int field_index, gboolean editable) {
	ChaFormWoPrivate *priv = cha_form_wo_get_instance_private((ChaFormWo *) e_form);
	CHECK_IF_WRITABLE();

	int index;
	int found_index = -1;
	for(index=0; index<cat_array_wo_size(priv->fields); index++) {
		ChaFormFieldWo *test_field = (ChaFormFieldWo *) cat_array_wo_get(priv->fields, index);
		ChaCursorMWo *test_end = cha_form_field_wo_get_end(test_field);
		ChaCursorOrder ora = cha_cursorm_wo_order(c_start_cursor, test_end);

		if ((ora==CHA_CURSOR_RIGHT) || (ora==CHA_CURSOR_BELOW)) {
			found_index = index;
			ChaCursorMWo *test_start = cha_form_field_wo_get_start(test_field);
			ChaCursorOrder ora = cha_cursorm_wo_order(c_end_cursor, test_start);
			if (ora!=CHA_CURSOR_SAME) {
				break;
			} else {
				found_index = -2;	// TODO: even to me it is unclear why and when this occurs
				break;
			}
		}
	}
	if (found_index!=-2) {
		if (cat_array_wo_is_anchored(priv->fields)) {
			CatArrayWo *e_list = cat_array_wo_create_editable(priv->fields);
			cat_unref_ptr(priv->fields);
			priv->fields = e_list;
		}

		ChaFormFieldWo *new_field = cha_form_field_wo_new(c_start_cursor, c_end_cursor, field_index, editable);
		if (found_index>=0) {
			cat_array_wo_insert(priv->fields, (GObject *) new_field, found_index);
		} else {
			cat_array_wo_append(priv->fields, (GObject *) new_field);
		}
		cat_unref_ptr(new_field);
	} else {
		cat_log_error("error creating field");
	}
	cat_log_on_debug({
		l_dump(e_form);
		});
}



static VARIABLE_IS_NOT_USED void l_dump(const ChaFormWo *form) {
	if (form==NULL) {
		cat_log_print("DUMP", "form:<null>");
		return;
	}
	const ChaFormWoPrivate *priv = cha_form_wo_get_instance_private((ChaFormWo *) form);
	cat_log_print("DUMP", "form:%p, field-count=%d", form, cat_array_wo_size(priv->fields))
	CatIIterator *iter = cat_array_wo_iterator(priv->fields);
	int field_idx = 0;
	while(cat_iiterator_has_next(iter)) {
		ChaFormFieldWo *form_field = cat_iiterator_next(iter);
		if (form_field) {
			int start_row = -1;
			int start_col = -1;
			ChaCursorMWo *fstart = cha_form_field_wo_get_start(form_field);
			if (fstart) {
				start_row = (int) cha_cursorm_wo_get_row(fstart);
				start_col = cha_cursorm_wo_get_column(fstart);
			}

			int end_row = -1;
			int end_col = -1;
			ChaCursorMWo *fend = cha_form_field_wo_get_end(form_field);
			if (fend) {
				end_row = (int) cha_cursorm_wo_get_row(fend);
				end_col = cha_cursorm_wo_get_column(fend);
			}

			cat_log_print("DUMP", " field[%d] range[%d:%d - %d:%d], field-index:%d, %p, %s", field_idx, start_row, start_col, end_row, end_col, cha_form_field_wo_get_field_index(form_field), form_field, cat_wo_is_anchored((CatWo *) form_field) ? "anchored" : "editable");
		} else {
			cat_log_print("DUMP", " field[%d] <null>", field_idx);
		}
		field_idx++;
	}
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_FORM_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaFormWoPrivate *priv = cha_form_wo_get_instance_private((ChaFormWo *) e_uninitialized);
	if (original) {
		ChaFormWoPrivate *rpriv = cha_form_wo_get_instance_private((ChaFormWo *) original);
		priv->fields = cat_array_wo_create_editable(rpriv->fields);
	} else {
	}
	return CAT_WO_CLASS(cha_form_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static void l_anchor_children(CatWo *wo, int version) {
	ChaFormWoPrivate *priv = cha_form_wo_get_instance_private((ChaFormWo *) wo);
	priv->fields = cat_array_wo_anchor(priv->fields, version);
}

static CatWo *l_anchor(CatWo *wo, int version) {
	CatWo *result = CAT_WO_CLASS(cha_form_wo_parent_class)->anchor(wo, version);
	cat_log_on_debug({
		l_dump((ChaFormWo *) result);
	});
	return result;
}

static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaFormWoPrivate *priv_a = cha_form_wo_get_instance_private((ChaFormWo *) wo_a);
	const ChaFormWoPrivate *priv_b = cha_form_wo_get_instance_private((ChaFormWo *) wo_b);
	return cat_array_wo_equal(priv_a->fields, priv_b->fields, (GEqualFunc) cha_form_field_wo_equal);
}


static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_FORM_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaFormWoPrivate *priv = cha_form_wo_get_instance_private(CHA_FORM_WO(e_uninitialized));
	if (wo_source) {
		ChaFormWoPrivate *priv_src = cha_form_wo_get_instance_private(CHA_FORM_WO(wo_source));
		priv->fields = cat_array_wo_clone(priv_src->fields, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->fields = cat_array_wo_new();
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_form_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(ChaFormWo,cha_form_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaFormWo *instance = CHA_FORM_WO(self);
	ChaFormWoPrivate *priv = cha_form_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s fields=%o]", iname, self, cat_wo_get_version((CatWo *) self), cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->fields);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
