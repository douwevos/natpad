/*
   File:    xanaugmentrequest.c
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 10, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "xanaugmentrequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "XanAugmentRequest"
#include <logging/catlog.h>

struct _XanAugmentRequestPrivate {
	XanScannerFactory *scanner_factory;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(XanAugmentRequest, xan_augment_request, DRA_TYPE_AUGMENT_REQUEST,
		G_ADD_PRIVATE(XanAugmentRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);

static void xan_augment_request_class_init(XanAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAugmentRequestClass *aug_class = DRA_AUGMENT_REQUEST_CLASS(clazz);
	aug_class->runAugment = l_run_augment;
}

static void xan_augment_request_init(XanAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	XanAugmentRequest *instance = XAN_AUGMENT_REQUEST(object);
//	XanAugmentRequestPrivate *priv = xan_augment_request_get_instance_private(instance);
	G_OBJECT_CLASS(xan_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(xan_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


XanAugmentRequest *xan_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key, XanScannerFactory *scanner_factory) {
	XanAugmentRequest *result = g_object_new(XAN_TYPE_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	XanAugmentRequestPrivate *priv = xan_augment_request_get_instance_private(result);
	dra_augment_request_construct((DraAugmentRequest *) result, document, a_revision, slot_key);
	priv->scanner_factory = cat_ref_ptr(scanner_factory);
	return result;
}


static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {
	XanAugmentRequestPrivate *priv = xan_augment_request_get_instance_private(XAN_AUGMENT_REQUEST(request));
	ChaRevisionReader *revision_reader = cha_revision_reader_new(a_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);


	CatStringWo *slot_key = dra_augment_request_get_slot_key(request);
	int slot_key_idx = cha_revision_wo_get_slot_index(a_revision, (GObject *) slot_key, -1);
//	XanDomNode *dom_node = (XanDomNode *) cha_revision_wo_get_slot_content_ref(a_revision, slot_key_idx, slot_key);




	XanScanner *scanner = xan_scanner_factory_create_scanner(priv->scanner_factory, utf8_scanner);

	gboolean keep_scanning = TRUE;

	while(keep_scanning) {
		GroRunIToken *token = xan_scanner_next(scanner);
		if (token==NULL) {
			break;
		}
		GroRunSymbol *symbol = grorun_itoken_get_symbol(token);
		int sym_id = grorun_symbol_get_id(symbol);

		GroRunLocation *location = grorun_full_token_get_location((GroRunFullToken *) token);
		int left_column, right_column;
		long left_row, right_row;
		grorun_location_get_all(location, &left_column, &left_row, &right_column, &right_row);

		switch(sym_id) {
			case XAN_SYM_END_OF_INPUT : {
				keep_scanning = FALSE;
				break;
			}
			case XAN_SYM_TAG_OPEN :
			case XAN_SYM_TAG_OPEN_TERMINATOR :
			case XAN_SYM_TAG_CLOSE :
			case XAN_SYM_TAG_TERMINATOR_CLOSE :
			case XAN_SYM_ELEMENT_OPEN :
			case XAN_SYM_ELEMENT_CLOSE : {
				dra_keyword_printer_print_fg_color(keyword_printer, left_row, left_column, right_row, right_column, 5);
			} break;

			case XAN_SYM_NAME : {
				dra_keyword_printer_print_fg_color(keyword_printer, left_row, left_column, right_row, right_column, 7);
			} break;

			case XAN_SYM_ID : {
				dra_keyword_printer_print_fg_color(keyword_printer, left_row, left_column, right_row, right_column, 9);
			} break;

			case XAN_SYM_LITERAL : {
				dra_keyword_printer_print_fg_color(keyword_printer, left_row, left_column, right_row, right_column, 10);
			} break;
		}
	}


	cat_unref_ptr(utf8_scanner);
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
