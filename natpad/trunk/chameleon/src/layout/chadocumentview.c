/*
   File:    chadocumentview.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 31, 2015
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

#include "chadocumentview.h"

#include <cairo.h>
#include <catiiterator.h>
#include <catistringable.h>
#include <catlib.h>
#include <concurrent/catatomicinteger.h>
#include <concurrent/catweaklist.h>
#include <gdk/gdktypes.h>
#include <glib/gmacros.h>
#include <glib/gmem.h>
#include <glib/gslist.h>
#include <glib/gstrfuncs.h>
#include <glib/gtypes.h>
#include <gobject/gobject.h>
#include <gobject/gtype.h>
#include <gtk/gtkadjustment.h>
#include <gtk/gtktypes.h>
#include <gtk/gtkwidget.h>
#include <math.h>
#include <pango/pango-context.h>
#include <pango/pango-font.h>
#include <pango/pango-fontmap.h>
#include <pango/pango-glyph-item.h>
#include <pango/pango-layout.h>
#include <pango/pango-tabs.h>
#include <pango/pango-types.h>
#include <string.h>
#include <woo/catarraywo.h>
#include <woo/catstringwo.h>
#include <woo/catwo.h>
#include <worrequest.h>

#include "../document/chacursormwo.h"
#include "../document/chacursorwo.h"
#include "../document/chaenrichmentdatamapwo.h"
#include "../document/chaformfieldwo.h"
#include "../document/chaformwo.h"
#include "../document/chaidocumentlistener.h"
#include "../document/chalinelocationwo.h"
#include "../document/chalinewo.h"
#include "../document/chapagewo.h"
#include "../document/charevisionwo.h"
#include "../io/chadocumentmanager.h"
#include "../preferences/chaprefscolormapwo.h"
#include "chapagelayoutrequest.h"
#include "../selection/chaselectionprivate.h"
#include "chasurfacepool.h"

#define FORCE_FULL_PAINT 0

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaDocumentView"
#include <logging/catlog.h>

struct _ChaDocumentViewPrivate {
	ChaDocument *document;
	CatWeakList *listeners;
	ChaPreferencesWo *a_preferences;
	ChaPrefsColorMapWo *color_map;
	PangoContext *pango_context;
	PangoLayout *pango_layout_no_wrap;
	PangoLayout *pango_layout_wrap;

	ChaDocumentViewContext ctx;
	int invalidate_count;

	GtkWidget *widget;
	ChaSurfacePool *surface_pool;

	CatArrayWo *cached_lines;
	CatStringWo *a_slot_key;
	int last_slot_key_idx;

	int marked_layout_x_cursor;
	ChaCursorWo *last_cursor;

	long long int view_x, view_y;
	int view_width, view_height;
	long long int layout_height;
	int layout_width;

	GtkAdjustment *hadjustment;
	GtkAdjustment *vadjustment;
	CatAtomicInteger *selection_sequence;
	ChaSelection *selection;
	int selection_mod_count;


	int last_revision_page_list_version;
	CatArrayWo *pages_on_hold;
	CatArrayWo *sub_line_cache;

	long long cache_view_y;
	int cache_view_height;

	ChaRevisionWo *a_revision_wo;
	ChaEditMode edit_mode;
	gboolean highlight_current_line;
	gboolean has_focus;
	gboolean cursor_blink_up;
	gboolean in_scroll;

    const CatStringWo *preedit_string;
    gint predit_cursor_pos;
    PangoAttrList *preedit_attrs;

};

static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaDocumentView, cha_document_view, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaDocumentView)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_IDOCUMENT_LISTENER, l_document_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_relayout_pages(ChaDocumentView *document_view, ChaRevisionWo *a_rev, const char *src);
static gboolean l_post_layout_requests(ChaDocumentView *document_view, ChaRevisionWo *a_rev);
static void l_invalidate_revision(ChaDocumentView *document_view, ChaRevisionWo *a_rev);

static void cha_document_view_class_init(ChaDocumentViewClass *clazz) {
	clazz->createSubLineCache = NULL;
	clazz->createLineLayout = NULL;
	clazz->onInvalidateLine = NULL;
	clazz->createRichText = NULL;
	clazz->invalidateRevision = l_invalidate_revision;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_document_view_init(ChaDocumentView *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaDocumentView *instance = CHA_DOCUMENT_VIEW(object);
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(instance);
	if (priv->pages_on_hold) {
		CatIIterator *iter = cat_array_wo_iterator(priv->pages_on_hold);
		while(cat_iiterator_has_next(iter)) {
			ChaPageWo *page_on_hold = (ChaPageWo *) cat_iiterator_next(iter);
			cha_page_wo_release_lines(page_on_hold);
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(priv->pages_on_hold);
	}

	cat_unref_ptr(priv->listeners);
	cat_unref_ptr(priv->a_preferences);
	cat_unref_ptr(priv->document);
	cat_unref_ptr(priv->color_map);
	cat_unref_ptr(priv->surface_pool);
	cat_unref_ptr(priv->selection);
	cat_unref_ptr(priv->a_slot_key);
	cat_unref_ptr(priv->cached_lines);
	cat_unref_ptr(priv->last_cursor);
	cat_unref_ptr(priv->pango_layout_no_wrap);
	cat_unref_ptr(priv->pango_layout_wrap);
	cat_unref_ptr(priv->pango_context);
	cat_unref_ptr(priv->sub_line_cache);
	cat_unref_ptr(priv->selection_sequence);
	cat_unref_ptr(priv->a_revision_wo);

	if (priv->ctx.tab_array) {
		pango_tab_array_free(priv->ctx.tab_array);
		priv->ctx.tab_array = 0;
	}

	G_OBJECT_CLASS(cha_document_view_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_document_view_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_apply_preferences(ChaDocumentView *document_view);

void cha_document_view_construct(ChaDocumentView *document_view, ChaDocument *document, PangoContext *pango_context, GtkWidget *widget) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	priv->listeners = cat_weak_list_new();
	priv->document = cat_ref_ptr(document);
	priv->a_preferences = NULL;
	priv->pango_context = cat_ref_ptr(pango_context);
	priv->pango_layout_no_wrap = pango_layout_new(pango_context);
	priv->pango_layout_wrap = pango_layout_new(pango_context);
	pango_layout_set_single_paragraph_mode(priv->pango_layout_no_wrap, TRUE);
	pango_layout_set_single_paragraph_mode(priv->pango_layout_wrap, TRUE);
	priv->surface_pool = cha_surface_pool_new();
	priv->last_revision_page_list_version = -1;
	priv->widget = widget;
	priv->cached_lines = NULL;
	priv->last_cursor = NULL;
	priv->view_x = 0;
	priv->view_y = 0;
	priv->layout_height = 0;
	priv->view_width = 0;
	priv->view_height = 0;
	priv->selection_sequence = cat_atomic_integer_new();
	priv->selection = NULL;
	priv->selection_mod_count = -1;
	priv->sub_line_cache = NULL;
	priv->has_focus = TRUE;
	priv->in_scroll = FALSE;
	priv->cursor_blink_up = TRUE;
	priv->pages_on_hold = NULL;
	priv->highlight_current_line = TRUE;
	priv->edit_mode = CHA_EDIT_MODE_INSERT_CONTEXT;
	priv->a_slot_key = cat_string_wo_new();
	cat_string_wo_format(priv->a_slot_key, "%p", document_view);
	priv->a_slot_key = cat_string_wo_anchor(priv->a_slot_key, 0);

	priv->ctx.font_version = 0;
	priv->ctx.wrap_lines = FALSE;
	priv->ctx.show_line_numbers = TRUE;
	priv->ctx.show_whitespace = FALSE;
	priv->ctx.tab_size = priv->ctx.digit_width*8;

	priv->ctx.line_nr_view_width = 100;
	priv->ctx.text_view_width = 0;
	priv->invalidate_count = 0;

	priv->a_revision_wo = NULL;
	priv->color_map = NULL;
	priv->preedit_attrs = NULL;
	priv->preedit_string = NULL;

	l_apply_preferences(document_view);


	cha_document_add_listener(document, (ChaIDocumentListener *) document_view);
	priv->last_slot_key_idx = -1;
	priv->last_slot_key_idx = cha_document_register_enrichment_slot(priv->document, (GObject *) priv->a_slot_key);
}


ChaDocumentView *cha_document_view_new(ChaDocument *document, PangoContext *pango_context, GtkWidget *widget) {
	ChaDocumentView *result = g_object_new(CHA_TYPE_DOCUMENT_VIEW, NULL);
	cat_ref_anounce(result);
	cha_document_view_construct(result, document, pango_context, widget);
	return result;
}

void cha_document_view_set_in_scroll(ChaDocumentView *document_view, gboolean in_scroll) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	priv->in_scroll = in_scroll;
}


void cha_document_view_set_preedit(ChaDocumentView *document_view, const CatStringWo *predit_text, PangoAttrList *preedit_attrs, int cursor_pos) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);

	if (priv->preedit_attrs) {
		pango_attr_list_unref(priv->preedit_attrs);
		priv->preedit_attrs = NULL;
	}
	cat_unref_ptr(priv->preedit_string);

	priv->preedit_string = predit_text;
	priv->preedit_attrs = preedit_attrs;
	priv->predit_cursor_pos = cursor_pos;
	cha_document_view_invalidate_lines(document_view);
}

const CatStringWo *cha_document_view_get_preedit(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->preedit_string;
}

static gboolean l_initialize_font(ChaDocumentView *document_view);

static void l_apply_preferences(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	ChaPreferencesWo *a_preferences = priv->a_preferences;
	int tab_size = 8;
	gboolean n_wrap_lines = FALSE;
	gboolean n_show_line_numbers = TRUE;
	gboolean n_show_whitespace = FALSE;
	gboolean n_limit_cursor = TRUE;
	gboolean n_highlight_current_line = TRUE;
	ChaPrefsColorMapWo *n_color_map = NULL;


	CatStringWo *n_font_name = NULL;
	if (a_preferences) {
		n_wrap_lines = cha_preferences_wo_get_wrap_lines(a_preferences);
		n_show_line_numbers = cha_preferences_wo_get_show_line_numbers(a_preferences);
		n_show_whitespace = cha_preferences_wo_get_show_whitespace(a_preferences);
		n_font_name = cha_preferences_wo_get_font_name(a_preferences);
		n_limit_cursor = cha_preferences_wo_get_limit_cursor(a_preferences);
		n_highlight_current_line = cha_preferences_wo_get_highlight_current_line(a_preferences);
		tab_size = cha_preferences_wo_get_tab_size(a_preferences);
		n_color_map = cha_preferences_wo_get_color_map(a_preferences);
	}
	if (n_color_map==NULL) {
		n_color_map = cha_prefs_color_map_wo_new();
	} else {
		cat_ref_ptr(n_color_map);
	}

	priv->ctx.wrap_lines = n_wrap_lines;
	priv->ctx.show_line_numbers = n_show_line_numbers;
	priv->ctx.show_whitespace = n_show_whitespace;
	priv->ctx.limit_cursor = n_limit_cursor;

	priv->highlight_current_line = n_highlight_current_line;

	gboolean col_map_ch = cha_prefs_color_map_wo_equal(n_color_map, priv->color_map);
	cat_ref_swap(priv->color_map, n_color_map);
	cat_unref_ptr(n_color_map);
	if (!col_map_ch) {
		if (priv->cached_lines) {
			cat_array_wo_clear(priv->cached_lines);
		}
		priv->ctx.font_version++;
	}

	gboolean font_was_initialized = FALSE;

	PangoFontDescription *old_font_descr = pango_context_get_font_description(priv->pango_context);

	old_font_descr = pango_font_description_copy(old_font_descr);

	cat_log_debug("n_font_name=%o, old_font_descr=%s", n_font_name, pango_font_description_get_family(old_font_descr));

	if (n_font_name) {
		PangoFontDescription *new_font_descr = pango_font_description_from_string(cat_string_wo_getchars(n_font_name));
		if (new_font_descr) {
			if (!pango_font_description_equal(new_font_descr, old_font_descr)) {
				pango_context_set_font_description(priv->pango_context, new_font_descr);
				pango_context_changed(priv->pango_context);
				font_was_initialized = l_initialize_font(document_view);
			}
		}
	} else {
		PangoFontDescription *mfd = pango_font_description_copy(old_font_descr);
		PangoFontDescription *new_font_descr = pango_font_description_from_string("monospace");
		pango_font_description_merge(mfd, new_font_descr, TRUE);
		if (pango_font_description_equal(new_font_descr, mfd)) {
			cat_log_debug("merging did not create new font");
			mfd = pango_font_description_from_string("monospace");
		}
		cat_log_debug("mfd=%s", pango_font_description_get_family(mfd));
		pango_context_set_font_description(priv->pango_context, mfd);
		pango_context_changed(priv->pango_context);
		font_was_initialized = l_initialize_font(document_view);
	}

	if (!font_was_initialized) {
		pango_context_set_font_description(priv->pango_context, old_font_descr);
		l_initialize_font(document_view);
	}

	pango_font_description_free(old_font_descr);

	PangoLayout *layout = priv->pango_layout_no_wrap;
	char *str = g_strnfill(tab_size, ' ');
	pango_layout_set_text(layout, str, -1);
	g_free (str);
	int tab_width = 0;
	pango_layout_get_size (layout, &tab_width, NULL);
	priv->ctx.tab_size = tab_width;

	if (priv->ctx.tab_array) {
		pango_tab_array_free(priv->ctx.tab_array);
		priv->ctx.tab_array = 0;
	}
	PangoTabArray *tabs = pango_tab_array_new(1, FALSE);
	pango_tab_array_set_tab(tabs, 0, PANGO_TAB_LEFT, tab_width);
	priv->ctx.tab_array = tabs;

	if (priv->a_revision_wo) {
		l_run_relayout_pages(document_view, priv->a_revision_wo, "apply-prefs");
		cha_document_view_invalidate_lines(document_view);
	}
}


void cha_document_view_set_preferences(ChaDocumentView *document_view, ChaPreferencesWo *a_preferences) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	if (priv->a_preferences == a_preferences) {
		return;
	}
	cat_ref_swap(priv->a_preferences, a_preferences);
	l_apply_preferences(document_view);
}


static gboolean l_initialize_font(ChaDocumentView *document_view) {
	gboolean result = FALSE;
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	priv->ctx.font_version++;
	PangoContext *pango_context = priv->pango_context;

	PangoFontDescription *font_desc = pango_context_get_font_description(pango_context);


	const char *font_fam = pango_font_description_get_family(font_desc);
	int nr_families = 0;
	PangoFontFamily **families = NULL;
	pango_context_list_families(pango_context, (&families), &nr_families);

	int sidf;
	priv->ctx.is_mono_space = FALSE;
	for(sidf=0; sidf<nr_families; sidf++) {
		PangoFontFamily *font_family = families[sidf];
		cat_log_debug("pango-ff=%s, font_fam=%s", pango_font_family_get_name(font_family), font_fam);

		if (strcasecmp(pango_font_family_get_name(font_family), font_fam)==0) {
			priv->ctx.is_mono_space = pango_font_family_is_monospace(font_family);
			result = TRUE;
			cat_log_debug("^^^^^^^^^^^");
//			break;
		}
	}

	PangoRectangle inkt_rect;
	cat_log_debug("is-mono-space=%d", priv->ctx.is_mono_space);
	pango_layout_set_text(priv->pango_layout_no_wrap, "a", 1);
	pango_layout_get_pixel_extents(priv->pango_layout_no_wrap, NULL, &inkt_rect);
	priv->ctx.def_char_width = inkt_rect.width;

	pango_layout_set_text(priv->pango_layout_no_wrap, " ", 1);
	pango_layout_get_extents(priv->pango_layout_no_wrap, NULL, &inkt_rect);
	priv->ctx.ps_space_width = inkt_rect.width;

	pango_layout_set_text(priv->pango_layout_no_wrap, "0123456789", 10);
	PangoLayoutLine *pango_line = pango_layout_get_line(priv->pango_layout_no_wrap, 0);
	GSList *runs = pango_line->runs;
	PangoGlyphItem *itm = (PangoGlyphItem *) runs->data;
	int ws[10];
	pango_glyph_item_get_logical_widths(itm, "0123456789", ws);
	int widest_digit = 0;
	int idx;
	for(idx=0; idx<9; idx++) {
		int w = ws[idx];
		cat_log_debug("w=%d",w);
		if (w>widest_digit) {
			widest_digit = w;
		}
	}
	priv->ctx.digit_width = widest_digit;


//	pango_layout_set_text(priv->pango_layout_no_wrap, "⎲ ⎳ ⎷pPjI^", -1);

	pango_layout_get_pixel_extents(priv->pango_layout_no_wrap, NULL, &inkt_rect);
	priv->ctx.line_height = inkt_rect.height;

	cat_log_debug("line-height=%d", priv->ctx.line_height);


	if (priv->surface_pool) {
		cha_surface_pool_set_image_height(priv->surface_pool, priv->ctx.line_height);
	}
	return result;
}

void cha_document_view_set_edit_mode(ChaDocumentView *document_view, ChaEditMode edit_mode) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	if (priv->edit_mode!=edit_mode) {
		cat_log_debug("new edit mode=%d", edit_mode);
		priv->edit_mode = edit_mode;
		CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
		while(cat_iiterator_has_next(iter)) {
			ChaIDocumentViewListener *listener = (ChaIDocumentViewListener *) cat_iiterator_next(iter);
			ChaIDocumentViewListenerInterface *iface = CHA_IDOCUMENT_VIEW_LISTENER_GET_INTERFACE(listener);
			if (iface->editModeChanged) {
				iface->editModeChanged(listener, edit_mode);
			}
		}
		cat_unref_ptr(iter);
	}
}


void cha_document_view_set_has_focus(ChaDocumentView *document_view, gboolean has_focus) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	if (priv->has_focus!=has_focus) {
		priv->has_focus = has_focus;
		cha_document_view_invalidate_lines(document_view);
	}
}

gboolean cha_document_view_set_cursor_blink_up(ChaDocumentView *document_view, gboolean blink_up) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	gboolean result = FALSE;
	if (priv->cursor_blink_up!=blink_up) {
		priv->cursor_blink_up = blink_up;
		result = priv->has_focus;
	}
	return result;
}


ChaEditMode cha_document_view_get_edit_mode(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->edit_mode;
}

ChaPrefsColorMapWo *cha_document_view_get_color_map(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->color_map;
}

CatStringWo *cha_document_view_get_slot_key(ChaDocumentView *document_view, int *slot_index) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	CatStringWo *result = priv->a_slot_key;

	if (slot_index) {
		priv->last_slot_key_idx = cha_revision_wo_get_slot_index(priv->a_revision_wo, (GObject *) result, priv->last_slot_key_idx);
		*slot_index = priv->last_slot_key_idx;
	}
	return result;
}


void cha_document_view_add_listener(ChaDocumentView *document_view, ChaIDocumentViewListener *listener) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	cat_weak_list_append_once(priv->listeners, (GObject *) listener);
}

void cha_document_view_remove_listener(ChaDocumentView *document_view, ChaIDocumentViewListener *listener) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	cat_weak_list_remove(priv->listeners, (GObject *) listener);
}

void cha_document_view_notify_selection_changed(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
	while(cat_iiterator_has_next(iter)) {
		ChaIDocumentViewListener *listener = (ChaIDocumentViewListener *) cat_iiterator_next(iter);
		cha_idocument_view_listener_selection_changed(listener, priv->selection);
	}
	cat_unref_ptr(iter);
}


void cha_document_view_get_values(ChaDocumentView *document_view, gboolean *wrap_lines, gboolean *is_mono_space, gboolean *is_big_mode, int *def_char_width, int *line_height) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	if (wrap_lines) {
		*wrap_lines = priv->ctx.wrap_lines;
	}
	if (is_mono_space) {
		*is_mono_space = priv->ctx.is_mono_space;
	}
	if (is_big_mode) {
		*is_big_mode = cha_document_is_big_file_mode(priv->document);
	}
	if (def_char_width) {
		*def_char_width = priv->ctx.def_char_width;
	}
	if (line_height) {
		*line_height = priv->ctx.line_height;
	}
}

const ChaDocumentViewContext cha_document_view_get_context(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->ctx;
}

ChaRevisionWo *cha_document_view_get_revision(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->a_revision_wo;
}


int cha_document_view_get_visible_lines(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->view_height/priv->ctx.line_height;
}


void cha_document_view_set_adjustments(ChaDocumentView *document_view, GtkAdjustment *hadjustment, GtkAdjustment *vadjustment) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	priv->hadjustment = hadjustment;
	priv->vadjustment = vadjustment;

}


gboolean cha_document_view_layout_page(ChaDocumentView *document_view, ChaPageWo *a_page, ChaPageLayoutContext *page_layout_context, ChaPageLayout *page_layout, gboolean test_for_concurrent_update) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	PangoContext *pango_context = cha_document_view_get_pango_context(document_view);
	PangoLayout *pango_layout = NULL;
	gboolean is_mono_space = FALSE;
	gboolean is_big_mode = FALSE;
	gboolean wrap_lines = FALSE;

	int page_height = 0;
	int page_width = 0;
	int line_width=0;
	int sub_line_count=0;
	int single_line_height = 0;
	int char_width;

	cha_document_view_get_values(document_view, &wrap_lines, &is_mono_space, &is_big_mode, &char_width, &single_line_height);

	const ChaDocumentViewContext view_ctx = cha_document_view_get_context(document_view);

	int text_layout_width = view_ctx.text_layout_width;
	int page_version = cat_wo_get_version((CatWo *) a_page);


	if (is_big_mode) {
		pango_layout = pango_layout_new(pango_context);
		pango_layout_set_single_paragraph_mode(pango_layout, TRUE);
		pango_layout_set_tabs(pango_layout, view_ctx.tab_array);
		pango_layout_set_width(pango_layout, text_layout_width);
	}

//	cat_log_error("is_big_mode=%d, wrap_lines=%d, line_height=%d", is_big_mode, wrap_lines, line_height);

	cha_page_wo_hold_lines(a_page);

	gboolean is_updating = TRUE;

	int line_cnt = cha_page_wo_line_count(a_page);
	cat_log_detail("line_cnt=%d", line_cnt);
	if (line_cnt>0) {
		gboolean allow_fast_track = FALSE;
		if ((is_big_mode || !wrap_lines) && is_mono_space) {
			allow_fast_track = TRUE;
		}
		int line_idx;
		for(line_idx=0; is_updating && line_idx<line_cnt; line_idx++) {
			if (is_big_mode) {
				/* run the fast road */
				const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(a_page, line_idx, allow_fast_track);

				cat_log_trace("do_fast_layout=%d", do_fast_layout);

				if (allow_fast_track && utf8_text.only_ascii) {
					if (!utf8_text.has_tabs) {
						line_width = char_width * utf8_text.text_len;
						sub_line_count = 1;
					} else {

						int out = 0;
						const gchar *the_text = utf8_text.text;
						int in;
						for(in=0; in<utf8_text.text_len; in++) {
							gchar ch = the_text[in];
							if (ch==0x9) {
								out += view_ctx.tab_size;
								out = out - (out % view_ctx.tab_size);
							} else {
								out += char_width;
							}
						}
						line_width = out;
						sub_line_count = 1;
					}
				} else {
					pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);
					PangoRectangle inkt_rect;
					pango_layout_get_pixel_extents(pango_layout, NULL, &inkt_rect);
					line_width = inkt_rect.width;
					sub_line_count = pango_layout_get_line_count(pango_layout);
				}
				cha_utf8_text_cleanup(&utf8_text);
			} else {

				int length = 0;
				gboolean do_fast_layout = FALSE;


				// TODO is_mono_space is not enough to allow fast-track.
				if (allow_fast_track) {
					const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(a_page, line_idx, allow_fast_track);
					length = utf8_text.text_len;
					do_fast_layout = utf8_text.only_ascii && (!utf8_text.has_tabs);
					cha_utf8_text_cleanup(&utf8_text);
				}

				cat_log_debug("do_fast_layout=%d, is_mono_space=%d, line_idx=%d", do_fast_layout, is_mono_space, line_idx);


				if (do_fast_layout) {
					line_width = char_width * length;
					sub_line_count = 1;
				} else {
					ChaLineWo *a_line = cha_page_wo_line_at(a_page, line_idx);
					ChaLineLayout *line_layout = cha_document_view_get_line_layout_ref(document_view, a_line);
					cha_line_layout_lock(line_layout, FALSE);

					cat_log_debug("line=%o", a_line);

					if (cha_line_layout_set_text(line_layout, cha_line_wo_get_text(a_line), cha_line_wo_get_line_end(a_line), view_ctx.wrap_lines, view_ctx.tab_size, view_ctx.text_view_width, view_ctx.font_version)) {
						cha_line_layout_update(line_layout, pango_context, priv->color_map);
					}
					cha_line_layout_get_width_and_sub_line_count(line_layout, &line_width, &sub_line_count);
					cat_unref_ptr(a_line);
					cha_line_layout_unlock(line_layout);
					cat_unref_ptr(line_layout);
				}

			}
			page_height += sub_line_count*view_ctx.line_height;
			if (line_width>page_width) {
				page_width = line_width;
			}

			if (test_for_concurrent_update) {
				is_updating = cha_page_layout_version_is_updating(page_layout, page_version, text_layout_width, view_ctx.font_version/*, settings_version*/);
			}
		}
	}


	cat_log_detail("page=%p, page_height=%d, is_updating=%d, req[pv/lw/fo]=%d/%d/%d", a_page, page_height, is_updating, page_layout_context->page_version, page_layout_context->text_layout_width, page_layout_context->font_version);

	page_layout_context->requisition.height = page_height;
	page_layout_context->requisition.width = page_width;
	page_layout_context->text_layout_width = text_layout_width;
	page_layout_context->page_version = cat_wo_get_version((CatWo *) a_page);

//	if (is_updating) {
//		cha_page_layout_set_dimensions(e_page_view, page_width, page_height);
//		cha_page_layout_set_versions(e_page_view, page_version, text_view_width);
//	}

	cat_unref_ptr(pango_context);

	cha_page_wo_release_lines(a_page);

	cat_unref_ptr(pango_layout);
	return is_updating;
}


gboolean cha_document_view_page_set_layout(ChaDocumentView *document_view, ChaPageWo *a_page, ChaPageLayout *a_new_page_layout, ChaPageLayoutContext *page_layout_context, gboolean refresh) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	cat_log_detail("page_set_layout: new height:page=%p, new_height=%d, refresh=%d", a_page, page_layout_context->requisition.height, refresh);

	if (!cha_page_layout_apply_update_result(a_new_page_layout, page_layout_context)) {
		cat_log_detail("page_set_layout: canceled page=%p", a_page);
		return FALSE;
	}

	if (refresh) {
		ChaRevisionWo *rev = cha_document_get_current_revision_ref(priv->document);
		int page_cnt = cha_revision_wo_page_count(rev);
		int page_idx;
		long long int layout_height = 0;
		int layout_width = 0;
		for(page_idx=0; page_idx<page_cnt; page_idx++) {
			ChaPageWo *page = cha_revision_wo_page_at(rev, page_idx);
			ChaPageLayout *page_view_ref = (ChaPageLayout *) cha_page_wo_get_slot_content_ref(page, priv->last_slot_key_idx, (GObject *) priv->a_slot_key);
			cat_log_debug("page_idx=%d, page_view_ref=%o", page_idx, page_view_ref);
			if (page_view_ref) {
				layout_height += cha_page_layout_get_height(page_view_ref);
				int page_width = cha_page_layout_get_width(page_view_ref);
				layout_width = MAX(layout_width, page_width);
				cat_log_trace("page[%d].height=%d , anpl=%p, pal=%p", page_idx, cha_page_layout_get_height(page_view_ref), a_new_page_layout, page_view_ref);
				cat_unref_ptr(page_view_ref);
			}
		}

		priv->layout_height = layout_height;
		if (priv->vadjustment) {
			gdouble new_upper = (gdouble) layout_height;
			gdouble old_value = gtk_adjustment_get_value(priv->vadjustment);
			int view_height = gtk_adjustment_get_page_size(priv->vadjustment);
			new_upper = MAX(layout_height, view_height);

			cat_log_debug("new-upper=%d, priv->vadjustment=%p, view_height=%d, layout_height=%d", (int) new_upper, priv->vadjustment, view_height, layout_height);
			gtk_adjustment_set_upper(priv->vadjustment, new_upper);


			gdouble new_value = CLAMP(old_value, 0, new_upper - view_height);
			if (new_value != old_value) {
				gtk_adjustment_set_value (priv->vadjustment, new_value);
			}
		}






		layout_width = layout_width & (-(1<<6));
		layout_width+=128;

		priv->layout_width = layout_width;
		if (priv->hadjustment) {
			gdouble new_upper = (gdouble) layout_width;
			gdouble old_value = gtk_adjustment_get_value(priv->hadjustment);
			int view_width = gtk_adjustment_get_page_size(priv->hadjustment);
			new_upper = MAX(layout_width, view_width);

			cat_log_debug("new-upper=%d, priv->hadjustment=%p, view_width=%d, layout_width=%d", (int) new_upper, priv->hadjustment, view_width, layout_width);
			gtk_adjustment_set_upper(priv->hadjustment, new_upper);


			gdouble new_value = CLAMP(old_value, 0, new_upper - view_width);
			if (new_value != old_value) {
				gtk_adjustment_set_value (priv->hadjustment, new_value);
			}
		}


		cat_unref_ptr(rev);
	}
	return TRUE;
}



long long int cha_document_view_set_view_y(ChaDocumentView *document_view, long long int y_pos) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	long long int result = priv->view_y;
	priv->view_y = y_pos;
	return result;
}

long long int cha_document_view_set_view_x(ChaDocumentView *document_view, long long int x_pos) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	long long int result = priv->view_x;
	priv->view_x = x_pos;
	return result;
}

long long int cha_document_view_get_view_y(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->view_y;
}

long long int cha_document_view_get_view_x(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->view_x;
}

int cha_document_view_get_view_height(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->view_height;
}

long long int cha_document_view_get_layout_height(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->layout_height;
}


static void l_run_relayout_pages(ChaDocumentView *document_view, ChaRevisionWo *a_rev, const char *src) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	ChaDocumentManager *document_manager = cha_document_get_document_manager(priv->document);

	cat_log_detail(">>>>>>>>>>>>>> l_run_relayout_pages:%s", src);

//	int view_width = priv->view_width;
//	priv->ctx.text_view_width = view_width - priv->ctx.line_nr_view_width;
	int text_layout_width = priv->ctx.text_layout_width;

	gboolean invalidate_lines_at_end = FALSE;

	priv->last_slot_key_idx = cha_revision_wo_get_slot_index(a_rev, (GObject *) priv->a_slot_key, priv->last_slot_key_idx);
	ChaCursorWo *cursor_wo = cha_revision_wo_get_cursor(a_rev);
	ChaLineLocationWo *ll = cha_cursor_wo_get_line_location(cursor_wo);
	int cursor_page_index = cha_line_location_wo_get_page_index(ll);
	int page_cnt = cha_revision_wo_page_count(a_rev);
	int page_idx;
	for(page_idx=0; page_idx<page_cnt; page_idx++) {
		ChaPageWo *page = cha_revision_wo_page_at(a_rev, page_idx);
		ChaPageLayout *page_layout = (ChaPageLayout *) cha_page_wo_get_slot_content_ref(page, priv->last_slot_key_idx, (GObject *) priv->a_slot_key);
		if (page_layout==NULL) {
			page_layout = cha_page_layout_new();
			cat_log_trace("setting page layout on page:%p, slot_key=%o, priv->last_slot_key_idx=%d", page, priv->a_slot_key, priv->last_slot_key_idx);
			cha_page_wo_set_slot_content(page, priv->last_slot_key_idx, (GObject *) priv->a_slot_key, (GObject *) page_layout);
		}

		cat_log_detail("page[%d] page_layout=%p", page_idx, page_layout);
		if (cha_page_layout_versions_start_update(page_layout, cat_wo_get_version((CatWo *) page), text_layout_width, priv->ctx.font_version)) {
			cat_log_detail("page[%d]:%p, page_layout=%o, page_idx=%d, cursor_page_index=%d", page_idx, page, page_layout, page_idx, cursor_page_index);
			if (page_idx==cursor_page_index) {
				ChaPageLayoutContext page_layout_context;
				page_layout_context.page_version = cat_wo_get_version((CatWo *) page);
				page_layout_context.text_layout_width = text_layout_width;
				page_layout_context.font_version = priv->ctx.font_version;
				gboolean did_layout = cha_document_view_layout_page(document_view, page, &(page_layout_context), page_layout, FALSE);
				cat_log_debug("did_layout:%d", did_layout);
				if (did_layout) {
					cha_document_view_page_set_layout(document_view, page, page_layout, &(page_layout_context), TRUE);
					invalidate_lines_at_end = TRUE;
				}
			} else {
				ChaPageLayoutRequest *req = cha_page_layout_request_new(document_view, page, page_layout, text_layout_width, priv->ctx.font_version, page_idx);
				cha_document_manager_post(document_manager, (WorRequest *) req);
				cat_unref_ptr(req);
			}
		}
		cat_unref_ptr(page_layout);
	}
	cat_log_detail("<<<<<<<<<<<<<< l_run_relayout_pages");

	if (invalidate_lines_at_end) {
		cha_document_view_invalidate_lines(document_view);
	}
}


static void l_update_text_view_width(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);

	priv->ctx.text_view_width = priv->view_width - priv->ctx.line_nr_view_width - priv->ctx.line_height;
	int new_layout_width = priv->ctx.text_view_width;
	if (!priv->ctx.wrap_lines) {
		new_layout_width = -1;
	}
	if (priv->ctx.text_layout_width != new_layout_width) {
		priv->ctx.text_layout_width = new_layout_width;
		ChaRevisionWo *a_rev = cha_document_get_current_revision_ref(priv->document);
		l_run_relayout_pages(document_view, a_rev, "update-text_view");
		cat_unref_ptr(a_rev);
	}
	cat_log_trace("text_view_width=%d, text_layout_width=%d", priv->ctx.text_view_width, priv->ctx.text_layout_width);
}

void cha_document_view_set_view_size(ChaDocumentView *document_view, int view_width, int view_height) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	priv->view_height = view_height;
	priv->view_width = view_width;
	cha_surface_pool_set_image_width(priv->surface_pool, view_width);

	l_update_text_view_width(document_view);
}

ChaDocument *cha_document_view_get_document(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->document;
}

PangoContext *cha_document_view_get_pango_context(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	PangoContext *result = gtk_widget_create_pango_context(priv->widget);
	pango_context_set_font_description(result, pango_context_get_font_description(priv->pango_context));
	return result;
//	return priv->pango_context;
}

int cha_document_view_get_marked_layout_x_cursor(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->marked_layout_x_cursor;
}

void cha_document_view_mark_layout_x_cursor(ChaDocumentView *document_view, ChaRevisionWo *a_revision, ChaCursorWo *cursor) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	gboolean is_big_mode = cha_document_is_big_file_mode(priv->document);
	ChaLineLocationWo *a_line_location = cha_cursor_wo_get_line_location(cursor);
	int page_index = cha_line_location_wo_get_page_index(a_line_location);
	ChaPageWo *a_page = cha_revision_wo_page_at(a_revision, page_index);
	cha_page_wo_hold_lines(a_page);
	int page_line_idx = cha_line_location_wo_get_page_line_index(a_line_location);
	PangoLayout *pango_layout = NULL;
	ChaLineWo *a_line = NULL;
	ChaLineLayout *line_layout_ref = NULL;
	if (is_big_mode) {
		pango_layout = priv->ctx.wrap_lines ? priv->pango_layout_wrap : priv->pango_layout_no_wrap;
		ChaUtf8Text utf8_text = cha_page_wo_utf8_at(a_page, page_line_idx, FALSE);
		pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);
		cha_utf8_text_cleanup(&utf8_text);
	} else {
		a_line = cha_page_wo_line_at(a_page, page_line_idx);
		line_layout_ref = cha_document_view_get_line_layout_ref(document_view, a_line);
		cha_line_layout_lock(line_layout_ref, TRUE);
		cha_line_layout_set_text(line_layout_ref, cha_line_wo_get_text(a_line), cha_line_wo_get_line_end(a_line), priv->ctx.wrap_lines, priv->ctx.tab_size, priv->ctx.text_view_width, priv->ctx.font_version);
		cha_line_layout_update(line_layout_ref, priv->pango_context, priv->color_map);
		pango_layout = cha_line_layout_get_pango_layout(line_layout_ref);
	}
	PangoRectangle pcursor;
	int x_cursor_bytes = cha_cursor_wo_get_x_cursor_bytes(cursor);
	pango_layout_get_cursor_pos(pango_layout, x_cursor_bytes, &(pcursor), NULL);
	priv->marked_layout_x_cursor = pcursor.x/PANGO_SCALE;
	if (line_layout_ref) {
		cha_line_layout_unlock(line_layout_ref);
		cat_unref_ptr(line_layout_ref);
		cat_unref_ptr(a_line);
	}
	cha_page_wo_release_lines(a_page);

}

ChaLineLocationWo *cha_document_view_cursor_at_marker(ChaDocumentView *document_view, int x, int y, long long *view_line_y) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	long long int cursor_y = (long long int) y + priv->view_y;
	int marker_width = priv->view_width-priv->ctx.text_view_width;
	if (x<0 || x>marker_width) {
		return NULL;
	}

	ChaLineLocationWo *result = NULL;
	long long int page_y = 0;

	ChaRevisionWo *a_revision = cha_document_get_current_revision_ref(priv->document);
	gboolean is_big_mode = cha_document_is_big_file_mode(priv->document);
	int page_cnt = cha_revision_wo_page_count(a_revision);
	int page_idx;
	for(page_idx=0; page_idx<page_cnt; page_idx++) {
		ChaPageWo *page = cha_revision_wo_page_at(a_revision, page_idx);
		ChaPageLayout *page_view = (ChaPageLayout *) cha_page_wo_get_slot_content_ref(page, priv->last_slot_key_idx, NULL);
		cat_log_debug("page_view=%o", page_view);
		if (page_view==NULL) {
			cat_log_warn("no page view at idx:%d", page_idx);
			break;
		}

		cat_log_debug("page=%o", page);

		int height = cha_page_layout_get_height(page_view);

		cat_log_debug("page-view=%o, page_y=%d, cursor_y=%d", page_view, (int) page_y, (int) cursor_y);
		cat_unref_ptr(page_view);
		if (page_y+height<cursor_y) {
			page_y += height;
			cat_log_debug("page above");

			continue;
		}
		if (page_y>cursor_y) {
			cat_log_debug("page below");
			break;
		}

		cha_page_wo_hold_lines(page);


		long long int page_line_y = page_y;
		page_y += height;	// TODO: this is probably dangerous
		int line_cnt = cha_page_wo_line_count(page);
		cat_log_debug("line_cnt=%d", line_cnt);
		int line_idx;
		ChaLineLayout *a_line_layout = NULL;
		ChaLineWo *line = NULL;
//		char *text = NULL;
//		int text_length = 0;
		for(line_idx=0; line_idx<line_cnt; line_idx++) {
			gboolean valid_pango = FALSE;
			int line_width = 0;
			int sub_line_count = 0;
			if (is_big_mode) {

				if (!priv->ctx.wrap_lines && priv->ctx.is_mono_space) {
					sub_line_count = 1;
				} else {
					PangoLayout *pango_layout = priv->ctx.wrap_lines ? priv->pango_layout_wrap : priv->pango_layout_no_wrap;
					const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(page, line_idx, FALSE);
					pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);
					cha_utf8_text_cleanup(&utf8_text);
					PangoRectangle inkt_rect;
					pango_layout_get_pixel_extents(pango_layout, NULL, &inkt_rect);
					sub_line_count = pango_layout_get_line_count(pango_layout);
					line_width = inkt_rect.width;
					valid_pango = TRUE;
				}
			} else {
				line = cha_page_wo_line_at(page, line_idx);
				cat_log_trace("line=%o", line);
				a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
				cha_line_layout_lock(a_line_layout, TRUE);
				gboolean new_text = cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), priv->ctx.wrap_lines, priv->ctx.tab_size, priv->ctx.text_view_width, priv->ctx.font_version);
				if (new_text) {
					cha_line_layout_update(a_line_layout, priv->pango_context, priv->color_map);
				}
				cha_line_layout_get_width_and_sub_line_count(a_line_layout, &line_width, &sub_line_count);
			}

			cat_log_debug("line_idx=%d, sub_line_count=%d", line_idx, sub_line_count);


			int line_height = priv->ctx.line_height*sub_line_count;

			if (page_line_y+line_height<cursor_y) {
				page_line_y += line_height;
				if (a_line_layout) {
					cha_line_layout_unlock(a_line_layout);
					cat_unref_ptr(a_line_layout);
					cat_unref_ptr(line);
				}
				continue;
			}
			if (page_line_y>cursor_y) {
				if (a_line_layout) {
					cha_line_layout_unlock(a_line_layout);
					cat_unref_ptr(a_line_layout);
					cat_unref_ptr(line);
				}
				break;
			}

			int y = (int) (cursor_y-page_line_y);
			cat_log_debug("y=%d", y);

			PangoLayout *pango_layout = NULL;
			if (is_big_mode) {
				pango_layout = priv->ctx.wrap_lines ? priv->pango_layout_wrap : priv->pango_layout_no_wrap;
				if (!valid_pango) {
					const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(page, line_idx, FALSE);
					pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);
					cha_utf8_text_cleanup(&utf8_text);
				}
			} else {
				cha_line_layout_update(a_line_layout, priv->pango_context, priv->color_map);
				pango_layout = cha_line_layout_get_pango_layout(a_line_layout);
			}
			sub_line_count = pango_layout_get_line_count(pango_layout);
			int sub_line_idx;
			int baseline = pango_layout_get_baseline(pango_layout)/PANGO_SCALE;
			for(sub_line_idx=0; sub_line_idx<sub_line_count; sub_line_idx++) {
				PangoLayoutLine *pango_line = pango_layout_get_line_readonly(pango_layout, sub_line_idx);
				PangoRectangle inkt_rect;
				pango_layout_line_get_pixel_extents(pango_line, NULL, &inkt_rect);
				inkt_rect.y += baseline;

				cat_log_debug("inkt_rect=%d,%d, baseline=%d",inkt_rect.y, inkt_rect.height, baseline);
				if (y>=inkt_rect.y && y<=inkt_rect.y+priv->ctx.line_height) {
					result = cha_line_location_wo_new();
					cha_line_location_wo_set_page_index(result, page_idx);
					cha_line_location_wo_set_page_line_index(result, line_idx);
					cat_log_debug("result=%o", result);

					if (view_line_y!=NULL) {
						*view_line_y = page_line_y;
					}

					/* break loops */
					sub_line_idx = sub_line_count;
					line_idx = line_cnt;
					page_idx = page_cnt;
				}
				y -= priv->ctx.line_height;
			}

			if (!is_big_mode) {
				cha_line_layout_unlock(a_line_layout);
			}

			page_line_y += line_height;
			cat_unref_ptr(a_line_layout);
			cat_unref_ptr(line);
		}

		cha_page_wo_release_lines(page);
	}
	cat_unref_ptr(a_revision);
	return result;
}


ChaCursorWo *cha_document_view_cursor_at_xy(ChaDocumentView *document_view, int x, int y) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	ChaCursorWo *e_result = NULL;

	long long int cursor_y = (long long int) y + priv->view_y;
	int cursor_x =(int) (x + priv->view_x - (priv->view_width-priv->ctx.text_view_width));

	long long int page_y = 0;

	ChaRevisionWo *a_revision = cha_document_get_current_revision_ref(priv->document);
	gboolean is_big_mode = cha_document_is_big_file_mode(priv->document);
	int page_cnt = cha_revision_wo_page_count(a_revision);
	int page_idx;
	for(page_idx=0; page_idx<page_cnt; page_idx++) {
		ChaPageWo *page = cha_revision_wo_page_at(a_revision, page_idx);
		ChaPageLayout *page_view = (ChaPageLayout *) cha_page_wo_get_slot_content_ref(page, priv->last_slot_key_idx, NULL);
		cat_log_debug("page_view=%o", page_view);
		if (page_view==NULL) {
			cat_log_warn("no page view at idx:%d", page_idx);
			break;
		}

		cat_log_debug("page=%o", page);

		int height = cha_page_layout_get_height(page_view);

		cat_log_debug("page-view=%o, page_y=%d, cursor_y=%d", page_view, (int) page_y, (int) cursor_y);
		cat_unref_ptr(page_view);
		if (page_y+height<cursor_y) {
			page_y += height;
			cat_log_debug("page above");

			if (page_idx==page_cnt-1) {
				e_result = cha_cursor_wo_new();
				ChaLineLocationWo *e_line_location = cha_cursor_wo_get_editable_line_location(e_result);
				cha_line_location_wo_set_page_index(e_line_location, page_cnt-1);
				int page_line_index = cha_page_wo_line_count(page)-1;
				cha_line_location_wo_set_page_line_index(e_line_location, page_line_index);
				const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(page, page_line_index, FALSE);
				cha_cursor_wo_set_x_cursor_bytes(e_result, utf8_text.text_len);
				cha_utf8_text_cleanup(&utf8_text);
			}
			continue;
		}
		if (page_y>cursor_y) {
			cat_log_debug("page below");
			e_result = cha_cursor_wo_new();
			break;
		}

		cha_page_wo_hold_lines(page);


		long long int page_line_y = page_y;
		page_y += height;	// TODO: this is probably dangerous
		int line_cnt = cha_page_wo_line_count(page);
		cat_log_debug("line_cnt=%d", line_cnt);
		int line_idx;
		ChaLineLayout *a_line_layout = NULL;
		ChaLineWo *line = NULL;
		for(line_idx=0; line_idx<line_cnt; line_idx++) {
			int line_width = 0;
			int sub_line_count = 0;
			gboolean valid_pango = FALSE;
			if (is_big_mode) {

				if (!priv->ctx.wrap_lines && priv->ctx.is_mono_space) {
					sub_line_count = 1;
				} else {
					PangoLayout *pango_layout = priv->ctx.wrap_lines ? priv->pango_layout_wrap : priv->pango_layout_no_wrap;
					const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(page, line_idx, FALSE);
					pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);
					cha_utf8_text_cleanup(&utf8_text);
					PangoRectangle inkt_rect;
					pango_layout_get_pixel_extents(pango_layout, NULL, &inkt_rect);
					line_width = inkt_rect.width;
					sub_line_count = pango_layout_get_line_count(pango_layout);
					valid_pango = TRUE;
				}
			} else {
				line = cha_page_wo_line_at(page, line_idx);
				cat_log_trace("line=%o", line);
				a_line_layout = cha_document_view_get_line_layout_ref(document_view, line);
				cha_line_layout_lock(a_line_layout, TRUE);
				gboolean new_text = cha_line_layout_set_text(a_line_layout, cha_line_wo_get_text(line), cha_line_wo_get_line_end(line), priv->ctx.wrap_lines, priv->ctx.tab_size, priv->ctx.text_view_width, priv->ctx.font_version);
				if (new_text) {
					cha_line_layout_update(a_line_layout, priv->pango_context, priv->color_map);
				}
				cha_line_layout_get_width_and_sub_line_count(a_line_layout, &line_width, &sub_line_count);
			}

			cat_log_debug("line_idx=%d, sub_line_count=%d", line_idx, sub_line_count);

			int line_height = sub_line_count*priv->ctx.line_height;

			if (page_line_y+line_height<cursor_y) {
				page_line_y += line_height;
				if (a_line_layout) {
					cha_line_layout_unlock(a_line_layout);
					cat_unref_ptr(a_line_layout);
					cat_unref_ptr(line);
				}
				continue;
			}
			if (page_line_y>cursor_y) {
				if (a_line_layout) {
					cha_line_layout_unlock(a_line_layout);
					cat_unref_ptr(a_line_layout);
					cat_unref_ptr(line);
				}
				break;
			}

			int y = (int) (cursor_y-page_line_y);
			cat_log_debug("y=%d", y);

			PangoLayout *pango_layout = NULL;
			if (is_big_mode) {
				pango_layout = priv->ctx.wrap_lines ? priv->pango_layout_wrap : priv->pango_layout_no_wrap;
				if (!valid_pango) {
					const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(page, line_idx, FALSE);
					pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);
					cha_utf8_text_cleanup(&utf8_text);
				}
			} else {
				cha_line_layout_update(a_line_layout, priv->pango_context, priv->color_map);
				pango_layout = cha_line_layout_get_pango_layout(a_line_layout);
			}
			sub_line_count = pango_layout_get_line_count(pango_layout);
			int sub_line_idx;
			int baseline = pango_layout_get_baseline(pango_layout)/PANGO_SCALE;
			for(sub_line_idx=0; sub_line_idx<sub_line_count; sub_line_idx++) {
				PangoLayoutLine *pango_line = pango_layout_get_line_readonly(pango_layout, sub_line_idx);
				PangoRectangle inkt_rect;
				pango_layout_line_get_pixel_extents(pango_line, NULL, &inkt_rect);
				inkt_rect.y += baseline;

				cat_log_debug("inkt_rect=%d,%d, baseline=%d",inkt_rect.y, inkt_rect.height, baseline);
				if (y>=inkt_rect.y && y<=inkt_rect.y+priv->ctx.line_height) {
					int index=0;
					int x_sub = 0;
					int trailing = 0;
					if (!pango_layout_line_x_to_index(pango_line, cursor_x*PANGO_SCALE, &index, &trailing)) {
						index = pango_line->start_index;
						if (cursor_x>0) {
							index = pango_line->start_index+pango_line->length;
							if (sub_line_idx+1!=sub_line_count) {
								index--;
							}

							if (!priv->ctx.limit_cursor) {
								int xpp = 0;
								pango_layout_line_index_to_x(pango_line, index, TRUE, &xpp);
								xpp = cursor_x*PANGO_SCALE - xpp;
								x_sub = (int) round(xpp/priv->ctx.ps_space_width);
							}
						}

					} else {
						index += trailing;
					}


					e_result = cha_cursor_wo_new();
					cha_cursor_wo_set_x_cursor_bytes(e_result, index);
					cha_cursor_wo_set_x_sub(e_result, x_sub);
					ChaLineLocationWo *e_line_location = cha_line_location_wo_new();
					cha_line_location_wo_set_page_index(e_line_location, page_idx);
					cha_line_location_wo_set_page_line_index(e_line_location, line_idx);
					cha_cursor_wo_set_line_location(e_result, e_line_location);
					cat_unref_ptr(e_line_location);
					cat_log_debug("cursor=%o", e_result);

					/* break loops */
					sub_line_idx = sub_line_count;
					line_idx = line_cnt;
					page_idx = page_cnt;
				}
				y-=priv->ctx.line_height;
			}

			if (!is_big_mode) {
				cha_line_layout_unlock(a_line_layout);
			}

			page_line_y += line_height;
			cat_unref_ptr(a_line_layout);
			cat_unref_ptr(line);
		}

		cha_page_wo_release_lines(page);
	}
	cat_unref_ptr(a_revision);
	return e_result;
}

static long long int l_location_to_view_y(ChaDocumentView *document_view, ChaRevisionWo *revision, ChaCursorWo *cursor, int *o_line_height, int *view_x_cursor) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	long long int result = 0l;
	ChaLineLocationWo *location = cha_cursor_wo_get_line_location(cursor);
	int page_index = cha_line_location_wo_get_page_index(location);
	ChaPageWo *page = cha_revision_wo_page_at(revision, page_index);
	cha_page_wo_hold_lines(page);

	int page_line_index = cha_line_location_wo_get_page_line_index(location);

	PangoLayout *pango_layout = NULL;
	gboolean is_big_mode = cha_document_is_big_file_mode(priv->document);
	ChaLineWo *a_line = NULL;
	ChaLineLayout *line_layout = NULL;

	if (is_big_mode) {
		pango_layout = priv->ctx.wrap_lines ? priv->pango_layout_wrap : priv->pango_layout_no_wrap;
		const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(page, page_line_index, FALSE);
		pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);
		cha_utf8_text_cleanup(&utf8_text);
	} else {
		a_line = cha_page_wo_line_at(page, page_line_index);
		line_layout = cha_document_view_get_line_layout_ref(document_view, a_line);
		cha_line_layout_lock(line_layout, TRUE);

		gboolean new_text = cha_line_layout_set_text(line_layout, cha_line_wo_get_text(a_line), cha_line_wo_get_line_end(a_line), priv->ctx.wrap_lines, priv->ctx.tab_size, priv->ctx.text_view_width, priv->ctx.font_version);
		if (new_text) {
			cha_line_layout_update(line_layout, priv->pango_context, priv->color_map);
		}
		pango_layout = cha_line_layout_update_pango_layout(line_layout, priv->pango_context);
	}

	int cursor_sub_line_idx = 0;
	int cursor_xpos = 0;
	cat_log_debug("pango_layout=%p", pango_layout);
	pango_layout_index_to_line_x(pango_layout, cha_cursor_wo_get_x_cursor_bytes(cursor), FALSE, &cursor_sub_line_idx, &cursor_xpos);
	PangoLayoutLine *pango_line = pango_layout_get_line_readonly(pango_layout, cursor_sub_line_idx);
	PangoRectangle sub_line_rect;
	pango_layout_line_get_pixel_extents(pango_line, NULL, &sub_line_rect);
	int baseline = pango_layout_get_baseline(pango_layout)/PANGO_SCALE;
	sub_line_rect.y += baseline;
	result += sub_line_rect.y;
	*o_line_height = priv->ctx.line_height;
	*view_x_cursor = cursor_xpos/PANGO_SCALE;


	if (line_layout) {
		cha_line_layout_unlock(line_layout);
		cat_unref_ptr(line_layout);
		cat_unref_ptr(a_line);
	}


	if (!priv->ctx.wrap_lines && priv->ctx.is_mono_space) {
		/* fast lane: no wrap lines and mono space font */
		result += priv->ctx.line_height*page_line_index;
	} else if (is_big_mode) {
		pango_layout = priv->ctx.wrap_lines ? priv->pango_layout_wrap : priv->pango_layout_no_wrap;
		for(page_line_index--; page_line_index>=0; page_line_index--) {
			const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(page, page_line_index, FALSE);
			pango_layout_set_text(pango_layout, utf8_text.text, utf8_text.text_len);
			cha_utf8_text_cleanup(&utf8_text);
			result += pango_layout_get_line_count(pango_layout)*priv->ctx.line_height;
		}
	} else {
		/* last alternative */
		for(page_line_index--; page_line_index>=0; page_line_index--) {
			ChaLineWo *a_line = cha_page_wo_line_at(page, page_line_index);
			ChaLineLayout *pline_layout = cha_document_view_get_line_layout_ref(document_view, a_line);
			cha_line_layout_lock(pline_layout, TRUE);

			gboolean new_text = cha_line_layout_set_text(pline_layout, cha_line_wo_get_text(a_line), cha_line_wo_get_line_end(a_line), priv->ctx.wrap_lines, priv->ctx.tab_size, priv->ctx.text_view_width, priv->ctx.font_version);
			if (new_text) {
				cha_line_layout_update(pline_layout, priv->pango_context, priv->color_map);
			}
			int sub_line_count = 0;
			cha_line_layout_get_width_and_sub_line_count(pline_layout, NULL, &sub_line_count);
			result += sub_line_count*priv->ctx.line_height;
			cha_line_layout_unlock(pline_layout);
			cat_unref_ptr(pline_layout);
			cat_unref_ptr(a_line);
		}
	}

	cha_page_wo_release_lines(page);

	for(page_index--; page_index>=0; page_index--) {
		ChaPageWo *page = cha_revision_wo_page_at(revision, page_index);
		ChaPageLayout *page_view = (ChaPageLayout *) cha_page_wo_get_slot_content_ref(page, priv->last_slot_key_idx, NULL);
		int page_height = cha_page_layout_get_height(page_view);
		result += page_height;
		cat_unref_ptr(page_view);
	}
	return result;
}

gboolean cha_document_view_get_cursor_screen_location(ChaDocumentView *document_view, int *cursor_x, int *cursor_y, int *line_height) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);

	ChaCursorWo *cursor = cha_revision_wo_get_cursor(priv->a_revision_wo);
	gboolean result = TRUE;

	int x_layout = 0;
	long long int y_layout = l_location_to_view_y(document_view, priv->a_revision_wo, cursor, line_height, &x_layout);

	long long int y_view = y_layout-priv->view_y;
	if (y_view<0) {
		y_view = 0;
		result = FALSE;
	} else if (y_view>priv->view_height) {
		y_view = priv->view_height;
		result = FALSE;
	}



	int x_view = x_layout+priv->ctx.line_nr_view_width+priv->ctx.line_height-priv->view_x;
	if (x_view<0) {
		x_view = 0;
		result = FALSE;
	} else if (x_view>priv->ctx.text_view_width) {
		x_view = priv->ctx.text_view_width;
		result = FALSE;
	}

	*cursor_x = x_view;
	*cursor_y = y_view;
	return result;
}


void cha_document_view_move_view_to_focus(ChaDocumentView *document_view, gboolean do_center) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);

	ChaRevisionWo *revision_ref = cha_document_get_current_revision_ref(priv->document);
	ChaCursorWo *cursor = cha_revision_wo_get_cursor(revision_ref);
	int o_line_height = 0;
	int x_cursor_view = 0;

	long long int cursor_y_top = l_location_to_view_y(document_view, revision_ref, cursor, &o_line_height, &x_cursor_view);

	gint viewX = priv->view_x;
	long long int viewY = priv->view_y;

	int view_text_width = priv->ctx.text_view_width;
	int view_height = priv->view_height;


	gint phy_x_cursor = x_cursor_view /* + view_cursor->sub_column*document_view->view_space_width */;
	cat_log_debug("phy_x_cursor=%d, view_text_width=%d, viewX=%d", phy_x_cursor, view_text_width, viewX);
	if (phy_x_cursor<viewX) {
		viewX = phy_x_cursor-(int) (view_text_width*0.25f);
	}
	if (phy_x_cursor+3>viewX+view_text_width) {
		viewX = phy_x_cursor-(int) (view_text_width*0.75f);
	}


//	if (phy_x_cursor+view_text_width >= document_view->document_width) {
//		viewX = document_view->document_width - view_text_width;
//	}

	if (viewX<0) {
		viewX = 0;
	}

	gint font_height = o_line_height;

	long long int phy_y_cursor = cursor_y_top;
	long long int page_end = viewY+view_height;

	if (do_center && (phy_y_cursor<viewY ||  phy_y_cursor+font_height>page_end)) {
		viewY = phy_y_cursor - view_height/2;
	}

	cat_log_debug("phy_y_cursor=%ld, font_height=%ld, viewY=%ld", phy_y_cursor, font_height, viewY);
	if (phy_y_cursor<viewY) {
		viewY = phy_y_cursor;
	}

	if (phy_y_cursor+font_height>page_end) {
		viewY = phy_y_cursor-view_height+font_height;
	}

	if (viewY<0) {
		viewY=0;
	}

	cat_log_debug("viewXY[%d,%d]", viewX, viewY);
	gtk_adjustment_set_value(priv->vadjustment, (gdouble) viewY);
	gtk_adjustment_set_value(priv->hadjustment, (gdouble) viewX);
	cat_unref_ptr(revision_ref);
}

void cha_document_view_move_view_vertical(ChaDocumentView *document_view, int lines) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	int view_lines = priv->ctx.line_height*lines;
	long long int nwy = priv->view_y+view_lines;
	if (nwy<0) {
		nwy = 0;
	}
	gtk_adjustment_set_value(priv->vadjustment, (gdouble) nwy);
}


ChaSelection *cha_document_view_get_selection(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	return priv->selection;
}



ChaSelection *cha_document_view_get_or_create_selection(ChaDocumentView *document_view, ChaRevisionWo *a_revision, ChaCursorWo *cursor, gboolean block) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	if (priv->selection == NULL) {
		if (block) {
			priv->selection = (ChaSelection *) cha_block_selection_new(priv->selection_sequence, cursor);
		} else {
			priv->selection = (ChaSelection *) cha_plain_selection_new(priv->selection_sequence, cursor);
		}
	}
	return priv->selection;
}


void cha_document_view_clear_selection(ChaDocumentView *document_view, ChaRevisionWo *a_revision) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	if (priv->selection == NULL) {
		return;
	}
	cat_unref_ptr(priv->selection);
}


gboolean cha_document_view_remove_selection(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	gboolean result = FALSE;
	if (priv->selection) {
		gboolean is_editable = cha_document_is_editable(priv->document);
		ChaRevisionWo *e_revision = cha_document_get_editable_revision(priv->document);
		ChaCursorWo *end_cursor = cha_selection_delete(priv->selection, document_view, e_revision);
		cat_log_debug("end_cursor=%o", end_cursor);
		cha_revision_wo_set_cursor(e_revision, end_cursor);
		result = end_cursor!=NULL;
		cat_unref_ptr(end_cursor);
		if (!is_editable) {
			cha_document_anchor_document(priv->document);
		}
		cat_unref_ptr(priv->selection);
	}
	return result;
}

//void cha_document_view_set_selection_end(ChaDocumentView *document_view, ChaRevisionWo *revision, ChaCursorWo *cursor) {
//	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
//	if (priv->selection == NULL) {
//		return;
//	}
//
//	cha_selection_set_end(priv->selection, cursor);
//}

gboolean cha_document_view_set_plain_selection(ChaDocumentView *document_view, ChaCursorWo *start, ChaCursorWo *end) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	if (CHA_IS_BLOCK_SELECTION(priv->selection)) {
		cat_unref_ptr(priv->selection);
	} else if (CHA_IS_PLAIN_SELECTION(priv->selection)) {
		ChaCursorWo *cur_st = cha_selection_get_start(priv->selection);
		if (!cha_cursor_wo_equal(cur_st, start)) {
			cat_unref_ptr(priv->selection);
		}
	}

	if (priv->selection==NULL) {
		priv->selection = (ChaSelection *) cha_plain_selection_new(priv->selection_sequence, start);
	}

	cha_selection_set_end(priv->selection, end);
	return TRUE;
}


ChaLineLayout *cha_document_view_get_line_layout_ref(ChaDocumentView *document_view, ChaLineWo *a_line) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	ChaLineLayout *a_line_layout = (ChaLineLayout *) cha_line_wo_get_slot_content_ref(a_line, priv->last_slot_key_idx, NULL);
	cat_log_trace("line_view=%o", a_line_layout);
	if (a_line_layout==NULL) {
		ChaDocumentViewClass *dv_class = CHA_DOCUMENT_VIEW_GET_CLASS(document_view);
		if (dv_class->createLineLayout) {
			a_line_layout = dv_class->createLineLayout(document_view);
		} else {
			a_line_layout = cha_line_layout_new();
		}

		cha_line_wo_set_slot_content(a_line, priv->last_slot_key_idx, (GObject *) priv->a_slot_key, (GObject *) a_line_layout);
	}
//	cha_line_layout_wo_set_text(a_line_layout, cha_line_wo_get_text(a_line), cha_line_wo_get_line_end(a_line));
	return a_line_layout;
}

ChaLineLayout *cha_document_view_get_line_layout_ref_for_cursor(ChaDocumentView *document_view, ChaRevisionWo *a_revision, ChaCursorWo *a_cursor, ChaLineWo **line) {
	ChaLineLocationWo *a_location = cha_cursor_wo_get_line_location(a_cursor);
	int page_index = cha_line_location_wo_get_page_index(a_location);
	ChaPageWo *a_page = cha_revision_wo_page_at(a_revision, page_index);
	ChaLineWo *a_line = cha_page_wo_line_at(a_page, cha_line_location_wo_get_page_line_index(a_location));
	ChaLineLayout *result = cha_document_view_get_line_layout_ref(document_view, a_line);
	if (line) {
		*line = a_line;
	} else {
		cat_unref_ptr(a_line);
	}
	return result;
}


static int l_find_sub_line_cache(CatArrayWo *sub_line_cache, ChaLineLayout *line_layout, int sub_line_index, int exp_index) {
	if (sub_line_cache==NULL) {
		return -1;
	}
	if (exp_index>=0) {
		ChaSubLineCache *test = (ChaSubLineCache *) cat_array_wo_get(sub_line_cache, exp_index);
		if (test != NULL) {
			if (cha_sub_line_cache_defined_by(test, line_layout, sub_line_index)) {
				return exp_index;
			}
		}
	}
	cat_log_debug("cache miss:line_layout=%p, exp_index=%d", line_layout, exp_index);
	int c_idx = cat_array_wo_size(sub_line_cache)-1;
	for(;c_idx>=0; c_idx--) {
		ChaSubLineCache *test = (ChaSubLineCache *) cat_array_wo_get(sub_line_cache, c_idx);
		if (test != NULL) {
			if (cha_sub_line_cache_defined_by(test, line_layout, sub_line_index)) {
				return c_idx;
			}
		}
	}
	return -1;
}

gboolean cha_document_view_check_cache_boundaries(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	cat_log_debug("cache_view_y=%d, cache_height=%d, view_y=%d, view_height=%d", priv->cache_view_y, priv->cache_view_height, priv->view_y, priv->view_height);
	if ((priv->cache_view_y+priv->cache_view_height<priv->view_y+priv->view_height)
		|| (priv->cache_view_y>priv->view_y)) {
		return FALSE;
	}
	return TRUE;
}

static void l_selection_pre_apply(ChaDocumentView *document_view, ChaRevisionWo *revision_ref) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	PangoContext *pango_context = priv->pango_context;
	ChaCursorWo *cursor_start = cha_selection_get_start(priv->selection);
	ChaCursorWo *cursor_end = cha_selection_get_end(priv->selection);
	ChaLineLocationWo *location_start = cha_cursor_wo_get_line_location(cursor_start);
	ChaLineLocationWo *location_end = cha_cursor_wo_get_line_location(cursor_end);
	ChaLineLayout *e_line_layout_start = NULL;
	ChaLineLayout *e_line_layout_end = NULL;
	if (cha_line_location_wo_equal(location_start, location_end)) {
		ChaLineWo *a_line = NULL;
		e_line_layout_start = cha_document_view_get_line_layout_ref_for_cursor(document_view, revision_ref, cursor_start, &a_line);
		e_line_layout_end = e_line_layout_start;
		cha_line_layout_lock(e_line_layout_start, TRUE);
		cha_line_layout_set_text(e_line_layout_start, cha_line_wo_get_text(a_line), cha_line_wo_get_line_end(a_line), priv->ctx.wrap_lines, priv->ctx.tab_size, priv->ctx.text_view_width, priv->ctx.font_version);
		cha_line_layout_update_pango_layout(e_line_layout_start, pango_context);
		cat_unref_ptr(a_line);
	} else {
		ChaLineWo *a_line = NULL;
		e_line_layout_start = cha_document_view_get_line_layout_ref_for_cursor(document_view, revision_ref, cursor_start, &a_line);
		cha_line_layout_lock(e_line_layout_start, TRUE);
		cha_line_layout_set_text(e_line_layout_start, cha_line_wo_get_text(a_line), cha_line_wo_get_line_end(a_line), priv->ctx.wrap_lines, priv->ctx.tab_size, priv->ctx.text_view_width, priv->ctx.font_version);
		cha_line_layout_update_pango_layout(e_line_layout_start, pango_context);
		cat_unref_ptr(a_line);

		e_line_layout_end = cha_document_view_get_line_layout_ref_for_cursor(document_view, revision_ref, cursor_end, &a_line);
		cha_line_layout_lock(e_line_layout_end, TRUE);
		cha_line_layout_set_text(e_line_layout_end, cha_line_wo_get_text(a_line), cha_line_wo_get_line_end(a_line), priv->ctx.wrap_lines, priv->ctx.tab_size, priv->ctx.text_view_width, priv->ctx.font_version);
		cha_line_layout_update_pango_layout(e_line_layout_end, pango_context);
		cat_unref_ptr(a_line);
	}

	CHA_SELECTION_GET_CLASS(priv->selection)->preApply(priv->selection, e_line_layout_start, e_line_layout_end, priv->ctx.ps_space_width);

	if (e_line_layout_start==e_line_layout_end) {
		cha_line_layout_unlock(e_line_layout_start);
		cat_unref_ptr(e_line_layout_start);
	} else {
		cha_line_layout_unlock(e_line_layout_start);
		cha_line_layout_unlock(e_line_layout_end);
		cat_unref_ptr(e_line_layout_start);
		cat_unref_ptr(e_line_layout_end);
	}
}


#define TOP_BOTTOM_CACHED_PIXELS  128

static void l_replace_pages_on_hold(ChaDocumentViewPrivate *priv, CatArrayWo *new_pages) {
	if (priv->pages_on_hold) {
		CatIIterator *iter = cat_array_wo_iterator(priv->pages_on_hold);
		while(cat_iiterator_has_next(iter)) {
			ChaPageWo *page_on_hold = (ChaPageWo *) cat_iiterator_next(iter);
			cha_page_wo_release_lines(page_on_hold);
		}
		cat_unref_ptr(iter);
	}
	if (cat_array_wo_size(new_pages)==0) {
		cat_unref_ptr(new_pages);
	}
	cat_unref_ptr(priv->pages_on_hold);
	priv->pages_on_hold = new_pages;
}




static gboolean l_invalidate_lines(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	if (priv->invalidate_count==0) {
		return FALSE;
	}
	priv->invalidate_count = 0;
	ChaRevisionWo *revision_ref = cat_ref_ptr(priv->a_revision_wo);
	if (revision_ref==NULL) {
		return FALSE;
	}
	CHA_DOCUMENT_VIEW_GET_CLASS(document_view)->invalidateRevision(document_view, revision_ref);

	cat_unref_ptr(revision_ref);
	return FALSE;
}

void cha_document_view_invalidate_lines(ChaDocumentView *document_view) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	priv->invalidate_count++;
	l_invalidate_lines(document_view);
//	g_idle_add_full(G_PRIORITY_HIGH, l_invalidate_lines, document_view, NULL);
}



static void l_invalidate_revision(ChaDocumentView *document_view, ChaRevisionWo *revision_ref) {
	cat_log_debug("invalidate");
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
	PangoContext *pango_context = priv->pango_context;

	priv->selection_mod_count = priv->selection==NULL ? -1 : cha_selection_get_protected(priv->selection)->modification_count;

//	ChaRevisionWo *revision_ref = cha_document_get_current_revision_ref(priv->document);
	priv->last_slot_key_idx = cha_revision_wo_get_slot_index(revision_ref, (GObject *) priv->a_slot_key, priv->last_slot_key_idx);
	if (priv->last_slot_key_idx<0) {
		cat_log_fatal("slot_key not existing: revision_ref=%o", revision_ref);
		// TODO handle
		cat_unref_ptr(revision_ref);
		return;
	}


	cat_log_on_info(
		long long startTs = cat_monotonic_time();
	)

	cat_log_info("invalidate:revision=%p", revision_ref);

	ChaCursorWo *a_cursor = cha_revision_wo_get_cursor(revision_ref);
	ChaLineLocationWo *a_cursor_line_location = cha_cursor_wo_get_line_location(a_cursor);
	int cursor_page_idx = cha_line_location_wo_get_page_index(a_cursor_line_location);
	int cursor_line_idx;

	GdkWindow *window = gtk_widget_get_window(priv->widget);

	const long long int view_top = priv->view_y-TOP_BOTTOM_CACHED_PIXELS;
	const long long int view_bottom = priv->view_y+priv->view_height+TOP_BOTTOM_CACHED_PIXELS;

	cat_log_debug("view: top=%ld, bottom=%ld", view_top, view_bottom);


	const int view_x = (int) priv->view_x;
	const int view_width = priv->view_width;
	if (priv->ctx.show_line_numbers) {
		long long total_line_count = cha_revision_wo_count_lines(revision_ref);
		int sb = priv->ctx.digit_width;
		while(total_line_count>=10) {
			total_line_count = total_line_count/10;
			sb += priv->ctx.digit_width;
		}
		priv->ctx.line_nr_view_width = 1+(sb/PANGO_SCALE);
	} else {
		priv->ctx.line_nr_view_width = 0;
	}

	l_update_text_view_width(document_view);


	long long int line_nr = 0;
	long long int cache_top = -1;
	long long int cache_bottom = -1;
	long long int page_y = 0;
	int page_cnt = cha_revision_wo_page_count(revision_ref);
	int page_idx;
	CatArrayWo *new_pages_on_hold = cat_array_wo_new();
	CatArrayWo *new_cache = NULL;
	if (priv->cached_lines) {
		new_cache = cat_array_wo_new_size(cat_array_wo_size(priv->cached_lines)+5);
	} else {
		new_cache = cat_array_wo_new();
	}

	ChaSelectionClass *selection_class = NULL;
	if (priv->selection) {
		selection_class = CHA_SELECTION_GET_CLASS(priv->selection);
		if (selection_class->preApply) {
			l_selection_pre_apply(document_view, revision_ref);
		}
	}

	ChaDocumentViewClass *document_view_class = CHA_DOCUMENT_VIEW_GET_CLASS(document_view);
	PangoLayout *pango_layout_scratch = pango_layout_new(pango_context);
	pango_layout_set_single_paragraph_mode(pango_layout_scratch, TRUE);
	if (priv->ctx.wrap_lines) {
		pango_layout_set_wrap(pango_layout_scratch, PANGO_WRAP_WORD);
		pango_layout_set_width(pango_layout_scratch, priv->ctx.text_view_width*PANGO_SCALE);
	}

	ChaFormWo *form = cha_revision_wo_get_form(revision_ref);
	int form_field_index = 0;

	CatArrayWo *e_form_fields_on_line = NULL;
	if (form) {
		e_form_fields_on_line = cat_array_wo_new();
	}

	gboolean do_draw_cursor = priv->has_focus && priv->cursor_blink_up;

	ChaSubLineCacheUpdate cache_update;
	cache_update.form_fields_on_line = e_form_fields_on_line;
	cache_update.form_fields_hash = 0;
	cache_update.view_width = view_width;
	cache_update.scratch = pango_layout_scratch;
	cache_update.a_preferences = priv->a_preferences;
	cache_update.window = window;
	cache_update.color_map = priv->color_map;
	cache_update.highlight_current_line = priv->highlight_current_line;


	for(page_idx=0; page_idx<page_cnt; page_idx++) {
		ChaPageWo *page = cha_revision_wo_page_at(revision_ref, page_idx);
		cat_log_detail("page=%o", page);
		ChaPageLayout *page_layout = (ChaPageLayout *) cha_page_wo_get_slot_content_ref(page, priv->last_slot_key_idx, NULL);
		cat_log_detail("page_layout=%o", page_layout);
		if (page_layout==NULL) {
			cat_log_debug("WARN: no page_layout page_idx=%d, page=%o", page_idx, page);
			// TODO need to create the pageview locally
			break;
		}


		int height = cha_page_layout_get_height(page_layout);
		cat_log_debug("page=%p, height=%d", page, height);
		if (height<0) {
			/* TODO: height probably -1 which means the page has not been layed out yet */
			cat_unref_ptr(page_layout);
			break;
		}

		cat_log_trace("page-view=%o, page_y=%d, view_top=%d, view_bottom=%d", page_layout, (int) page_y, (int) view_top, (int) view_bottom);
		if (page_y+height<view_top) {
			page_y += height;
			line_nr += cha_page_wo_line_count(page);
			cat_log_detail("page above");
			cat_unref_ptr(page_layout);
			continue;
		}
		if (page_y>=view_bottom) {
			cat_log_detail("page below");
			cat_unref_ptr(page_layout);
			break;
		}

		cat_log_detail("invalidate----page_layout=%p", page_layout);
		if (cha_page_layout_versions_start_update(page_layout, cat_wo_get_version((CatWo *) page), priv->ctx.text_layout_width, priv->ctx.font_version)) {
			ChaPageLayoutContext pg_lay_ctx;
			cha_document_view_layout_page(document_view, page, &pg_lay_ctx, page_layout, FALSE);
			cha_document_view_page_set_layout(document_view, page, page_layout, &(pg_lay_ctx), TRUE);
			cat_log_debug("page layout:page_layout=%o", page_layout);
			height = cha_page_layout_get_height(page_layout);
		}
		cat_log_trace("layout-page-version=%d, page-version=%d", cha_page_layout_get_page_version(page_layout), cat_wo_get_version((CatWo *) page));
		cat_unref_ptr(page_layout);


		cursor_line_idx = -1;
		if (cursor_page_idx==page_idx) {
			cursor_line_idx = cha_line_location_wo_get_page_line_index(a_cursor_line_location);
		}

		gboolean is_mono_space = priv->ctx.is_mono_space;
		int single_line_height = priv->ctx.line_height;

		cha_page_wo_hold_lines(page);
		cat_array_wo_append(new_pages_on_hold, (GObject *) page);


		long long int page_line_y = page_y;
		page_y += height;	// TODO: this is probably dangerous
		int line_cnt = cha_page_wo_line_count(page);
		cat_log_detail("line_cnt=%d", line_cnt);
		int line_idx;
		int exp_sub_idx = -1;
		for(line_idx=0; line_idx<line_cnt; line_idx++) {
			int line_width = 0;
			int sub_line_count = 0;

//			cat_log_detail("line_idx=%d", line_idx, );

			if (is_mono_space && !priv->ctx.wrap_lines) {
				if (page_line_y+single_line_height<view_top) {
					page_line_y += single_line_height;
					line_nr++;
					continue;
				}
				if (page_line_y>=view_bottom) {
					break;
				}
			}


			ChaLineWo *a_line = cha_page_wo_line_at(page, line_idx);
			cat_log_info("line=%o", a_line);

			ChaLineLayout *line_layout = cha_document_view_get_line_layout_ref(document_view, a_line);
			cha_line_layout_lock(line_layout, TRUE);


			gboolean new_text = cha_line_layout_set_text(line_layout, cha_line_wo_get_text(a_line), cha_line_wo_get_line_end(a_line), priv->ctx.wrap_lines, priv->ctx.tab_size, priv->ctx.text_view_width, priv->ctx.font_version);

			int phy_x_cursor_bytes = cha_cursor_wo_get_x_cursor_bytes(a_cursor);

			if (line_idx==cursor_line_idx) {
				new_text |= cha_line_layout_set_preedit(line_layout, priv->preedit_string, priv->preedit_attrs, priv->predit_cursor_pos, phy_x_cursor_bytes);
				phy_x_cursor_bytes += priv->predit_cursor_pos;
			} else {
				new_text |= cha_line_layout_set_preedit(line_layout, NULL, NULL, -1, -1);
			}


			cat_log_debug("new_text=%d, line-text=%o", new_text, cha_line_wo_get_text(a_line));

			if (new_text || FORCE_FULL_PAINT==1) {
				cha_line_layout_update(line_layout, pango_context, priv->color_map);
			}

			cat_log_debug("page_line_y=%ld, new_text=%d", page_line_y, new_text);


			cha_line_layout_get_width_and_sub_line_count(line_layout, &line_width, &sub_line_count);
			int line_height = sub_line_count*priv->ctx.line_height;
			cat_log_debug("line_idx=%d, line_height=%ld, view_top=%d, view_bottom=%d", line_idx, line_height, view_top, view_bottom);
			if (page_line_y+line_height<view_top) {
				page_line_y += line_height;
				cha_line_layout_unlock(line_layout);
				cat_unref_ptr(line_layout);
				cat_unref_ptr(a_line);
				line_nr++;
				continue;
			}
			if (page_line_y>=view_bottom) {
				cha_line_layout_unlock(line_layout);
				cat_unref_ptr(line_layout);
				cat_unref_ptr(a_line);
				break;
			}

			cat_log_trace("line_idx=%d, selection");

			if (selection_class) {
				selection_class->apply(priv->selection, pango_context, a_line, line_layout, page_idx, line_idx);
			} else {
				cha_line_layout_set_selection(line_layout, NULL, FALSE, NULL, -1);
			}

			if (cache_top==-1) {
				cache_top = page_line_y;
			}

			cat_log_trace("line_idx=%d, form-fields");

			/* handle form fields */
			int form_fields_hash = 0;
			if (form) {
				unsigned long long int current_row = line_nr;
				while(cat_array_wo_size(e_form_fields_on_line)!=0) {
					ChaFormFieldWo *field = (ChaFormFieldWo *) cat_array_wo_get_first(e_form_fields_on_line);
					if (cha_form_field_wo_on_row(field, current_row)) {
						break;
					}
					cat_array_wo_remove_first(e_form_fields_on_line, NULL);
				}

				while(TRUE) {
					ChaFormFieldWo *field = (ChaFormFieldWo *) cha_form_wo_get_at(form, form_field_index);
//					cat_log_warn("field=%o", field);
					if (field) {
						if (cha_form_field_wo_on_row(field, current_row)) {
							cat_array_wo_append(e_form_fields_on_line, (GObject *) field);
							form_field_index++;
							form_fields_hash = form_fields_hash*7+cha_form_field_wo_hash(field);
						} else {
							ChaCursorMWo *fld_start_cur = cha_form_field_wo_get_start(field);
							unsigned long long int fld_start_row = cha_cursorm_wo_get_row(fld_start_cur);
							if (fld_start_row>current_row) {
								break;
							}
							form_field_index++;
						}
					} else {
						break;
					}
				}
//				cat_log_warn("form_fields_hash=%d", form_fields_hash);
				if (cat_array_wo_size(e_form_fields_on_line)>0 && form_fields_hash==0) {
					form_fields_hash++;
				}
			}

			cache_update.form_fields_hash = form_fields_hash;

			cat_log_trace("line_idx=%d, invalidate");

			if (document_view_class->onInvalidateLine) {
				document_view_class->onInvalidateLine(document_view, page_idx, line_idx, a_line, line_layout);
			}

			cat_log_trace("line_idx=%d, line_layout_update");

			cha_line_layout_update(line_layout, pango_context, priv->color_map);
			int layout_version = cha_line_layout_get_layout_version(line_layout);
			PangoLayout *pango_layout = cha_line_layout_get_pango_layout(line_layout);

			cat_log_trace("line_idx=%d, layout_version=%d", layout_version);


			int cursor_sub_line_idx = -1;
			int cursor_xpos = -1;
			cache_update.cursor_width = 0;
			if (cursor_line_idx==line_idx) {

//				int cursor_idx = cha_cursor_wo_get_x_cursor_bytes(a_cursor);
				cat_log_debug("pango_layout=%p", pango_layout);
				pango_layout_index_to_line_x(pango_layout, phy_x_cursor_bytes, FALSE, &cursor_sub_line_idx, &cursor_xpos);

				if (priv->edit_mode==CHA_EDIT_MODE_OVERWRITE) {
					cache_update.cursor_width = priv->ctx.ps_space_width/PANGO_SCALE;
					if (cha_cursor_wo_get_x_sub(a_cursor)<=0) {
						CatStringWo *txt = cha_line_wo_get_text(a_line);
						if (cursor_xpos<cat_string_wo_length(txt)) {
							if (cat_string_wo_char_at(txt, cursor_xpos)!=9) {
								int cursor_xpos_r = -1;
								pango_layout_index_to_line_x(pango_layout, phy_x_cursor_bytes, TRUE, &cursor_sub_line_idx, &cursor_xpos_r);
								cache_update.cursor_width = (cursor_xpos_r-cursor_xpos)/PANGO_SCALE;
							}
						}
					}
				}
				cursor_xpos += (cha_cursor_wo_get_x_sub(a_cursor)*priv->ctx.ps_space_width);

				if (!do_draw_cursor) {
					cache_update.cursor_width = -1;
				}

			}



			int sub_line_idx;
			int sub_line_cnt = pango_layout_get_line_count(pango_layout);
			cat_log_info("creating cache for: line_idx=%d/%d, a_line=%o, sub_line_cnt=%d, line_height=%d, view_bottom=%ld, page_line_y=%ld, page_y=%ld, page.height=%d", line_idx, page_idx, cha_line_wo_get_text(a_line), sub_line_cnt, line_height, view_bottom, page_line_y, page_y, height);
			long long int page_sub_line_y = page_line_y;

			cache_update.pango_layout = pango_layout;
			cache_update.selection_markings = cha_line_layout_get_selection_markings(line_layout);
			cache_update.selection_eol = cha_line_layout_get_selection_eol(line_layout);

			for(sub_line_idx=0; sub_line_idx<sub_line_cnt; sub_line_idx++) {
				int sub_line_cache_idx = l_find_sub_line_cache(priv->sub_line_cache, line_layout, sub_line_idx, exp_sub_idx);
				ChaSubLineCache *sub_line_cache = NULL;
				cat_log_debug("sub_line_cache_idx=%d", sub_line_cache_idx);
				if (sub_line_cache_idx<0) {
					if (document_view_class->createSubLineCache) {
						sub_line_cache = document_view_class->createSubLineCache(priv->surface_pool, line_layout, sub_line_idx);
					} else {
						sub_line_cache = cha_sub_line_cache_new(priv->surface_pool, line_layout, sub_line_idx);
					}
				} else {
					sub_line_cache = (ChaSubLineCache *) cat_array_wo_get(priv->sub_line_cache, sub_line_cache_idx);
					exp_sub_idx = sub_line_cache_idx+1;
					cat_ref_ptr(sub_line_cache);
				}

//				PangoLayoutLine *pango_line = pango_layout_get_line_readonly(pango_layout, sub_line_idx);
//				PangoRectangle sub_line_rect;
//				pango_layout_line_get_pixel_extents(pango_line, NULL, &sub_line_rect);
//				int sub_line_height = sub_line_rect.height;
				int sub_line_height = single_line_height;


				cache_update.page_sub_line_y = page_sub_line_y;
				cache_update.view_x = view_x;
				cache_update.sub_line_height = sub_line_height;
				cache_update.attributes_version = layout_version;
				cache_update.cursor_x_pos = cursor_sub_line_idx==sub_line_idx ? cursor_xpos/PANGO_SCALE : -1;
				cache_update.line_nr = line_nr;
				cache_update.line_nr_view_width = priv->ctx.line_nr_view_width;
				cache_update.paint_mark = cha_page_wo_is_line_marked(page, line_idx);
				cache_update.show_whitespace = priv->ctx.show_whitespace;


				gboolean needs_repaint = cha_sub_line_cache_update(sub_line_cache, &cache_update);
				if (needs_repaint || FORCE_FULL_PAINT==1) {
					int yposs=(int) (page_sub_line_y-priv->view_y);
//					if (!priv->in_scroll) {
						gtk_widget_queue_draw_area(priv->widget, 0, yposs, view_width, line_height);
						cat_log_debug("invalidating %o, yposs=%d, line_height=%d", sub_line_cache, yposs, line_height);
//					}
				}


				cat_log_debug("sub_line_height=%d", sub_line_height);
				page_sub_line_y += sub_line_height;
				cat_log_debug("adding %o", sub_line_cache);
				cat_array_wo_append(new_cache, (GObject *) sub_line_cache);
				cat_unref_ptr(sub_line_cache);
			}

			cha_line_layout_unlock(line_layout);
			cat_unref_ptr(line_layout);


			page_line_y += line_height;
			if (page_line_y!=page_sub_line_y) {
				cat_log_error("Wrong: %d, line_height=%d, sub_line_cnt=%d, line_height=%d, new_text=%d, text_vw=%d", (int) line_nr, line_height, sub_line_cnt, line_height, new_text, priv->ctx.text_view_width);

			}
			line_nr++;

			cache_bottom = page_line_y;
			cat_unref_ptr(a_line);
		}

		cat_log_debug("page=%p, page-layout=%p, act_height=%d, height=%d", page, page_layout, (int) (page_line_y - (page_y-height)), height);

	}


	l_replace_pages_on_hold(priv, new_pages_on_hold);
	cat_unref_ptr(pango_layout_scratch);

	priv->cache_view_y = cache_top;
	priv->cache_view_height = (int) (cache_bottom-cache_top);

	cat_ref_swap(priv->sub_line_cache, new_cache);
	cat_unref_ptr(new_cache);



	int ypos_ll_on_screen = (int) (page_y-priv->view_y);


	if (priv->view_height>ypos_ll_on_screen) {
		gtk_widget_queue_draw_area(priv->widget, 0, ypos_ll_on_screen , view_width, priv->view_height- ypos_ll_on_screen);
	}


	cat_log_on_info(
		long long endTs = cat_monotonic_time();
	)
	cat_log_info("#### draw-time=%d, content-version=%d", (int) (endTs-startTs), cha_revision_wo_get_content_version(revision_ref));


}

void cha_document_view_draw(ChaDocumentView *document_view, cairo_t *cr) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
//	if (priv->in_scroll) {
//		priv->in_scroll = FALSE;
//		cat_log_debug("BREAK");
//		return;
//	}
	cat_log_debug("draw");

	cat_log_debug("Doing the draw:%p ypos=%d", priv->sub_line_cache, (int) priv->view_y);


//	cairo_rectangle_list_t *list_t = cairo_copy_clip_rectangle_list(cr);
//	if (list_t) {
//		cat_log_detail("list_t=%p", list_t);
//		int idx;
//		for(idx=list_t->num_rectangles-1; idx>=0; idx--) {
//			cairo_rectangle_t rect = list_t->rectangles[idx];
//			cat_log_warn("r[%d]=[%d,%d - %d,%d]",idx, (int) rect.x, (int) rect.y, (int) rect.width, (int) rect.height);
//		}
//	}

	cairo_translate(cr, 0, (double) -priv->view_y);

	double clipx, clipy, clipwidth,clipheight;
	cairo_clip_extents(cr, &clipx, &clipy, &clipwidth, &clipheight);

	long long bottom = priv->view_y;

	if (priv->sub_line_cache) {
		CatIIterator *iter = cat_array_wo_iterator(priv->sub_line_cache);
		CatIIteratorInterface *iter_iface = CAT_IITERATOR_GET_INTERFACE(iter);
		ChaSubLineCache *line_cache = NULL;
		while(iter_iface->has_next(iter)) {
			line_cache = (ChaSubLineCache *) iter_iface->next(iter);
			cha_sub_line_cache_draw(line_cache, cr, clipy, clipheight);
		}
		cat_unref_ptr(iter);
		if (line_cache) {
			bottom = cha_sub_line_cache_get_bottom(line_cache);
		}
	}

	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

	cat_log_debug("bottom=%ld, priv->view_y=%ld", bottom, priv->view_y);


	cairo_rectangle(cr, 0, bottom, priv->view_width, priv->view_height - (bottom-priv->view_y));
	cairo_fill(cr);



//	cairo_set_source_rgb(cr, 0.5, 0,0);
//	cairo_move_to(cr, 0, priv->view_height-2);
//	cairo_line_to(cr, 100, priv->view_height-2);
//	cairo_stroke(cr);
//
//	cairo_move_to(cr, priv->view_width-2,0);
//	cairo_line_to(cr, priv->view_width-2,100);
//	cairo_stroke(cr);

}

/********************* start ChaIDocumentListener implementation *********************/

static gboolean l_post_layout_requests(ChaDocumentView *document_view, ChaRevisionWo *a_rev) {
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);

	cat_log_debug("a_rev=%o", a_rev);
	int page_list_version = cha_revision_wo_get_page_list_version(a_rev);
	if (priv->last_revision_page_list_version != page_list_version) {
		priv->last_revision_page_list_version = page_list_version;
		l_run_relayout_pages(document_view, a_rev, "post-layout-request");
		return TRUE;
	}
	return FALSE;
}

static void l_on_new_revision(ChaIDocumentListener *self, ChaRevisionWo *a_new_revision) {
	ChaDocumentView *instance = CHA_DOCUMENT_VIEW(self);
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(instance);

	if (priv->a_revision_wo==a_new_revision) {
		return;
	}

	ChaRevisionWo *prev_rev = priv->a_revision_wo;
	priv->a_revision_wo = cat_ref_ptr(a_new_revision);
	if (!l_post_layout_requests(instance, a_new_revision)) {
		gboolean run_invalidate = FALSE;
		/* If the content did not change then check if the cursor moved */
		ChaCursorWo *cur_cur = prev_rev!=NULL ? cha_revision_wo_get_cursor(prev_rev) : NULL;
		ChaCursorWo *ncursor = cha_revision_wo_get_cursor(a_new_revision);
		if (!cha_cursor_wo_equal(cur_cur, ncursor)) {
			run_invalidate = TRUE;
		} else {
			gboolean new_has_form = cha_revision_wo_get_form(a_new_revision)!=NULL;
			gboolean old_has_form = cha_revision_wo_get_form(prev_rev)!=NULL;
			run_invalidate = old_has_form!=new_has_form;
			if (!run_invalidate) {
				if ((priv->selection==NULL && priv->selection_mod_count!=-1) ||
						(priv->selection && cha_selection_get_protected(priv->selection)->modification_count!=priv->selection_mod_count)) {
					run_invalidate = TRUE;
				}
			}
		}

		if (run_invalidate) {
			cha_document_view_invalidate_lines(instance);
		}
	}
	cat_unref_ptr(prev_rev);
}

static void l_on_slot_registered(ChaIDocumentListener *self, ChaRevisionWo *a_revision, ChaEnrichmentDataMapWo *a_map, GObject *slot_key, int slot_index) {
	ChaDocumentView *instance = CHA_DOCUMENT_VIEW(self);
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(instance);
	if (((GObject *) priv->a_slot_key) != slot_key) {
		return;
	}

	int page_cnt = cha_revision_wo_page_count(a_revision);
	int page_idx;
	for(page_idx=0; page_idx<page_cnt; page_idx++) {
		ChaPageWo *page = cha_revision_wo_page_at(a_revision, page_idx);
		ChaPageLayout *page_view = cha_page_layout_new();
		cha_page_wo_set_slot_content(page, slot_index, NULL, (GObject *) page_view);
		cat_log_trace("slot_index=%d", slot_index);

//		ChaPageLayoutRequest *req = cha_page_layout_request_new(instance, page, text_view_width, (GObject *) priv->a_slot_key, priv->last_slot_key_idx, page_idx);
//		cha_document_manager_post(document_manager, (WorRequest *) req);
//		cat_unref_ptr(req);
		cat_unref_ptr(page_view);
	}
	cat_ref_swap(priv->a_revision_wo, a_revision);
	l_post_layout_requests(instance, a_revision);
}


static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface) {
	iface->onNewRevision = l_on_new_revision;
	iface->onSlotRegistered = l_on_slot_registered;
}

/********************* end ChaIDocumentListener implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaDocumentView *instance = CHA_DOCUMENT_VIEW(self);
	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p slot-key=%o]", iname, self, priv->a_slot_key);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

