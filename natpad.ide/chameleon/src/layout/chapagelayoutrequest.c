/*
   File:    chapagelayoutrequest.c
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

#include "chapagelayoutrequest.h"
#include "chalinelayout.h"
#include "chapagelayout.h"
#include "../document/page/chammappagewo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "ChaPageLayoutRequest"
#include <logging/catlog.h>

struct _ChaPageLayoutRequestPrivate {
	ChaRevisionWo *a_rev_at_construction;
	ChaDocumentView *document_view;
	ChaPageWo *a_page_wo;
	gboolean has_run;
	CatLock *lock;
	ChaPageLayout *page_layout;

//	int page_version;
//	int text_layout_width;
	int page_index;

	ChaPageLayoutContext page_layout_context;

	gboolean did_decrease_counter;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPageLayoutRequest, cha_page_layout_request, WOR_TYPE_REQUEST,
		G_ADD_PRIVATE(ChaPageLayoutRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);


static void cha_page_layout_request_class_init(ChaPageLayoutRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;


	clazz->enlisted_counter = cat_atomic_integer_new();
	cat_ref_ignore(clazz->enlisted_counter);
}

static void cha_page_layout_request_init(ChaPageLayoutRequest *instance) {
}

static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
	ChaPageLayoutRequest *instance = CHA_PAGE_LAYOUT_REQUEST(object);
	ChaPageLayoutRequestPrivate *priv = cha_page_layout_request_get_instance_private(instance);
	if (priv->a_page_wo) {
		cha_page_wo_impoverish(priv->a_page_wo);
		cat_unref_ptr(priv->a_page_wo);
	}
	cat_unref_ptr(priv->page_layout);
	cat_unref_ptr(priv->a_rev_at_construction);
	cat_unref_ptr(priv->document_view);
	cat_unref_ptr(priv->lock);
	if (!priv->did_decrease_counter) {
		cat_atomic_integer_decrement(CHA_PAGE_LAYOUT_REQUEST_GET_CLASS(instance)->enlisted_counter);
	}
	G_OBJECT_CLASS(cha_page_layout_request_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_page_layout_request_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
}


ChaPageLayoutRequest *cha_page_layout_request_new(ChaDocumentView *document_view, ChaPageWo *a_page_wo, ChaPageLayout *page_layout, int text_layout_width, short font_version, int page_index) {
	ChaPageLayoutRequest *result = g_object_new(CHA_TYPE_PAGE_LAYOUT_REQUEST, NULL);
	cat_ref_anounce(result);
	cat_atomic_integer_increment(CHA_PAGE_LAYOUT_REQUEST_GET_CLASS(result)->enlisted_counter);

	ChaPageLayoutRequestPrivate *priv = cha_page_layout_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	priv->document_view = cat_ref_ptr(document_view);
	priv->a_page_wo = cat_ref_ptr(a_page_wo);
	ChaDocument *document = cha_document_view_get_document(document_view);
	priv->a_rev_at_construction = cha_document_get_current_revision_ref(document);
	ChaEnrichmentDataMapWo *enrichment_map = cha_document_get_enrichment_data_map(document);
	cha_page_wo_enrich(a_page_wo, enrichment_map);

	priv->lock = cat_lock_new();
	priv->has_run = FALSE;
	priv->page_layout_context.page_version = cat_wo_get_version((CatWo *) a_page_wo);
	priv->page_layout_context.text_layout_width = text_layout_width;
	priv->page_layout_context.font_version = font_version;
	priv->page_index = page_index;
	priv->page_layout = cat_ref_ptr(page_layout);
	priv->did_decrease_counter = FALSE;

	return result;
}

static gboolean l_idle_apply_layout_updates(gpointer user_data) {
	ChaPageLayoutRequest *instance = CHA_PAGE_LAYOUT_REQUEST(user_data);
	ChaPageLayoutRequestPrivate *priv = cha_page_layout_request_get_instance_private(instance);


	/* TODO need to do testing if we really should apply the new content.
	 *      Since this is another thread the thread running l_run_request.
	 *      New version check should be done here
	 */
//
	ChaPageLayoutRequestClass *layout_class = CHA_PAGE_LAYOUT_REQUEST_GET_CLASS(instance);
	int ec = cat_atomic_integer_decrement(layout_class->enlisted_counter);
	priv->did_decrease_counter = TRUE;
	gboolean refresh = FALSE;
	if (ec>2) {
		int64_t now = cat_date_current_time();
		if ((now-layout_class->last_update)<5000) {
			refresh = TRUE;
			layout_class->last_update = now;
		}
	} else {
		refresh = TRUE;
	}


	gboolean was_updated = cha_document_view_page_set_layout(priv->document_view, priv->a_page_wo, priv->page_layout, &(priv->page_layout_context), refresh);

//	gboolean was_updated = cha_page_layout_apply_update_result(priv->page_layout, &(priv->page_layout_context));
	cat_log_detail("refresh=%d, was_updated=%d, page_index=%d", refresh, was_updated, priv->page_index);

	if (was_updated && refresh) {
		cha_document_view_invalidate_lines(priv->document_view);
	}

	cat_lock_lock(priv->lock);
	priv->has_run = TRUE;
	cat_lock_notify(priv->lock);
	cat_lock_unlock(priv->lock);

//	cat_ref_entry_report();

	cat_log_debug("page:%d done, ec=%d  %o, page_height=%d", priv->page_index, ec, priv->a_page_wo, priv->page_layout_context.requisition.height);

	cat_unref_ptr(instance);
	return FALSE;
}



static void l_run_request(WorRequest *request) {
	ChaPageLayoutRequest *instance = CHA_PAGE_LAYOUT_REQUEST(request);
	ChaPageLayoutRequestPrivate *priv = cha_page_layout_request_get_instance_private(instance);


//	int version = cat_wo_get_version(CAT_WO(cur_rev));
//	int page_version = cat_wo_get_version((CatWo *) priv->a_page_wo);
//	if (page_version!=version) {
//		cat_log_debug("revision-version not equal to page-version");
//		cat_unref(cur_rev);
//		return;
//	}
	if (!cha_page_layout_version_is_updating(priv->page_layout, priv->page_layout_context.page_version, priv->page_layout_context.text_layout_width, priv->page_layout_context.font_version/*, settings_version*/)) {
		cat_log_debug("page was up-to-date: page=%o", priv->a_page_wo);
		return;
	}



	gboolean did_layout = cha_document_view_layout_page(priv->document_view, priv->a_page_wo, &(priv->page_layout_context), priv->page_layout, TRUE);

	if (did_layout) {
		cat_lock_lock(priv->lock);
		g_idle_add((GSourceFunc) l_idle_apply_layout_updates, cat_ref_ptr(instance));
		int cnt = 10;
		uint64_t ts = 200;
		while(cnt-->0) {
			cat_lock_wait_timed(priv->lock, ts);
			if (priv->has_run) {
				break;
			}
			ts = 2000;
		}
//		cat_lock_wait_timed(priv->lock, 200);
		cat_lock_unlock(priv->lock);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaPageLayoutRequest *instance = CHA_PAGE_LAYOUT_REQUEST(self);
	ChaPageLayoutRequestPrivate *priv = cha_page_layout_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p rev=%p, priv->page_index=%d]", iname, self, priv->a_rev_at_construction, priv->page_index);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
