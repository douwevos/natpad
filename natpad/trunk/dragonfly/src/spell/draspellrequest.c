/*
   File:    draspellrequest.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 26, 2016
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

#include "draspellrequest.h"
#include "draspellchecker.h"
#include "../drakeywordprinter.h"
#include "../dralinetagwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraSpellRequest"
#include <logging/catlog.h>

struct _DraSpellRequestPrivate {
	ChaDocument *document;
	ChaRevisionWo *a_new_revision;
	CatStringWo *slot_key;
	int last_content_version;
	DraSpellHelper *spell_helper;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraSpellRequest, dra_spell_request, WOR_TYPE_REQUEST,
		G_ADD_PRIVATE(DraSpellRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void dra_spell_request_class_init(DraSpellRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void dra_spell_request_init(DraSpellRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraSpellRequest *instance = DRA_SPELL_REQUEST(object);
//	DraSpellRequestPrivate *priv = dra_spell_request_get_instance_private(instance);
	G_OBJECT_CLASS(dra_spell_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_spell_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraSpellRequest *dra_spell_request_new(DraSpellHelper *spell_helper, ChaDocument *document, ChaRevisionWo *a_new_revision, CatStringWo *slot_key) {
	DraSpellRequest *result = g_object_new(DRA_TYPE_SPELL_REQUEST, NULL);
	cat_ref_anounce(result);
	DraSpellRequestPrivate *priv = dra_spell_request_get_instance_private(result);
	priv->document = cat_ref_ptr(document);
	priv->a_new_revision = cat_ref_ptr(a_new_revision);
	priv->slot_key = cat_ref_ptr(slot_key);
	priv->last_content_version = cha_revision_wo_get_page_list_version(a_new_revision);
	priv->spell_helper = cat_ref_ptr(spell_helper);
	wor_request_construct((WorRequest *) result);
	wor_request_set_time_out((WorRequest *) result, cat_date_current_time()+120);
	return result;
}


static void l_run_request(WorRequest *request) {
	cat_log_debug("running request");

	DraSpellRequest *instance = DRA_SPELL_REQUEST(request);
	DraSpellRequestPrivate *priv = dra_spell_request_get_instance_private(instance);

	ChaRevisionWo *a_most_recent_revision = cha_document_get_current_revision_ref(priv->document);
	int content_version = cha_revision_wo_get_page_list_version(a_most_recent_revision);
	cat_unref_ptr(a_most_recent_revision);
	if (priv->last_content_version != content_version) {
		return;
	}

	int markup_slot_idx = cha_revision_wo_get_slot_index(priv->a_new_revision, (GObject *) priv->slot_key, -1);
	cat_log_debug("Running:markup_slot_idx=%d", markup_slot_idx);
	if (markup_slot_idx<0) {
		return;
	}


	DraKeywordPrinter *line_tag_printer = dra_keyword_printer_new(priv->a_new_revision, priv->slot_key, markup_slot_idx);

	ChaRevisionReader *revision_reader = cha_revision_reader_new(priv->a_new_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);


	DraSpellChecker *spell_checker = dra_spell_checker_new(CAT_IUTF8_SCANNER(revision_reader));

	dra_spell_helper_scan_all(priv->spell_helper, line_tag_printer, spell_checker);

	dra_keyword_printer_flush_line_tags(line_tag_printer);

	cat_unref_ptr(spell_checker);
	cat_unref_ptr(revision_reader);
	cat_unref_ptr(line_tag_printer);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraSpellRequest *instance = DRA_SPELL_REQUEST(self);
	DraSpellRequestPrivate *priv = dra_spell_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
