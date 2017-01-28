/*
   File:    draaugmentrequest.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 27, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "draaugmentrequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraAugmentRequest"
#include <logging/catlog.h>

struct _DraAugmentRequestPrivate {
	ChaDocument *document;
	ChaRevisionWo *a_revision;
	CatStringWo *slot_key;
	DraSpellHelper *spell_helper;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraAugmentRequest, dra_augment_request, WOR_TYPE_REQUEST,
		G_ADD_PRIVATE(DraAugmentRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void dra_augment_request_class_init(DraAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void dra_augment_request_init(DraAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraAugmentRequest *instance = DRA_AUGMENT_REQUEST(object);
//	DraAugmentRequestPrivate *priv = dra_augment_request_get_instance_private(instance);
	G_OBJECT_CLASS(dra_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void dra_augment_request_construct(DraAugmentRequest *request, ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key) {
	DraAugmentRequestPrivate *priv = dra_augment_request_get_instance_private(request);
	priv->document = cat_ref_ptr(document);
	priv->a_revision = cat_ref_ptr(a_revision);
	priv->slot_key = cat_ref_ptr(slot_key);
	wor_request_construct((WorRequest *) request);
	wor_request_set_time_out((WorRequest *) request, cat_date_current_time()+120);
}

CatStringWo *dra_augment_request_get_slot_key(DraAugmentRequest *request) {
	DraAugmentRequestPrivate *priv = dra_augment_request_get_instance_private(request);
	return priv->slot_key;
}

void dra_augment_request_set_spell_helper(DraAugmentRequest *request, DraSpellHelper *spell_helper) {
	DraAugmentRequestPrivate *priv = dra_augment_request_get_instance_private(request);
	cat_ref_swap(priv->spell_helper, spell_helper);
}

DraSpellHelper *dra_augment_request_get_spell_helper(DraAugmentRequest *request) {
	DraAugmentRequestPrivate *priv = dra_augment_request_get_instance_private(request);
	return priv->spell_helper;
}


static gboolean l_idle_cb(DraAugmentRequest *request) {
	DraAugmentRequestPrivate *priv = dra_augment_request_get_instance_private(request);
	cha_document_post_enrichment_slot_notify(priv->document, priv->a_revision, (GObject *) priv->slot_key, NULL);
	cat_unref_ptr(request);
	return FALSE;
}


static void l_run_request(WorRequest *request) {
	DraAugmentRequestPrivate *priv = dra_augment_request_get_instance_private(DRA_AUGMENT_REQUEST(request));
	ChaRevisionWo *cur_revision = cha_document_get_current_revision_ref(priv->document);
	int cur_pl_version = cha_revision_wo_get_page_list_version(cur_revision);
	int rev_pl_version = cha_revision_wo_get_page_list_version(priv->a_revision);
	if (rev_pl_version<cur_pl_version) {
		return;
	}
	int slot_key_index = cha_revision_wo_get_slot_index(priv->a_revision, (GObject *) priv->slot_key, -1);
	if (slot_key_index<0) {
		return;
	}

	DraKeywordPrinter *keyword_printer = dra_keyword_printer_new(priv->a_revision, priv->slot_key, slot_key_index);
	DraKeywordPrinter *line_tag_printer = dra_keyword_printer_new(priv->a_revision, priv->slot_key, slot_key_index);

	DraAugmentRequestClass *aug_class = DRA_AUGMENT_REQUEST_GET_CLASS(request);
	gboolean do_notify = aug_class->runAugment((DraAugmentRequest *) request, priv->a_revision, keyword_printer, line_tag_printer);


	dra_keyword_printer_flush_line_tags(line_tag_printer);
	dra_keyword_printer_flush(keyword_printer);

    cat_unref_ptr(line_tag_printer);
    cat_unref_ptr(keyword_printer);

    if (do_notify) {
		cat_ref_ptr(request);
		g_idle_add((GSourceFunc) l_idle_cb, request);
    }
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
