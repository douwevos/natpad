/*
   File:    chalinelayout.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 23, 2015
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

#include "chalinelayout.h"
#include "chacairosurfacewrapper.h"
#include "chamarking.h"
#include "chapangoattrholder.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaLineLayout"
#include <logging/catlog.h>

struct _ChaLineLayoutPrivate {
	CatStringWo *a_text;
	gboolean do_wrap;
	int tab_size;
	short font_version;
	int view_width;
	ChaLineEnd line_end;
	int selection_version;
	CatArrayWo *selection_attributes;
	gboolean selection_eol;
	CatArrayWo *selection_markings;

	CatLock *lock;

	int width;
	int sub_line_count;

	gboolean is_main_context;

	/* The PangoLayout which only exists while being locked */
	PangoLayout *pango_layout;

	/* cached PangoLayout for use by the main thread only !!! */
	PangoLayout *pango_layout_main_thread;

	int hold_cnt;

	gboolean attributes_dirty;
	int layout_version;

	PangoTabArray *tab_array;

	CatStringWo *preedit_text;
	PangoAttrList *preedit_attrs;
	int preedit_cursor;
	int x_cursor_bytes;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaLineLayout, cha_line_layout, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaLineLayout)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);


static void cha_line_layout_class_init(ChaLineLayoutClass *clazz) {
	clazz->applyExtraAttributes = NULL;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_line_layout_init(ChaLineLayout *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaLineLayout *instance = CHA_LINE_LAYOUT(object);
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(instance);
	cat_unref_ptr(priv->pango_layout);
	cat_unref_ptr(priv->a_text);
	cat_unref_ptr(priv->lock);
	cat_unref_ptr(priv->selection_attributes);
	cat_unref_ptr(priv->selection_markings);
	cat_unref_ptr(priv->pango_layout_main_thread);
	if (priv->tab_array) {
		pango_tab_array_free(priv->tab_array);
		priv->tab_array = NULL;
	}
	cat_unref_ptr(priv->preedit_text);
	if (priv->preedit_attrs) {
		pango_attr_list_unref(priv->preedit_attrs);
		priv->preedit_attrs = NULL;
	}

	G_OBJECT_CLASS(cha_line_layout_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_line_layout_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void cha_line_layout_construct(ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	priv->a_text = NULL;
	priv->line_end = CHA_LINE_END_NONE;
	priv->pango_layout = NULL;
	priv->pango_layout_main_thread = NULL;
	priv->selection_attributes = NULL;
	priv->selection_markings = NULL;
	priv->selection_version = -1;
	priv->lock = cat_lock_new();
	priv->is_main_context = FALSE;
	priv->hold_cnt = 0;
	priv->sub_line_count = -1;
	priv->do_wrap = FALSE;
	priv->view_width = 0;
	priv->tab_size = 3;
	priv->tab_array = NULL;
	priv->font_version = -1;
	priv->preedit_text = NULL;
	priv->preedit_attrs = NULL;

}

ChaLineLayout *cha_line_layout_new() {
	ChaLineLayout *result = g_object_new(CHA_TYPE_LINE_LAYOUT, NULL);
	cat_ref_anounce(result);
	cha_line_layout_construct(result);
	return result;
}


void cha_line_layout_lock(ChaLineLayout *line_layout, gboolean is_main_context) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	cat_lock_lock(priv->lock);
	priv->is_main_context = is_main_context;
	if (is_main_context) {
		priv->pango_layout = priv->pango_layout_main_thread;
		priv->pango_layout_main_thread = NULL;
	}
}

void cha_line_layout_unlock(ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	if (priv->is_main_context && priv->hold_cnt>0) {
		priv->pango_layout_main_thread = priv->pango_layout;
		priv->pango_layout = NULL;
	} else {
		cat_unref_ptr(priv->pango_layout);
	}
	cat_lock_unlock(priv->lock);
}


void cha_line_layout_get_width_and_sub_line_count(ChaLineLayout *line_layout, int *width, int *sub_line_count) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	if (width) {
		*width = priv->width;
	}
	if (sub_line_count) {
		*sub_line_count = priv->sub_line_count;
	}
}

CatArrayWo *cha_line_layout_get_selection_markings(ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	return priv->selection_markings;
}

PangoLayout *cha_line_layout_get_pango_layout(ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	return priv->pango_layout;
}

gboolean cha_line_layout_set_text(ChaLineLayout *line_layout, CatStringWo *a_text, ChaLineEnd line_end, gboolean do_wrap, int tab_size, int view_width, short font_version) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	cat_log_trace("priv->view_width=%d, view_width=%d",priv->view_width, view_width);
	cat_log_debug("[%p] priv->a_text=%p, is_main_context=%d, a_text=%o", line_layout, priv->a_text, priv->is_main_context, a_text);

	if (cat_string_wo_equal(priv->a_text, a_text) && priv->line_end==line_end && priv->do_wrap==do_wrap && priv->tab_size==tab_size && priv->font_version==font_version && (!do_wrap || priv->view_width==view_width)) {
		return priv->sub_line_count==-1;
	}
	CatStringWo *rtext = cat_string_wo_clone(a_text, CAT_CLONE_DEPTH_AS_ANCHORED);
	cat_ref_swap(priv->a_text, rtext);
	cat_unref_ptr(rtext);
	priv->font_version = font_version;
	priv->line_end = line_end;
	priv->view_width = view_width;
	priv->do_wrap = do_wrap;
	if (priv->tab_size!=tab_size) {
		priv->tab_size = tab_size;
		if (priv->tab_array) {
			pango_tab_array_free(priv->tab_array);
			priv->tab_array = NULL;
		}
	}
	cat_log_trace("return  TRUE");

	cat_unref_ptr(priv->pango_layout);
	cat_unref_ptr(priv->pango_layout_main_thread);
	return TRUE;
}

gboolean cha_line_layout_set_preedit(ChaLineLayout *line_layout, const CatStringWo *preedit_text, PangoAttrList *predit_attrs, int preedit_cursor, int x_cursor_bytes) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	priv->x_cursor_bytes = x_cursor_bytes;
	priv->preedit_cursor = preedit_cursor;
	if (cat_string_wo_equal(priv->preedit_text, preedit_text) && priv->preedit_attrs==predit_attrs && priv->preedit_cursor==preedit_cursor && priv->x_cursor_bytes==x_cursor_bytes) {
		return FALSE;
	}
	CatStringWo *atext = cat_string_wo_clone(preedit_text, CAT_CLONE_DEPTH_AS_ANCHORED);
	cat_ref_swap(priv->preedit_text, atext);
	cat_unref_ptr(atext);
	pango_attr_list_ref(predit_attrs);
	pango_attr_list_unref(priv->preedit_attrs);
	priv->preedit_attrs = predit_attrs;
	cat_unref_ptr(priv->pango_layout);
	cat_unref_ptr(priv->pango_layout_main_thread);
	return TRUE;
}

const CatStringWo *cha_line_layout_get_text(const ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private((ChaLineLayout *) line_layout);
	return priv->a_text;
}

ChaLineEnd cha_line_layout_get_line_end(const ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private((ChaLineLayout *) line_layout);
	return priv->line_end;
}


void cha_line_layout_mark_dirty(ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	cat_unref_ptr(priv->pango_layout);
	cat_unref_ptr(priv->pango_layout_main_thread);
}

void cha_line_layout_mark_for_repaint(ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	priv->layout_version++;
}

void cha_line_layout_mark_attributes_dirty(ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	priv->attributes_dirty = TRUE;
}


int cha_line_layout_get_layout_version(const ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private((ChaLineLayout *) line_layout);
	return priv->layout_version;
}



void cha_line_layout_hold(ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	priv->hold_cnt++;
}

void cha_line_layout_release(ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	priv->hold_cnt--;
	if (priv->hold_cnt<=0) {
		cat_unref_ptr(priv->pango_layout_main_thread);
	}
}


static void l_set_attributes(ChaLineLayout *line_layout, PangoLayout *pango_layout, ChaPrefsColorMapWo *color_map) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);

	cat_log_debug("start");

	PangoAttrList *pango_attrs = pango_attr_list_new();


	ChaColorEntryInt fg_col = cha_prefs_color_map_wo_get_color_int(color_map, CHA_COLOR_FOREGROUND);

	PangoAttribute *pango_attr = pango_attr_foreground_new(fg_col.red, fg_col.green, fg_col.blue);
	pango_attr->start_index = 0;
	pango_attr->end_index = 100000;

	pango_attr_list_insert(pango_attrs, pango_attr);

	ChaLineLayoutClass *ll_class = CHA_LINE_LAYOUT_GET_CLASS(line_layout);
	cat_log_debug("appl-extra");
	if (ll_class->applyExtraAttributes) {
		ll_class->applyExtraAttributes(line_layout, pango_attrs, color_map);
	}

	cat_log_debug("selection_attr");
	if (priv->selection_attributes) {
		CatIIterator *iter = cat_array_wo_iterator(priv->selection_attributes);
		while(cat_iiterator_has_next(iter)) {
			cat_log_debug("--attrxx");
			ChaPangoAttrHolder *holder = (ChaPangoAttrHolder *) cat_iiterator_next(iter);
			PangoAttribute *attr = (PangoAttribute *) cha_pango_attr_holder_get(holder);
			if (attr->start_index>=attr->end_index) {
//				cat_log_fatal("start_index:%d, end_index:%d", attr->start_index, attr->end_index);
				continue;
			}
			cat_log_debug("attr=%o, start=%d, end=%d", attr, attr->start_index, attr->end_index);
			pango_attr_list_change(pango_attrs, pango_attribute_copy(attr));
		}
		cat_unref_ptr(iter);
	}


	cat_log_on_debug({
		PangoAttrIterator *iter = pango_attr_list_get_iterator(pango_attrs);
		while(pango_attr_iterator_next(iter)) {
			cat_log_debug("--attr");
			GSList *attrs = pango_attr_iterator_get_attrs(iter);
			while(attrs) {
				PangoAttribute *attr = (PangoAttribute *) attrs->data;
				cat_log_debug("attr=%o, start=%d, end=%d", attr, attr->start_index, attr->end_index);
				attrs = attrs->next;
			}
		}
		pango_attr_iterator_destroy(iter);
	})


	cat_log_debug("predit_attrs");

	if (priv->preedit_attrs) {
	      pango_attr_list_splice(pango_attrs, priv->preedit_attrs, priv->x_cursor_bytes, cat_string_wo_length(priv->preedit_text));
	}

	pango_layout_set_attributes(pango_layout, pango_attrs);

	pango_attr_list_unref(pango_attrs);
}

PangoLayout *cha_line_layout_update_pango_layout(ChaLineLayout *line_layout, PangoContext *pango_context) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	if (priv->pango_layout==NULL) {
		PangoLayout *pango_layout = pango_layout_new(pango_context);
		pango_layout_set_single_paragraph_mode(pango_layout, TRUE);
		cat_log_trace("pango_layout=%p, priv->do_wrap=%d, view_width=%d", pango_layout,priv->do_wrap, priv->view_width);
		if (priv->do_wrap) {
			pango_layout_set_wrap(pango_layout, PANGO_WRAP_WORD);
			pango_layout_set_width(pango_layout, priv->view_width*PANGO_SCALE);
		} else {
			pango_layout_set_width(pango_layout, -1);
		}
		if (priv->tab_array==NULL) {
			priv->tab_array = pango_tab_array_new(1, FALSE);
			pango_tab_array_set_tab(priv->tab_array, 0, PANGO_TAB_LEFT, priv->tab_size);
		}
		pango_layout_set_tabs(pango_layout, priv->tab_array);
		if (priv->preedit_text!=NULL) {

			CatStringWo *e_full_text = cat_string_wo_clone(priv->a_text, CAT_CLONE_DEPTH_MAIN);
			cat_string_wo_insert(e_full_text, priv->preedit_text, priv->x_cursor_bytes);
			pango_layout_set_text(pango_layout, cat_string_wo_getchars(e_full_text), cat_string_wo_length(e_full_text));
			cat_unref_ptr(e_full_text);
		} else {
			pango_layout_set_text(pango_layout, cat_string_wo_getchars(priv->a_text), cat_string_wo_length(priv->a_text));
		}

		cat_log_on_debug({
			if (cat_time_running_in_ms()>10000) {
				cat_log_info("text=%p", priv->a_text);
			}
		});

		PangoRectangle inkt_rect;
		pango_layout_get_pixel_extents(pango_layout, NULL, &inkt_rect);
		priv->width = inkt_rect.width;
		priv->sub_line_count = pango_layout_get_line_count(pango_layout);
		priv->pango_layout = pango_layout;
		priv->attributes_dirty = TRUE;
	}
	return priv->pango_layout;
}


void cha_line_layout_update(ChaLineLayout *line_layout, PangoContext *pango_context, ChaPrefsColorMapWo *color_map) {
	cha_line_layout_update_pango_layout(line_layout, pango_context);
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	if (priv->is_main_context && priv->attributes_dirty) {
		l_set_attributes(line_layout, priv->pango_layout, color_map);
		priv->attributes_dirty = FALSE;
	}
}


int cha_line_layout_get_selection_version(ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	return priv->selection_version;
}

gboolean cha_line_layout_get_selection_eol(ChaLineLayout *line_layout) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	return priv->selection_eol;
}


void cha_line_layout_set_selection(ChaLineLayout *line_layout, CatArrayWo *c_selection_attributes, gboolean selection_eol, CatArrayWo *c_selection_markings, int selection_version) {
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(line_layout);
	if (c_selection_attributes == NULL && selection_eol==FALSE) {
		selection_version = -1;
	} else {
		if (cat_array_wo_equal(priv->selection_attributes, c_selection_attributes, (GEqualFunc) cha_pango_attr_holder_equal)
				&& cat_array_wo_equal(priv->selection_markings, c_selection_markings, (GEqualFunc) cha_marking_equal)
				&& priv->selection_eol==selection_eol) {
			priv->selection_version = selection_version;
		}
	}
	if (priv->selection_version==selection_version) {
		cat_unref_ptr(c_selection_attributes);
		cat_unref_ptr(c_selection_markings);
		return;
	}
	cat_unref_ptr(priv->selection_attributes);
	priv->selection_attributes = c_selection_attributes;
	priv->selection_eol = selection_eol;
	cat_unref_ptr(priv->selection_markings);
	priv->selection_markings = c_selection_markings;
	priv->selection_version = selection_version;
	priv->attributes_dirty = TRUE;
	priv->layout_version++;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaLineLayout *instance = CHA_LINE_LAYOUT(self);
	ChaLineLayoutPrivate *priv = cha_line_layout_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %s text=%o]", iname, self, cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->a_text);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

