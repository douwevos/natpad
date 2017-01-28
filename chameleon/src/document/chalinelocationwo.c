/*
   File:    chalinelocationwo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 22, 2015
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

#include "chalinelocationwo.h"
#include "charevisionwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaLineLocationWo"
#include <logging/catlog.h>

struct _ChaLineLocationWoPrivate {
	int page_index;
	int line_index;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaLineLocationWo, cha_line_location_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaLineLocationWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void cha_line_location_wo_class_init(ChaLineLocationWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_line_location_wo_init(ChaLineLocationWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cha_line_location_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_line_location_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaLineLocationWo *cha_line_location_wo_new() {
	ChaLineLocationWo *result = g_object_new(CHA_TYPE_LINE_LOCATION_WO, NULL);
	cat_ref_anounce(result);
	ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->page_index = 0;
	priv->line_index = 0;
	return result;
}

ChaLineLocationWo *cha_line_location_wo_new_with(int page_index, int page_line_index) {
	ChaLineLocationWo *result = g_object_new(CHA_TYPE_LINE_LOCATION_WO, NULL);
	cat_ref_anounce(result);
	ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->page_index = page_index;
	priv->line_index = page_line_index;
	return result;
}


int cha_line_location_wo_get_page_index(const ChaLineLocationWo *line_location) {
	const ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private((ChaLineLocationWo *) line_location);
	return priv->page_index;
}

int cha_line_location_wo_get_page_line_index(const ChaLineLocationWo *line_location) {
	const ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private((ChaLineLocationWo *) line_location);
	return priv->line_index;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_line_location)) { \
			cat_log_error("Object is read only:%o", e_line_location); \
			return rval; \
		} \

gboolean cha_line_location_wo_bind_to_revision(ChaLineLocationWo *e_line_location, struct _ChaRevisionWo *revision) {
	ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private(e_line_location);
	CHECK_IF_WRITABLE(FALSE);
	gboolean result = TRUE;
	int tline_idx = priv->line_index;
	int tpage_idx = priv->page_index;
	while(result) {
		if (tline_idx<0) {
			tpage_idx--;
			ChaPageWo *page = cha_revision_wo_page_at(revision, tpage_idx);
			if (page) {
				int line_count = cha_page_wo_line_count(page);
				tline_idx += line_count;
				if (tline_idx>=0) {
					break;
				}
			} else {
				result = FALSE;
			}
		} else {
			ChaPageWo *page = cha_revision_wo_page_at(revision, tpage_idx);
			if (page) {
				int line_count = cha_page_wo_line_count(page);
				if (tline_idx<line_count) {
					break;
				}
				tpage_idx++;
				tline_idx-=line_count;
			} else {
				result = FALSE;
			}
		}
	}
	if (result) {
		priv->line_index = tline_idx;
		priv->page_index = tpage_idx;
	}
	return result;
}



void cha_line_location_wo_set_page_index(ChaLineLocationWo *e_line_location, int new_index) {
	ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private(e_line_location);
	CHECK_IF_WRITABLE()
	priv->page_index = new_index;
}

void cha_line_location_wo_set_page_line_index(ChaLineLocationWo *e_line_location, int new_index) {
	ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private(e_line_location);
	CHECK_IF_WRITABLE()
	priv->line_index = new_index;
}

void cha_line_location_wo_add_to_page_line_index(ChaLineLocationWo *e_line_location, int to_add) {
	ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private(e_line_location);
	CHECK_IF_WRITABLE()
	priv->line_index += to_add;
}


ChaLineLocationWo *cha_line_location_wo_create_editable(ChaLineLocationWo *instance) {
	return (ChaLineLocationWo *) cat_wo_create_editable((CatWo *) instance);
}

ChaLineLocationWo *cha_line_location_wo_anchor(ChaLineLocationWo *instance, int version) {
	return (ChaLineLocationWo *) cat_wo_anchor((CatWo *) instance, version);
}

ChaLineLocationWo *cha_line_location_wo_get_original(ChaLineLocationWo *instance) {
	return (ChaLineLocationWo *) cat_wo_get_original((CatWo *) instance);
}

gboolean cha_line_location_wo_is_anchored(const ChaLineLocationWo *instance) {
	return cat_wo_is_anchored((CatWo *) instance);
}

gboolean cha_line_location_wo_equal(const ChaLineLocationWo *instance_a, const ChaLineLocationWo *instance_b) {
	return cat_wo_equal((const CatWo *) instance_a, (const CatWo *) instance_b);
}

int cha_line_location_wo_compare(const ChaLineLocationWo *instance_a, const ChaLineLocationWo *instance_b) {
	if (instance_a==instance_b) {
		return 0;
	} else if (instance_a==NULL || instance_b==NULL) {
		return 0;
	}
	const ChaLineLocationWoPrivate *priv_a = cha_line_location_wo_get_instance_private((ChaLineLocationWo *) instance_a);
	const ChaLineLocationWoPrivate *priv_b = cha_line_location_wo_get_instance_private((ChaLineLocationWo *) instance_b);
	if (priv_a->page_index<priv_b->page_index) {
		return -1;
	} else if (priv_a->page_index>priv_b->page_index) {
		return 1;
	} else if (priv_a->line_index<priv_b->line_index) {
		return -1;
	} else if (priv_a->line_index>priv_b->line_index) {
		return 1;
	}
	return 0;
}


ChaLineLocationWo *cha_line_location_wo_clone(const ChaLineLocationWo *source, CatCloneDepth clone_depth) {
	if (source == NULL) {
		return NULL;
	} else if (clone_depth==CAT_CLONE_DEPTH_NONE && cha_line_location_wo_is_anchored(source)) {
		return cat_ref_ptr((ChaLineLocationWo *) source);
	}
	const ChaLineLocationWoPrivate *spriv = cha_line_location_wo_get_instance_private((ChaLineLocationWo *) source);
	ChaLineLocationWo *result = cha_line_location_wo_new();
	ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private(result);
	if (clone_depth==CAT_CLONE_DEPTH_MAIN) {
		clone_depth = CAT_CLONE_DEPTH_NONE;
	}
	priv->line_index = spriv->line_index;
	priv->page_index = spriv->page_index;
	return result;
}




static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_LINE_LOCATION_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private((ChaLineLocationWo *) e_uninitialized);

	if (original) {
		ChaLineLocationWoPrivate *rpriv = cha_line_location_wo_get_instance_private((ChaLineLocationWo *) original);
		priv->page_index = rpriv->page_index;
		priv->line_index = rpriv->line_index;
	} else {
		priv->page_index = 0;
		priv->line_index = 0;
	}
	return CAT_WO_CLASS(cha_line_location_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static void l_anchor_children(CatWo *wo, int version) {
}

static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaLineLocationWoPrivate *priv_a = cha_line_location_wo_get_instance_private((ChaLineLocationWo *) wo_a);
	const ChaLineLocationWoPrivate *priv_b = cha_line_location_wo_get_instance_private((ChaLineLocationWo *) wo_b);
	return (priv_a->page_index == priv_b->page_index)
			&& (priv_a->line_index == priv_b->line_index);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_LINE_LOCATION_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private(CHA_LINE_LOCATION_WO(e_uninitialized));
	if (wo_source) {
		ChaLineLocationWoPrivate *priv_src = cha_line_location_wo_get_instance_private(CHA_LINE_LOCATION_WO(wo_source));
		priv->line_index = priv_src->line_index;
		priv->page_index = priv_src->page_index;
	} else {
		priv->line_index = 0;
		priv->page_index = 0;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_line_location_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaLineLocationWo *instance = CHA_LINE_LOCATION_WO(self);
	ChaLineLocationWoPrivate *priv = cha_line_location_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %s, page-idx=%d, page-line-idx=%d]", iname, self, cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->page_index, priv->line_index);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
