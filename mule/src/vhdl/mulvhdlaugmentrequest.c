/*
   File:    mulvhdlaugmentrequest.c
   Project: mule
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

#include "mulvhdlaugmentrequest.h"
#include "parser/mulvhdlplainparser.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulVhdlAugmentRequest"
#include <logging/catlog.h>

struct _MulVhdlAugmentRequestPrivate {
	GroRunModel *model;
	GroRunITokenFactory *token_factory;
	MulVhdlScannerFactory *scanner_factory;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulVhdlAugmentRequest, mul_vhdl_augment_request, DRA_TYPE_AUGMENT_REQUEST,
		G_ADD_PRIVATE(MulVhdlAugmentRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);

static void mul_vhdl_augment_request_class_init(MulVhdlAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAugmentRequestClass *aug_class = DRA_AUGMENT_REQUEST_CLASS(clazz);
	aug_class->runAugment = l_run_augment;
}

static void mul_vhdl_augment_request_init(MulVhdlAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulVhdlAugmentRequest *instance = MUL_VHDL_AUGMENT_REQUEST(object);
	MulVhdlAugmentRequestPrivate *priv = mul_vhdl_augment_request_get_instance_private(instance);
	cat_unref_ptr(priv->model);
	cat_unref_ptr(priv->scanner_factory);
	cat_unref_ptr(priv->token_factory);
	G_OBJECT_CLASS(mul_vhdl_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_vhdl_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MulVhdlAugmentRequest *mul_vhdl_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key,
		GroRunModel *model, GroRunITokenFactory *token_factory, MulVhdlScannerFactory *scanner_factory) {
	MulVhdlAugmentRequest *result = g_object_new(MUL_TYPE_VHDL_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	MulVhdlAugmentRequestPrivate *priv = mul_vhdl_augment_request_get_instance_private(result);
	priv->model = cat_ref_ptr(model);
	priv->scanner_factory = cat_ref_ptr(scanner_factory);
	priv->token_factory = cat_ref_ptr(token_factory);
	dra_augment_request_construct((DraAugmentRequest *) result, document, a_revision, slot_key);
	return result;
}

static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {
	MulVhdlAugmentRequestPrivate *priv = mul_vhdl_augment_request_get_instance_private(MUL_VHDL_AUGMENT_REQUEST(request));
	ChaRevisionReader *revision_reader = cha_revision_reader_new(a_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);

	MulVhdlScanner *scanner = mul_vhdl_scanner_factory_create_scanner(priv->scanner_factory, utf8_scanner);
	MulVhdlPlainParser *plain_parser = mul_vhdl_plain_parser_new(scanner, priv->token_factory, priv->model, keyword_printer, line_tag_printer);
	mul_vhdl_plain_parser_run(plain_parser);

	cat_unref_ptr(revision_reader);
	cat_unref_ptr(plain_parser);
	cat_unref_ptr(scanner);
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
