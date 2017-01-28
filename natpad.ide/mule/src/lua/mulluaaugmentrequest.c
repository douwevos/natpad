/*
   File:    mulluaaugmentrequest.c
   Project: mule
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

#include "mulluaaugmentrequest.h"
#include "mulluascanner.h"
#include "parser/mulluaplainparser.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulLuaAugmentRequest"
#include <logging/catlog.h>

struct _MulLuaAugmentRequestPrivate {
	GroRunModel *model;
	GroRunITokenFactory *token_factory;
	MulLuaScannerFactory *scanner_factory;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulLuaAugmentRequest, mul_lua_augment_request, DRA_TYPE_AUGMENT_REQUEST,
		G_ADD_PRIVATE(MulLuaAugmentRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);

static void mul_lua_augment_request_class_init(MulLuaAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAugmentRequestClass *aug_class = DRA_AUGMENT_REQUEST_CLASS(clazz);
	aug_class->runAugment = l_run_augment;
}

static void mul_lua_augment_request_init(MulLuaAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulLuaAugmentRequest *instance = MUL_LUA_AUGMENT_REQUEST(object);
	MulLuaAugmentRequestPrivate *priv = mul_lua_augment_request_get_instance_private(instance);
	cat_unref_ptr(priv->model);
	cat_unref_ptr(priv->scanner_factory);
	cat_unref_ptr(priv->token_factory);
	G_OBJECT_CLASS(mul_lua_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_lua_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MulLuaAugmentRequest *mul_lua_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key,
		GroRunModel *model, GroRunITokenFactory *token_factory, MulLuaScannerFactory *scanner_factory) {
	MulLuaAugmentRequest *result = g_object_new(MUL_TYPE_LUA_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	MulLuaAugmentRequestPrivate *priv = mul_lua_augment_request_get_instance_private(result);
	priv->model = cat_ref_ptr(model);
	priv->scanner_factory = cat_ref_ptr(scanner_factory);
	priv->token_factory = cat_ref_ptr(token_factory);
	dra_augment_request_construct((DraAugmentRequest *) result, document, a_revision, slot_key);
	return result;
}

static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {
	MulLuaAugmentRequestPrivate *priv = mul_lua_augment_request_get_instance_private(MUL_LUA_AUGMENT_REQUEST(request));

	ChaRevisionReader *revision_reader = cha_revision_reader_new(a_revision, NULL, NULL);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);
	CatIUtf8Scanner *utf8_scanner = CAT_IUTF8_SCANNER(revision_reader);

	MulLuaScanner *scanner = mul_lua_scanner_factory_create_scanner(priv->scanner_factory, (CatIUtf8Scanner *) utf8_scanner);

	MulLuaPlainParser *plain_parser = mul_lua_plain_parser_new(scanner, priv->token_factory, priv->model, keyword_printer, line_tag_printer);

	mul_lua_plain_parser_run(plain_parser);

	dra_keyword_printer_flush_line_tags(line_tag_printer);
	dra_keyword_printer_reset(line_tag_printer);


	DraSpellHelper *spell_helper = dra_augment_request_get_spell_helper(request);
	CatArrayWo *token_cash = mul_lua_plain_parser_get_token_cash(plain_parser);
	CatIIterator *iter = cat_array_wo_iterator(token_cash);
	while(cat_iiterator_has_next(iter)) {
		GroRunIToken *token = (GroRunIToken *) cat_iiterator_next(iter);
		cat_log_debug("token=%O", token);
		int sym_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
		if (sym_index==MUL_LUA_SYM_EOL_COMMENT) {
			GObject *val = grorun_itoken_get_value(token);
			if (CAT_IS_STRING_WO(val)) {

				GroRunLocation *location = grorun_itoken_get_location(token);
				int left_column;
				long left_row;
				grorun_location_get_begin(location, &left_column, &left_row);

				dra_spell_helper_scan(spell_helper, line_tag_printer, (CatStringWo *) val, left_column, left_row);
			}
		}
	}
	cat_unref_ptr(iter);




	cat_unref_ptr(utf8_scanner);
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
