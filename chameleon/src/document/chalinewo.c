/*
   File:    chalinewo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 25, 2015
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

#include "chalinewo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaLineWo"
#include <logging/catlog.h>

struct _ChaLineWoPrivate {
	int enriched_count;
	ChaEnrichmentData *enrichment_data;
	CatStringWo *text;
	ChaLineEnd line_end;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaLineWo, cha_line_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaLineWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void cha_line_wo_class_init(ChaLineWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_line_wo_init(ChaLineWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaLineWo *instance = CHA_LINE_WO(object);
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(instance);
	cat_unref_ptr(priv->enrichment_data);
	cat_unref_ptr(priv->text);
	G_OBJECT_CLASS(cha_line_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_line_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaLineWo *cha_line_wo_new() {
	ChaLineWo *result = g_object_new(CHA_TYPE_LINE_WO, NULL);
	cat_ref_anounce(result);
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->enriched_count = 0;
	priv->text = cat_string_wo_new_with("");
	priv->line_end = CHA_LINE_END_NONE;
	priv->enrichment_data = NULL;
	return result;
}

ChaLineWo *cha_line_wo_new_with(CatStringWo *c_text, ChaLineEnd line_end) {
	ChaLineWo *result = g_object_new(CHA_TYPE_LINE_WO, NULL);
	cat_ref_anounce(result);
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->enriched_count = 0;
	priv->text = c_text;
	priv->line_end = line_end;
	priv->enrichment_data = NULL;
	return result;
}


ChaLineWo *cha_line_wo_new_anchored(CatStringWo *c_text, ChaLineEnd line_end) {
	ChaLineWo *result = g_object_new(CHA_TYPE_LINE_WO, NULL);
	cat_ref_anounce(result);
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, FALSE);
	priv->enriched_count = 0;
	priv->text = cat_string_wo_anchor(c_text, 0);
	priv->line_end = line_end;
	priv->enrichment_data = NULL;
	return result;
}



void cha_line_wo_enrich(ChaLineWo *line, ChaEnrichmentDataMapWo *a_map, CatLock *lock) {
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(line);
	if (priv->enriched_count==0) {
		if (priv->enrichment_data==NULL) {
			priv->enrichment_data = cha_enrichment_data_new_lock(a_map, lock);
		}
	}
	priv->enriched_count++;
}

void cha_line_wo_impoverish(ChaLineWo *line) {
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(line);
	if (priv->enriched_count==1) {
		cat_unref_ptr(priv->enrichment_data);
	}
	priv->enriched_count--;

}

void cha_line_wo_enrichment_remap(ChaLineWo *line, ChaEnrichmentDataMapWo *a_old_map, ChaEnrichmentDataMapWo *a_new_map, int add_idx, int rem_idx) {
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(line);
	ChaEnrichmentData *enrichment_data = priv->enrichment_data;
	if (enrichment_data) {
		cha_enrichment_data_remap(enrichment_data, a_old_map, a_new_map, add_idx, rem_idx);
	}
}

GObject *cha_line_wo_get_slot_content_ref(ChaLineWo *line, int slot_index, GObject *slot_key) {
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(line);
	GObject *result = NULL;
	ChaEnrichmentData *enrichment_data = priv->enrichment_data;
	if (enrichment_data) {
		result = cha_enrichment_data_get_slot_content_ref(enrichment_data, slot_index, slot_key);
	}
	return result;
}

void cha_line_wo_set_slot_content(ChaLineWo *line, int slot_index, GObject *slot_key, GObject *content) {
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(line);
	ChaEnrichmentData *enrichment_data = priv->enrichment_data;
	if (enrichment_data) {
		cha_enrichment_data_set_slot_content(enrichment_data, slot_index, slot_key, content);
	}
}


#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_line)) { \
			cat_log_error("Object is read only:%o", e_line); \
			return rval; \
		} \


CatStringWo *cha_line_wo_get_text(ChaLineWo *line) {
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(line);
	return priv->text;
}

void cha_line_wo_set_text(ChaLineWo *e_line, CatStringWo *c_text) {
	CHECK_IF_WRITABLE()
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(e_line);
	cat_unref_ptr(priv->text);
	priv->text = c_text;
}

ChaLineEnd cha_line_wo_get_line_end(ChaLineWo *line) {
	const ChaLineWoPrivate *priv = cha_line_wo_get_instance_private((ChaLineWo *) line);
	return priv->line_end;
}

void cha_line_wo_set_line_end(ChaLineWo *e_line, ChaLineEnd line_end) {
	CHECK_IF_WRITABLE()
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(e_line);
	priv->line_end = line_end;
}



int cha_line_wo_byte_count(const ChaLineWo *line) {
	const ChaLineWoPrivate *priv = cha_line_wo_get_instance_private((ChaLineWo *) line);
	return cat_string_wo_length(priv->text);
}

int cha_line_wo_byte_count_real(const ChaLineWo *line) {
	const ChaLineWoPrivate *priv = cha_line_wo_get_instance_private((ChaLineWo *) line);
	int length = cat_string_wo_length(priv->text);
	switch(priv->line_end) {
	case CHA_LINE_END_CR :
	case CHA_LINE_END_LF :
		length++;
		break;
	case CHA_LINE_END_CRLF :
	case CHA_LINE_END_LFCR :
		length+=2;
		break;
	case CHA_LINE_END_NONE :
		break;
	}
	return length;
}


ChaLineWo *cha_line_wo_create_editable(ChaLineWo *instance) {
	return (ChaLineWo *) cat_wo_create_editable((CatWo *) instance);
}

ChaLineWo *cha_line_wo_anchor(ChaLineWo *instance, int version) {
	return (ChaLineWo *) cat_wo_anchor((CatWo *) instance, version);
}

ChaLineWo *cha_line_wo_get_original(ChaLineWo *instance) {
	return (ChaLineWo *) cat_wo_get_original((CatWo *) instance);
}

gboolean cha_line_wo_is_anchored(const ChaLineWo *instance) {
	return cat_wo_is_anchored((CatWo *) instance);
}

gboolean cha_line_wo_equal(const ChaLineWo *instance_a, const ChaLineWo *instance_b) {
	if (instance_a==instance_b) {
		return TRUE;
	} else if (instance_a==NULL || instance_b==NULL) {
		return FALSE;
	}
	return CAT_WO_GET_CLASS(instance_a)->equal((const CatWo *) instance_a, (const CatWo *) instance_b);
}

ChaLineWo *cha_line_wo_clone(const ChaLineWo *source, CatCloneDepth clone_depth) {
	if (source==NULL) {
		return NULL;
	}
	ChaLineWo *result = (ChaLineWo *) cat_wo_clone((CatWo *) source, clone_depth);
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(result);
	priv->enrichment_data = NULL;
	return result;
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_LINE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private((ChaLineWo *) e_uninitialized);

	if (original) {
		ChaLineWoPrivate *rpriv = cha_line_wo_get_instance_private((ChaLineWo *) original);
		priv->enriched_count = 0;
		priv->text = cat_string_wo_create_editable(rpriv->text);
		priv->line_end = rpriv->line_end;
		priv->enrichment_data = cat_ref_ptr(rpriv->enrichment_data);
	} else {
		priv->enriched_count = 0;
		priv->text = NULL;
		priv->line_end = CHA_LINE_END_NONE;
		priv->enrichment_data = NULL;
	}
	return CAT_WO_CLASS(cha_line_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}



static void l_anchor_children(CatWo *wo, int version) {
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private((ChaLineWo *) wo);
	priv->text = cat_string_wo_anchor(priv->text, version);
}



static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	}
	if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaLineWoPrivate *priv_a = cha_line_wo_get_instance_private((ChaLineWo *) wo_a);
	const ChaLineWoPrivate *priv_b = cha_line_wo_get_instance_private((ChaLineWo *) wo_b);
	return cat_string_wo_equal(priv_a->text, priv_b->text) && (priv_a->line_end==priv_b->line_end);
}


static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_LINE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(CHA_LINE_WO(e_uninitialized));
	if (wo_source) {
		ChaLineWoPrivate *priv_src = cha_line_wo_get_instance_private(CHA_LINE_WO(wo_source));
		priv->enriched_count = 0;
		priv->text = cat_string_wo_clone(priv_src->text, CAT_CLONE_DEPTH_MAIN);
		priv->line_end = priv_src->line_end;
		priv->enrichment_data = NULL;
	} else {
		priv->enriched_count = 0;
		priv->text = NULL;
		priv->line_end = CHA_LINE_END_NONE;
		priv->enrichment_data = NULL;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_line_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaLineWo *instance = CHA_LINE_WO(self);
	ChaLineWoPrivate *priv = cha_line_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s enr-cnt=%d, enr-data=%o, line_end=%d, text=%o]", iname, self, cat_wo_get_version((CatWo *) self), cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->enriched_count, priv->enrichment_data, priv->line_end, priv->text);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/



void cha_line_end_to_string(ChaLineEnd line_end, CatStringWo *e_string) {
	char let[] = { 13, 10, 13 };
	switch(line_end) {
		case CHA_LINE_END_CR :
			cat_string_wo_append_chars_len(e_string, let, 1);
			break;
		case CHA_LINE_END_CRLF :
			cat_string_wo_append_chars_len(e_string, let, 2);
			break;
		case CHA_LINE_END_LF :
			cat_string_wo_append_chars_len(e_string, let+1, 1);
			break;
		case CHA_LINE_END_LFCR :
			cat_string_wo_append_chars_len(e_string, let+1, 2);
			break;
		case CHA_LINE_END_MIXED :
		case CHA_LINE_END_NONE :
			break;
	}

}
