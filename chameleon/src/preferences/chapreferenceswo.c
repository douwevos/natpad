/*
   File:    chapreferenceswo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 19, 2014
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

#include "chapreferenceswo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaPreferencesWo"
#include <logging/catlog.h>

struct _ChaPreferencesWoPrivate {
	gboolean do_show_white_space;
	gboolean do_blink_cursor;
	gboolean word_wrap_lines;
	gboolean show_line_numbers;
	gboolean limit_to_text;
	gboolean highlight_current_line;
	gboolean block_mode;
	int tab_size;
	gboolean escape_key;
	gboolean deprecated_scrolling;


	CatStringWo *font_name;
	CatStringWo *big_mode_font_name;
	gboolean big_mode_force_ascii;
	int font_weight;

	ChaPrefsColorMapWo *color_map;

	CatHashMapWo *template_map;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPreferencesWo, cha_preferences_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaPreferencesWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void cha_preferences_wo_class_init(ChaPreferencesWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->equal = l_equal;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_preferences_wo_init(ChaPreferencesWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaPreferencesWo *instance = CHA_PREFERENCES_WO(object);
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(instance);
	cat_unref_ptr(priv->color_map);
	cat_unref_ptr(priv->font_name);
	cat_unref_ptr(priv->big_mode_font_name);
	cat_unref_ptr(priv->template_map);
	G_OBJECT_CLASS(cha_preferences_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_preferences_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void cha_preferences_wo_construct(ChaPreferencesWo *prefs) {
	cat_wo_construct((CatWo *) prefs, TRUE);
}

ChaPreferencesWo *cha_preferences_wo_new() {
	ChaPreferencesWo *result = g_object_new(CHA_TYPE_PREFERENCES_WO, NULL);
	cat_ref_anounce(result);
	cha_preferences_wo_construct(result);
	return result;
}


gboolean cha_preferences_wo_get_blink_cursor(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->do_blink_cursor;
}

gboolean cha_preferences_wo_get_show_whitespace(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->do_show_white_space;
}

gboolean cha_preferences_wo_get_wrap_lines(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->word_wrap_lines;
}

gboolean cha_preferences_wo_get_show_line_numbers(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->show_line_numbers;
}

gboolean cha_preferences_wo_get_limit_cursor(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->limit_to_text;
}

gboolean cha_preferences_wo_get_highlight_current_line(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->highlight_current_line;
}

gboolean cha_preferences_wo_get_block_selection(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->block_mode;
}

gboolean cha_preferences_wo_get_deprecated_scrolling(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->deprecated_scrolling;
}

gboolean cha_preferences_wo_get_escape_key(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->escape_key;
}

int cha_preferences_wo_get_tab_size(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->tab_size;
}


CatStringWo *cha_preferences_wo_get_font_name(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->font_name;
}

CatStringWo *cha_preferences_wo_get_big_mode_font_name(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->big_mode_font_name;
}

gboolean cha_preferences_wo_get_big_mode_force_ascii(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->big_mode_force_ascii;
}


int cha_preferences_wo_get_font_weight(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->font_weight;
}

ChaPrefsColorMapWo *cha_preferences_wo_get_color_map(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->color_map;
}


CatHashMapWo *cha_preferences_wo_get_template_map(const ChaPreferencesWo *prefs) {
	const ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) prefs);
	return priv->template_map;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_prefs)) { \
			cat_log_error("Object is read only:%o", e_prefs); \
			return rval; \
		} \


void cha_preferences_wo_set_blink_cursor(ChaPreferencesWo *e_prefs, gboolean blink_cursor) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	priv->do_blink_cursor = blink_cursor;
}

void cha_preferences_wo_set_show_whitespace(ChaPreferencesWo *e_prefs, gboolean show_whitespace) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	priv->do_show_white_space = show_whitespace;
}

void cha_preferences_wo_set_wrap_lines(ChaPreferencesWo *e_prefs, gboolean wrap_lines) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	priv->word_wrap_lines = wrap_lines;
}

void cha_preferences_wo_set_show_line_numbers(ChaPreferencesWo *e_prefs, gboolean show_line_numbers) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	priv->show_line_numbers = show_line_numbers;
}

void cha_preferences_wo_set_limit_cursor(ChaPreferencesWo *e_prefs, gboolean limit_cursor) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	priv->limit_to_text = limit_cursor;
}

void cha_preferences_wo_set_highlight_current_line(ChaPreferencesWo *e_prefs, gboolean highlight_current_line) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	priv->highlight_current_line = highlight_current_line;
}


void cha_preferences_wo_set_block_selection(ChaPreferencesWo *e_prefs, gboolean block_selection) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	priv->block_mode = block_selection;
}

void cha_preferences_wo_set_deprecated_scrolling(ChaPreferencesWo *e_prefs, gboolean deprecated_scrolling) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	priv->deprecated_scrolling = deprecated_scrolling;
}

void cha_preferences_wo_set_tab_size(ChaPreferencesWo *e_prefs, int tab_size) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	priv->tab_size = tab_size;
}


void cha_preferences_wo_set_font_name(ChaPreferencesWo *e_prefs, CatStringWo *font_name) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->font_name, font_name);
}

void cha_preferences_wo_set_big_mode_force_ascii(ChaPreferencesWo *e_prefs, gboolean force_ascii) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	priv->big_mode_force_ascii = force_ascii;
}

void cha_preferences_wo_set_big_mode_font_name(ChaPreferencesWo *e_prefs, CatStringWo *font_name) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->big_mode_font_name, font_name);
}

void cha_preferences_wo_set_font_weight(ChaPreferencesWo *e_prefs, int font_weight) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	priv->font_weight = font_weight;
}


ChaPrefsColorMapWo *cha_preferences_wo_get_editable_color_map(ChaPreferencesWo *e_prefs) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	if (cha_prefs_color_map_wo_is_anchored(priv->color_map)) {
		ChaPrefsColorMapWo *e_map = cha_prefs_color_map_wo_create_editable(priv->color_map);
		CHECK_IF_WRITABLE(NULL);
		cat_ref_swap(priv->color_map, e_map);
	}
	return priv->color_map;
}

CatHashMapWo *cha_preferences_wo_get_editable_template_map(ChaPreferencesWo *e_prefs) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(e_prefs);
	if (cat_hash_map_wo_is_anchored(priv->template_map)) {
		CatHashMapWo *e_map = cat_hash_map_wo_create_editable(priv->template_map);
		CHECK_IF_WRITABLE(NULL);
		cat_ref_swap(priv->template_map, e_map);
	}
	return priv->template_map;
}

static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_PREFERENCES_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) e_uninitialized);
	if (original) {
		ChaPreferencesWoPrivate *rpriv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) original);
		priv->do_show_white_space = rpriv->do_show_white_space;
		priv->do_blink_cursor = rpriv->do_blink_cursor;
		priv->word_wrap_lines = rpriv->word_wrap_lines;
		priv->show_line_numbers = rpriv->show_line_numbers;
		priv->limit_to_text = rpriv->limit_to_text;
		priv->highlight_current_line = rpriv->highlight_current_line;
		priv->block_mode = rpriv->block_mode;
		priv->tab_size = rpriv->tab_size;
		priv->escape_key = rpriv->escape_key;
		priv->deprecated_scrolling = rpriv->deprecated_scrolling;

		priv->font_name = cat_ref_ptr(rpriv->font_name);
		priv->font_weight = rpriv->font_weight;
		priv->big_mode_font_name = cat_ref_ptr(rpriv->big_mode_font_name);
		priv->big_mode_force_ascii = rpriv->big_mode_force_ascii;

		priv->color_map = cat_ref_ptr(rpriv->color_map);

		priv->template_map = cat_ref_ptr(rpriv->template_map);
	} else {

		priv->do_blink_cursor = TRUE;
		priv->do_show_white_space = FALSE;
		priv->word_wrap_lines = FALSE;
		priv->show_line_numbers = FALSE;
		priv->limit_to_text = TRUE;
		priv->highlight_current_line = TRUE;
		priv->block_mode = FALSE;
		priv->tab_size = 4;
		priv->escape_key = FALSE;
		priv->deprecated_scrolling = FALSE;

		priv->font_name = NULL;
		priv->font_weight = PANGO_WEIGHT_NORMAL;
		priv->big_mode_font_name = NULL;
		priv->big_mode_force_ascii = TRUE;

		priv->color_map = cha_prefs_color_map_wo_new();

		priv->template_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	}
	return CAT_WO_CLASS(cha_preferences_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static void l_anchor_children(CatWo *wo, int version) {
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) wo);
	priv->color_map = cha_prefs_color_map_wo_anchor(priv->color_map, version);
	priv->template_map = cat_hash_map_wo_anchor(priv->template_map, version);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaPreferencesWoPrivate *priv_a = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) wo_a);
	const ChaPreferencesWoPrivate *priv_b = cha_preferences_wo_get_instance_private((ChaPreferencesWo *) wo_b);

	return (priv_a->do_show_white_space == priv_b->do_show_white_space)
			&& (priv_a->do_blink_cursor == priv_b->do_blink_cursor)
			&& (priv_a->word_wrap_lines == priv_b->word_wrap_lines)
			&& (priv_a->show_line_numbers == priv_b->show_line_numbers)
			&& (priv_a->limit_to_text == priv_b->limit_to_text)
			&& (priv_a->highlight_current_line == priv_b->highlight_current_line)
			&& (priv_a->block_mode == priv_b->block_mode)
			&& (priv_a->tab_size == priv_b->tab_size)
			&& (priv_a->escape_key == priv_b->escape_key)
			&& (priv_a->deprecated_scrolling == priv_b->deprecated_scrolling)
			&& (priv_a->font_weight == priv_b->font_weight)
			&& (priv_a->big_mode_force_ascii == priv_b->big_mode_force_ascii)
			&& (cat_string_wo_equal(priv_a->big_mode_font_name, priv_b->big_mode_font_name))
			&& (cat_string_wo_equal(priv_a->font_name, priv_b->font_name))
			&& cha_prefs_color_map_wo_equal(priv_a->color_map, priv_b->color_map)
			&& cat_hash_map_wo_equal(priv_a->template_map, priv_b->template_map, NULL);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized == NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_PREFERENCES_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(CHA_PREFERENCES_WO(e_uninitialized));
	if (wo_source) {
		const ChaPreferencesWoPrivate *priv_src = cha_preferences_wo_get_instance_private(CHA_PREFERENCES_WO(wo_source));
		priv->do_show_white_space = priv_src->do_show_white_space;
		priv->do_blink_cursor = priv_src->do_blink_cursor;
		priv->word_wrap_lines = priv_src->word_wrap_lines;
		priv->show_line_numbers = priv_src->show_line_numbers;
		priv->limit_to_text = priv_src->limit_to_text;
		priv->highlight_current_line = priv_src->highlight_current_line;
		priv->block_mode = priv_src->block_mode;
		priv->tab_size = priv_src->tab_size;
		priv->escape_key = priv_src->escape_key;
		priv->deprecated_scrolling = priv_src->deprecated_scrolling;
		priv->font_name = cat_string_wo_clone(priv_src->font_name, CAT_CLONE_DEPTH_NONE);
		priv->font_weight = priv_src->font_weight;
		priv->big_mode_force_ascii = priv_src->big_mode_force_ascii;
		priv->big_mode_font_name = cat_string_wo_clone(priv_src->big_mode_font_name, CAT_CLONE_DEPTH_NONE);
		priv->color_map = cha_prefs_color_map_wo_clone(priv_src->color_map, CAT_CLONE_DEPTH_NONE);
		priv->template_map = cat_hash_map_wo_clone(priv_src->template_map, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->do_blink_cursor = TRUE;
		priv->do_show_white_space = FALSE;
		priv->word_wrap_lines = FALSE;
		priv->show_line_numbers = FALSE;
		priv->limit_to_text = TRUE;
		priv->highlight_current_line = TRUE;
		priv->block_mode = FALSE;
		priv->tab_size = 4;
		priv->escape_key = FALSE;
		priv->deprecated_scrolling = FALSE;

		priv->font_name = NULL;
		priv->font_weight = PANGO_WEIGHT_NORMAL;
		priv->big_mode_force_ascii = TRUE;
		priv->big_mode_font_name = NULL;

		priv->color_map = cha_prefs_color_map_wo_new();

		priv->template_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	}


	CatWoClass *woc = CAT_WO_CLASS(cha_preferences_wo_parent_class);
	if (woc->cloneContent) {
		return woc->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;

}

CAT_WO_BASE_C(ChaPreferencesWo, cha_preferences_wo)

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaPreferencesWo *instance = CHA_PREFERENCES_WO(self);
	ChaPreferencesWoPrivate *priv = cha_preferences_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: %s font=%O, whsp=%d, blcu=%d, wowr=%d, shnr=%d, limt=%d, blck=%d, tabs=%d, esck=%d, x11s=%d, colormap=%o, template_map=%o]", iname, self, cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->font_name, priv->do_show_white_space, priv->do_blink_cursor, priv->word_wrap_lines, priv->show_line_numbers, priv->limit_to_text, priv->block_mode, priv->tab_size, priv->escape_key, priv->deprecated_scrolling, priv->color_map, priv->template_map);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/







