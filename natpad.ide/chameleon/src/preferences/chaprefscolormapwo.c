/*
   File:    chaprefscolormapwo.c
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

#include "chaprefscolormapwo.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaPrefsColorMapWo"
#include <logging/catlog.h>

struct _ChaPrefsColorMapWoPrivate {
	ChaColorEntry *entry_array;
	int entry_count;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPrefsColorMapWo, cha_prefs_color_map_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaPrefsColorMapWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void cha_prefs_color_map_wo_class_init(ChaPrefsColorMapWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->constructEditable = l_construct_editable;
	wo_class->equal = l_equal;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_prefs_color_map_wo_init(ChaPrefsColorMapWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaPrefsColorMapWo *instance = CHA_PREFS_COLOR_MAP_WO(object);
	ChaPrefsColorMapWoPrivate *priv = cha_prefs_color_map_wo_get_instance_private(instance);

	ChaPrefsColorMapWo *original = (ChaPrefsColorMapWo *) cat_wo_get_original((CatWo *) instance);

	if (original) {
		ChaPrefsColorMapWoPrivate *spriv = cha_prefs_color_map_wo_get_instance_private(original);
		if (spriv->entry_array==priv->entry_array) {
			priv->entry_array = NULL;
		}
	}
	cat_free_ptr(priv->entry_array);
	priv->entry_count = 0;
	G_OBJECT_CLASS(cha_prefs_color_map_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_prefs_color_map_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

#define ENTRY_COUNT 18
#define SET_ENTRY(field, r, g, b)  { (field)->red = ((double) r)/0xFFFF; (field)->green=((double) g)/0xFFFF; (field)->blue=((double) b)/0xFFFF; }

ChaPrefsColorMapWo *cha_prefs_color_map_wo_new() {
	ChaPrefsColorMapWo *result = g_object_new(CHA_TYPE_PREFS_COLOR_MAP_WO, NULL);
	cat_ref_anounce(result);
	cat_wo_construct((CatWo *) result, TRUE);
	return result;
}


void cha_prefs_color_map_wo_set_cairo_color(const ChaPrefsColorMapWo *color_map, cairo_t *cairo, int color_idx) {
	const ChaPrefsColorMapWoPrivate *priv = cha_prefs_color_map_wo_get_instance_private((ChaPrefsColorMapWo *) color_map);

	if (color_idx<0 || color_idx>=priv->entry_count) {
		color_idx = 1;
		if (color_idx<0 || color_idx>=priv->entry_count) {
			cairo_set_source_rgb(cairo, 0, 0, 0);
			return;
		}
	}

	const ChaColorEntry *entry = priv->entry_array+color_idx;
	cairo_set_source_rgb(cairo, entry->red, entry->green, entry->blue);
}

int cha_prefs_color_map_wo_count(const ChaPrefsColorMapWo *color_map) {
	const ChaPrefsColorMapWoPrivate *priv = cha_prefs_color_map_wo_get_instance_private((ChaPrefsColorMapWo *) color_map);
	return priv->entry_count;
}


ChaColorEntry cha_prefs_color_map_wo_get_color(const ChaPrefsColorMapWo *color_map, int color_idx) {
	const ChaPrefsColorMapWoPrivate *priv = cha_prefs_color_map_wo_get_instance_private((ChaPrefsColorMapWo *) color_map);

	if (color_idx<0 || color_idx>=priv->entry_count) {
		color_idx = 1;
		if (color_idx<0 || color_idx>=priv->entry_count) {
			ChaColorEntry r = {0};
			return r;
		}
	}

	return *(priv->entry_array+color_idx);
}


ChaColorEntryInt cha_prefs_color_map_wo_get_color_int(const ChaPrefsColorMapWo *color_map, int color_idx) {
	const ChaColorEntry cce = cha_prefs_color_map_wo_get_color(color_map, color_idx);
	ChaColorEntryInt result;
	result.red = (guint16 ) (cce.red*0xFFFF);
	result.green = (guint16 ) (cce.green*0xFFFF);
	result.blue = (guint16 ) (cce.blue*0xFFFF);
	return result;
}



#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_color_map)) { \
			cat_log_error("Object is read only:%o", e_color_map); \
			return rval; \
		} \


void cha_prefs_color_map_wo_set_entry_count(ChaPrefsColorMapWo *e_color_map, int entry_count) {
	ChaPrefsColorMapWoPrivate *priv = cha_prefs_color_map_wo_get_instance_private(e_color_map);
	CHECK_IF_WRITABLE();
	if (entry_count >= priv->entry_count) {
		return;
	}

	ChaPrefsColorMapWo *original = (ChaPrefsColorMapWo *) cat_wo_get_original((CatWo *) e_color_map);
	if (original) {
		ChaPrefsColorMapWoPrivate *spriv = cha_prefs_color_map_wo_get_instance_private(original);
		if (spriv->entry_array == priv->entry_array) {
			priv->entry_count = entry_count;
			priv->entry_array = g_new(ChaColorEntry, entry_count);
			int idx;
			for(idx=spriv->entry_count-1; idx>=0; idx--) {
				priv->entry_array[idx] = spriv->entry_array[idx];
			}
			for(idx=entry_count-1; idx>=spriv->entry_count; idx--) {
				priv->entry_array[idx].red =  0;
				priv->entry_array[idx].green = 0;
				priv->entry_array[idx].blue = 0;
			}
			return;
		}
	}

	ChaColorEntry *new_entries = g_new(ChaColorEntry, entry_count);
	int idx;
	for(idx=priv->entry_count-1; idx>=0; idx--) {
		new_entries[idx] = priv->entry_array[idx];
	}
	for(idx=entry_count-1; idx>=priv->entry_count; idx--) {
		new_entries[idx].red =  0;
		new_entries[idx].green = 0;
		new_entries[idx].blue = 0;
	}

	cat_free_ptr(priv->entry_array);
	priv->entry_array = new_entries;
	priv->entry_count = entry_count;
}

void cha_prefs_color_map_wo_set_color_at(ChaPrefsColorMapWo *e_color_map, double red, double green, double blue, int color_idx) {
	ChaPrefsColorMapWoPrivate *priv = cha_prefs_color_map_wo_get_instance_private(e_color_map);
	CHECK_IF_WRITABLE();
	if (color_idx<0 || color_idx >= priv->entry_count) {
		cat_log_warn("index:%d out of bounds:%d", color_idx, priv->entry_count);
		return;
	}

	ChaPrefsColorMapWo *original = (ChaPrefsColorMapWo *) cat_wo_get_original((CatWo *) e_color_map);
	if (original) {
		ChaColorEntry *entry = priv->entry_array + color_idx;
		if (entry->red==red && entry->green && entry->blue==blue) {
			return;
		}
		ChaPrefsColorMapWoPrivate *spriv = cha_prefs_color_map_wo_get_instance_private(original);
		if (spriv->entry_array == priv->entry_array) {
			priv->entry_count = spriv->entry_count;
			priv->entry_array = g_new(ChaColorEntry, priv->entry_count);
			int idx;
			for(idx=spriv->entry_count-1; idx>=0; idx--) {
				priv->entry_array[idx] = spriv->entry_array[idx];
			}
		}
	}

	ChaColorEntry *entry = priv->entry_array + color_idx;
	entry->red = red;
	entry->green = green;
	entry->blue = blue;
}



static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_PREFS_COLOR_MAP_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaPrefsColorMapWoPrivate *priv = cha_prefs_color_map_wo_get_instance_private((ChaPrefsColorMapWo *) e_uninitialized);
	if (original) {
		ChaPrefsColorMapWoPrivate *rpriv = cha_prefs_color_map_wo_get_instance_private((ChaPrefsColorMapWo *) original);
		priv->entry_array = rpriv->entry_array;
		priv->entry_count = rpriv->entry_count;
	} else {
		priv->entry_count = ENTRY_COUNT;
		priv->entry_array = g_new(ChaColorEntry, ENTRY_COUNT);

		SET_ENTRY(priv->entry_array + 0, 0xFFFF, 0xFFFF, 0xFFFF);
		SET_ENTRY(priv->entry_array + 1, 0x0000, 0x0000, 0x0000);
		SET_ENTRY(priv->entry_array + 2, 0x3333, 0x3333, 0xAAAA);
		SET_ENTRY(priv->entry_array + 3, 0x5F5F, 0x9E9E, 0xA0A0);
		SET_ENTRY(priv->entry_array + 4, 0xB2B2, 0x2222, 0x2222);
		SET_ENTRY(priv->entry_array + 5, 0xDADA, 0x7070, 0xD6D6);
		SET_ENTRY(priv->entry_array + 6, 0xBCBC, 0x6F6F, 0x6F6F);
		SET_ENTRY(priv->entry_array + 7, 0x2222, 0x8B8B, 0x2222);
		SET_ENTRY(priv->entry_array + 8, 0xB8B8, 0x8686, 0x0B0B);
		SET_ENTRY(priv->entry_array + 9, 0x0000, 0x0000, 0xFFFF);
		SET_ENTRY(priv->entry_array + 10, 0xA0A0, 0x2020, 0xF0F0);
		SET_ENTRY(priv->entry_array + 11, 0x0000, 0x5050, 0x5050);
		SET_ENTRY(priv->entry_array + 12, 0x2222, 0x8B8B, 0x2222);
		SET_ENTRY(priv->entry_array + 13, 0x6060, 0x0000, 0xA0A0);
		SET_ENTRY(priv->entry_array + 14, 0x3535, 0xD9D9, 0x3535);
		SET_ENTRY(priv->entry_array + 15, 0xAAAA, 0xAAAA, 0xAAAA);
		SET_ENTRY(priv->entry_array + 16, 0xDEDE, 0xFFFF, 0xC4C4);
		SET_ENTRY(priv->entry_array + 17, 0x3333, 0x3333, 0xFFFF);
	}
	return CAT_WO_CLASS(cha_prefs_color_map_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaPrefsColorMapWoPrivate *priv_a = cha_prefs_color_map_wo_get_instance_private((ChaPrefsColorMapWo *) wo_a);
	const ChaPrefsColorMapWoPrivate *priv_b = cha_prefs_color_map_wo_get_instance_private((ChaPrefsColorMapWo *) wo_b);
	if (priv_a->entry_count!=priv_b->entry_count) {
		return FALSE;
	}
	if (priv_a->entry_array==priv_b->entry_array) {
		return TRUE;
	}
	return memcmp(priv_a->entry_array, priv_b->entry_array, sizeof(ChaColorEntry)*priv_a->entry_count)==0;
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_PREFS_COLOR_MAP_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaPrefsColorMapWoPrivate *priv = cha_prefs_color_map_wo_get_instance_private(CHA_PREFS_COLOR_MAP_WO(e_uninitialized));
	if (wo_source) {
		ChaPrefsColorMapWoPrivate *priv_src = cha_prefs_color_map_wo_get_instance_private(CHA_PREFS_COLOR_MAP_WO(wo_source));
		if (!cat_wo_is_anchored(wo_source)) {
			priv->entry_count = priv_src->entry_count;
			priv->entry_array = g_new(ChaColorEntry, priv->entry_count);
			int idx;
			for(idx=priv->entry_count-1; idx>=0; idx--) {
				priv->entry_array[idx] = priv_src->entry_array[idx];
			}
		} else {
			priv->entry_array = priv_src->entry_array;
			priv->entry_count = priv_src->entry_count;
		}
	} else {
		priv->entry_count = ENTRY_COUNT;
		priv->entry_array = g_new(ChaColorEntry, ENTRY_COUNT);

		SET_ENTRY(priv->entry_array + 0, 0xFFFF, 0xFFFF, 0xFFFF);
		SET_ENTRY(priv->entry_array + 1, 0x0000, 0x0000, 0x0000);
		SET_ENTRY(priv->entry_array + 2, 0x3333, 0x3333, 0xAAAA);
		SET_ENTRY(priv->entry_array + 3, 0x5F5F, 0x9E9E, 0xA0A0);
		SET_ENTRY(priv->entry_array + 4, 0xB2B2, 0x2222, 0x2222);
		SET_ENTRY(priv->entry_array + 5, 0xDADA, 0x7070, 0xD6D6);
		SET_ENTRY(priv->entry_array + 6, 0xBCBC, 0x6F6F, 0x6F6F);
		SET_ENTRY(priv->entry_array + 7, 0x2222, 0x8B8B, 0x2222);
		SET_ENTRY(priv->entry_array + 8, 0xB8B8, 0x8686, 0x0B0B);
		SET_ENTRY(priv->entry_array + 9, 0x0000, 0x0000, 0xFFFF);
		SET_ENTRY(priv->entry_array + 10, 0xA0A0, 0x2020, 0xF0F0);
		SET_ENTRY(priv->entry_array + 11, 0x0000, 0x5050, 0x5050);
		SET_ENTRY(priv->entry_array + 12, 0x2222, 0x8B8B, 0x2222);
		SET_ENTRY(priv->entry_array + 13, 0x6060, 0x0000, 0xA0A0);
		SET_ENTRY(priv->entry_array + 14, 0x3535, 0xD9D9, 0x3535);
		SET_ENTRY(priv->entry_array + 15, 0xAAAA, 0xAAAA, 0xAAAA);
		SET_ENTRY(priv->entry_array + 16, 0xDEDE, 0xFFFF, 0xC4C4);
		SET_ENTRY(priv->entry_array + 17, 0x3333, 0x3333, 0xFFFF);
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_prefs_color_map_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}

CAT_WO_BASE_C(ChaPrefsColorMapWo, cha_prefs_color_map_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaPrefsColorMapWo *instance = CHA_PREFS_COLOR_MAP_WO(self);
	ChaPrefsColorMapWoPrivate *priv = cha_prefs_color_map_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: %s, entry_array=%p]", iname, self, cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->entry_array);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
