/*
   File:    dradocumentview.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Mar 27, 2015
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

#include "dradocumentview.h"

#include "dralinelayout.h"
#include "drasublinecache.h"
#include "attr/draiattributebuilder.h"
#include "attr/draforeground.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraDocumentView"
#include <logging/catlog.h>

struct _DraDocumentViewPrivate {
	CatStringWo *a_line_info_key;
	ChaOccurrencesResultSet *occurrence_result_set;

};

static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraDocumentView, dra_document_view, CHA_TYPE_DOCUMENT_VIEW,
		G_ADD_PRIVATE(DraDocumentView)
		G_IMPLEMENT_INTERFACE(CHA_TYPE_IDOCUMENT_LISTENER, l_document_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static ChaSubLineCache *l_create_sub_line_cache(ChaSurfacePool *surface_pool, ChaLineLayout *line_layout, int sub_line_idx);
static ChaLineLayout *l_create_line_layout(ChaDocumentView *document_view);
static void l_on_invalidate_line(ChaDocumentView *document_view, int page_index, int page_line_index, ChaLineWo *a_line, ChaLineLayout *line_layout);
static CatStringWo *l_create_rich_text(ChaDocumentView *document_view, ChaPageWo *a_page, ChaLineWo *a_line, int l_index, int r_index);

static void l_invalidate_revision(ChaDocumentView *document_view, ChaRevisionWo *a_rev);

static void dra_document_view_class_init(DraDocumentViewClass *clazz) {
	ChaDocumentViewClass *document_view_class = CHA_DOCUMENT_VIEW_CLASS(clazz);
	document_view_class->createSubLineCache = l_create_sub_line_cache;
	document_view_class->createLineLayout = l_create_line_layout;
	document_view_class->onInvalidateLine = l_on_invalidate_line;
	document_view_class->createRichText = l_create_rich_text;
	document_view_class->invalidateRevision = l_invalidate_revision;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_document_view_init(DraDocumentView *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraDocumentView *instance = DRA_DOCUMENT_VIEW(object);
	DraDocumentViewPrivate *priv = dra_document_view_get_instance_private(instance);
	cat_unref_ptr(priv->a_line_info_key);
	cat_unref_ptr(priv->occurrence_result_set);
	G_OBJECT_CLASS(dra_document_view_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_document_view_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraDocumentView *dra_document_view_new(ChaDocument *document, PangoContext *pango_context, GtkWidget *widget, CatStringWo *a_line_info_key) {
	DraDocumentView *result = g_object_new(DRA_TYPE_DOCUMENT_VIEW, NULL);
	cat_ref_anounce(result);
	DraDocumentViewPrivate *priv = dra_document_view_get_instance_private(result);
	priv->a_line_info_key = cat_ref_ptr(a_line_info_key);
	priv->occurrence_result_set = NULL;
	cha_document_view_construct((ChaDocumentView *) result, document, pango_context, widget);
	return result;
}

void dra_document_view_set_occurrences_rset(DraDocumentView *document_view, ChaOccurrencesResultSet *occurrences_rset) {
	DraDocumentViewPrivate *priv = dra_document_view_get_instance_private(document_view);
	cat_ref_swap(priv->occurrence_result_set, occurrences_rset);
}

static ChaSubLineCache *l_create_sub_line_cache(ChaSurfacePool *surface_pool, ChaLineLayout *line_layout, int sub_line_idx) {
	return (ChaSubLineCache *) dra_sub_line_cache_new(surface_pool, line_layout, sub_line_idx);
}


static ChaLineLayout *l_create_line_layout(ChaDocumentView *document_view) {
	return (ChaLineLayout *) dra_line_layout_new();
}

static void l_invalidate_revision(ChaDocumentView *document_view, ChaRevisionWo *a_rev) {
	DraDocumentViewPrivate *priv = dra_document_view_get_instance_private((DraDocumentView *) document_view);
	if (priv->occurrence_result_set) {
		cha_occurences_result_set_set_revision(priv->occurrence_result_set, a_rev);
	}

	CHA_DOCUMENT_VIEW_CLASS(dra_document_view_parent_class)->invalidateRevision(document_view, a_rev);
}


static void l_on_invalidate_line(ChaDocumentView *document_view, int page_index, int page_line_index,  ChaLineWo *a_line, ChaLineLayout *line_layout) {
	DraDocumentViewPrivate *priv = dra_document_view_get_instance_private((DraDocumentView *) document_view);
	DraLineLayout *dra_line_layout = (DraLineLayout *) line_layout;
	DraLineInfoWo *a_line_info = (DraLineInfoWo *) cha_line_wo_get_slot_content_ref(a_line, -1, (GObject *) priv->a_line_info_key);
	if (a_line_info) {
		dra_line_layout_set_line_info(dra_line_layout, a_line_info);
		cat_unref_ptr(a_line_info);
	}

	CatArrayWo *occurrences = NULL;
	if (priv->occurrence_result_set) {
		occurrences = cha_occurrences_result_set_enlist_for_line(priv->occurrence_result_set, page_index, page_line_index);
	}
	dra_line_layout_set_occurrences(dra_line_layout, occurrences);
//	cat_unref_ptr(occurrences);
}


struct _DraRtfPart {
	int right;
	int foreground;
};

typedef struct _DraRtfPart DraRtfPart;


static void l_get_rtf_part_from_ab_list(DraRtfPart *rtf_part, CatArrayWo *ab_list, int index) {
	rtf_part->right = 0x7FFFFFF;
	rtf_part->foreground = 1;
	if (ab_list) {
		CatIIterator *iter = cat_array_wo_iterator(ab_list);
		while(cat_iiterator_has_next(iter)) {
			DraIAttributeBuilder *builder = (DraIAttributeBuilder *) cat_iiterator_next(iter);
			DraIAttributeBuilderInterface *ab_iface = DRA_IATTRIBUTE_BUILDER_GET_INTERFACE(builder);
			int start = 0;
			int end = 0;
			ab_iface->getBeginAndEnd(builder, &start, &end);
			if (index>=end) {
				continue;
			}
			if (index<start) {
				if (rtf_part->right > start) {
					rtf_part->right = start;
				}
				continue;
			}

			if (rtf_part->right > end) {
				rtf_part->right = end;
			}


			if (DRA_IS_FOREGROUND(builder)) {
				rtf_part->foreground = dra_foreground_get_color_index((DraForeground *) builder);
			}
		}
		cat_unref_ptr(iter);
	}
}

static void l_convert_to_rtf_and_append(CatStringWo *e_out, const char *text, int len) {
	while(len>0) {
		char ch = *text;
		if ((ch=='{') || (ch=='}') || (ch=='\\')) {
			cat_string_wo_append_chars(e_out, "\\");
		}
		cat_string_wo_append_char(e_out, ch);

		text++;
		len--;
	}
}

static CatStringWo *l_create_rich_text(ChaDocumentView *document_view, ChaPageWo *a_page, ChaLineWo *a_line, int left, int right) {
	DraDocumentViewPrivate *priv = dra_document_view_get_instance_private((DraDocumentView *) document_view);
	DraLineInfoWo *a_line_info = (DraLineInfoWo *) cha_line_wo_get_slot_content_ref(a_line, -1, (GObject *) priv->a_line_info_key);

	const CatStringWo *a_text = cha_line_wo_get_text(a_line);
	int len = right-left;

	CatStringWo *e_buf = cat_string_wo_new();
	if (len>0) {


		CatArrayWo *ab_list = NULL;
		if (a_line_info) {
			ab_list = dra_line_info_wo_get_ab_list(a_line_info);
		}

		DraRtfPart rtf_part;

		while(left<right) {
			l_get_rtf_part_from_ab_list(&rtf_part, ab_list, left);
			int mid = rtf_part.right;
			if (mid>right) {
				mid = right;
			}

			int len = mid-left;
			cat_string_wo_append_chars(e_buf, "{\\cf");
			cat_string_wo_append_decimal(e_buf, rtf_part.foreground);
			cat_string_wo_append_chars_len(e_buf, " ",1);
			l_convert_to_rtf_and_append(e_buf, cat_string_wo_getchars(a_text)+left, len);
			cat_string_wo_append_chars_len(e_buf, "}",1);
			left = mid;
		}
	}
	cat_unref_ptr(a_line_info);
	return e_buf;
}


static void l_on_enrichment_slot_notify(ChaIDocumentListener *self, ChaRevisionWo *a_revision, GObject *slot_key, gpointer user_data) {
	cha_document_view_invalidate_lines((ChaDocumentView *) self);
}



static void l_document_listener_iface_init(ChaIDocumentListenerInterface *iface) {
	ChaIDocumentListenerInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->onNewRevision = p_iface->onNewRevision;
	iface->onSlotRegistered = p_iface->onSlotRegistered;
	iface->onEnrichmentSlotNotify = l_on_enrichment_slot_notify;
	iface->onHistoryModified = p_iface->onHistoryModified;
	iface->onModeChanged = p_iface->onModeChanged;
	iface->onNewSavedRevision = p_iface->onNewSavedRevision;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
