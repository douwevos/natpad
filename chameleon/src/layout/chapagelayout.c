/*
   File:    chapagelayout.c
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

#include "chapagelayout.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaPageLayout"
#include <logging/catlog.h>

struct _ChaPageLayoutPrivate {
	ChaPageLayoutContext ctx;
	CatLock *lock;

	int64_t request_ts;
	int request_page_version;
	int request_text_layout_width;
	short request_font_version;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPageLayout, cha_page_layout, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaPageLayout)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_page_layout_class_init(ChaPageLayoutClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_page_layout_init(ChaPageLayout *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaPageLayout *instance = CHA_PAGE_LAYOUT(object);
	ChaPageLayoutPrivate *priv = cha_page_layout_get_instance_private(instance);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(cha_page_layout_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_page_layout_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaPageLayout *cha_page_layout_new() {
	ChaPageLayout *result = g_object_new(CHA_TYPE_PAGE_LAYOUT, NULL);
	cat_ref_anounce(result);
	ChaPageLayoutPrivate *priv = cha_page_layout_get_instance_private(result);
	priv->ctx.page_version = -1;
	priv->ctx.font_version = -1;
	priv->ctx.settings_version = -1;
	priv->ctx.requisition.width = 0;
	priv->ctx.requisition.height = -1;
	priv->ctx.text_layout_width = -1;
	priv->request_font_version = -1;
	priv->request_page_version = -1;
	priv->lock = cat_lock_new();
	return result;
}

gboolean cha_page_layout_versions_start_update(ChaPageLayout *page_view, int page_version, int text_layout_width, short font_version) {
	ChaPageLayoutPrivate *priv = cha_page_layout_get_instance_private(page_view);
	cat_lock_lock(priv->lock);
	gboolean result = TRUE;
	if ((priv->ctx.page_version == page_version && priv->ctx.text_layout_width == text_layout_width && priv->ctx.font_version == font_version) ||
			(priv->request_page_version == page_version && priv->request_text_layout_width == text_layout_width && priv->request_font_version == font_version)) {
		result = FALSE;
	}
	cat_log_detail("result=%d, page_layout=%o", result, page_view);
	priv->request_page_version = page_version;
	priv->request_text_layout_width = text_layout_width;
	priv->request_font_version = font_version;
	cat_lock_unlock(priv->lock);
	return result;
}

int cha_page_layout_get_page_version(ChaPageLayout *page_view) {
	ChaPageLayoutPrivate *priv = cha_page_layout_get_instance_private(page_view);
	return priv->ctx.page_version;
}

gboolean cha_page_layout_version_is_updating(ChaPageLayout *page_view, int page_version, int text_layout_width, short font_version/*, int settings_version*/) {
	ChaPageLayoutPrivate *priv = cha_page_layout_get_instance_private(page_view);
	cat_lock_lock(priv->lock);
	gboolean result = priv->request_page_version == page_version && priv->request_text_layout_width==text_layout_width && priv->request_font_version==font_version;
	cat_lock_unlock(priv->lock);
	return result;
}


int cha_page_layout_get_height(ChaPageLayout *page_view) {
	ChaPageLayoutPrivate *priv = cha_page_layout_get_instance_private(page_view);
	return priv->ctx.requisition.height;
}

int cha_page_layout_get_width(ChaPageLayout *page_view) {
	ChaPageLayoutPrivate *priv = cha_page_layout_get_instance_private(page_view);
	return priv->ctx.requisition.width;
}


gboolean cha_page_layout_apply_update_result(ChaPageLayout *page_layout, ChaPageLayoutContext *context) {
	ChaPageLayoutPrivate *priv = cha_page_layout_get_instance_private((ChaPageLayout *) page_layout);
	gboolean result = FALSE;
	cat_lock_lock(priv->lock);
	cat_log_debug("page_layout=%p, req[pv/lw/fo]=%d/%d/%d, cur[pv/lw/fo]=%d/%d/%d", page_layout, priv->request_page_version, priv->request_text_layout_width, priv->request_font_version, context->page_version, context->text_layout_width, context->font_version);
	if ((priv->request_page_version==context->page_version) && (priv->request_text_layout_width==context->text_layout_width) && (priv->request_font_version==context->font_version)) {
		result = TRUE;
		priv->ctx = *context;
	}
	cat_lock_unlock(priv->lock);
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaPageLayout *instance = CHA_PAGE_LAYOUT(self);
	ChaPageLayoutPrivate *priv = cha_page_layout_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: req[pv/lw/fo]=%d/%d/%d, cur[pv/lw/fo]=%d/%d/%d, height=%d]", iname, self,
				priv->request_page_version, priv->request_text_layout_width, priv->request_font_version,
				priv->ctx.page_version, priv->ctx.text_layout_width, priv->ctx.font_version,
				priv->ctx.requisition.height);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
