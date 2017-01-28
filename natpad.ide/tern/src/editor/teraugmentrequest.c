/*
   File:    teraugmentrequest.c
   Project: tern
   Author:  Douwe Vos
   Date:    Dec 28, 2016
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

#include "teraugmentrequest.h"
#include "../grammar/scanner/terscanner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerAugmentRequest"
#include <logging/catlog.h>

struct _TerAugmentRequestPrivate {
	TerGrammarMap *grammar_map;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerAugmentRequest, ter_augment_request, DRA_TYPE_AUGMENT_REQUEST,
		G_ADD_PRIVATE(TerAugmentRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);

static void ter_augment_request_class_init(TerAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAugmentRequestClass *aug_class = DRA_AUGMENT_REQUEST_CLASS(clazz);
	aug_class->runAugment = l_run_augment;
}

static void ter_augment_request_init(TerAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerAugmentRequest *instance = TER_AUGMENT_REQUEST(object);
	TerAugmentRequestPrivate *priv = ter_augment_request_get_instance_private(instance);
	cat_unref_ptr(priv->grammar_map);
	G_OBJECT_CLASS(ter_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TerAugmentRequest *ter_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *a_slot_key, TerGrammarMap *grammar_map) {
	TerAugmentRequest *result = g_object_new(TER_TYPE_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	TerAugmentRequestPrivate *priv = ter_augment_request_get_instance_private(result);
	priv->grammar_map = cat_ref_ptr(grammar_map);
	dra_augment_request_construct((DraAugmentRequest *) result, document, a_revision, a_slot_key);
	return result;
}

static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {
	TerAugmentRequestPrivate *priv = ter_augment_request_get_instance_private(TER_AUGMENT_REQUEST(request));

	CatStringWo *a_slot_key = dra_augment_request_get_slot_key(request);

	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(cha_revision_reader_new(a_revision, NULL, NULL));
	TerSyntax *syntax = ter_grammar_map_get_syntax(priv->grammar_map, a_slot_key);
	TerScanner *tscanner = ter_scanner_new(syntax, utf8_scanner, keyword_printer);
	ter_scanner_run(tscanner);

	cat_unref_ptr(utf8_scanner);
    cat_unref_ptr(keyword_printer);
	return TRUE;
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
