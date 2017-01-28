///*
//   File:    cusparsercontextext.c
//   Project: cuscus
//   Author:  Douwe Vos
//   Date:    May 16, 2016
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2016 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#include "cusparsercontextext.h"
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "CusParserContextExt"
//#include <logging/catlog.h>
//
//struct _CusParserContextExtPrivate {
//	CatArrayWo *e_terminals;
//};
//
//static void l_stringable_iface_init(CatIStringableInterface *iface);
//
//G_DEFINE_TYPE_WITH_CODE(CusParserContextExt, cus_parser_context_ext, CUS_TYPE_PARSER_CONTEXT,
//		G_ADD_PRIVATE(CusParserContextExt)
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void cus_parser_context_ext_class_init(CusParserContextExtClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void cus_parser_context_ext_init(CusParserContextExt *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
////	CusParserContextExt *instance = CUS_PARSER_CONTEXT_EXT(object);
////	CusParserContextExtPrivate *priv = cus_parser_context_ext_get_instance_private(instance);
//	G_OBJECT_CLASS(cus_parser_context_ext_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(cus_parser_context_ext_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//
//CusParserContextExt *cus_parser_context_ext_new(CusIScanner *scanner) {
//	CusParserContextExt *result = g_object_new(CUS_TYPE_PARSER_CONTEXT_EXT, NULL);
//	cat_ref_anounce(result);
//	CusParserContextExtPrivate *priv = cus_parser_context_ext_get_instance_private(result);
//	priv->e_terminals = cat_array_wo_new();
//	cus_parser_context_construct((CusParserContext *) result, scanner);
//	return result;
//}
//
//CusTerminal *cus_parser_context_ext_create_term(CusParserContext *parser_context, CatStringWo *name) {
//	CusParserContextExtPrivate *priv = cus_parser_context_ext_get_instance_private((CusParserContextExt *) parser_context);
//	int index = cat_array_wo_size(priv->e_terminals);
//	CusTerminal *result = cus_terminal_new(index, cat_ref_ptr(name));
//	cat_array_wo_append(priv->e_terminals, (GObject *) result);
//	cat_log_debug("result=%o", result);
//	return result;
//}
//
//void cus_parser_context_ext_set_stack_type(CusParserContext *parser_context, CatStringWo *stack_type, CatArrayWo *symbol_list) {
//	CatIIterator *iter = (CatIIterator *) cat_array_wo_iterator(symbol_list);
//	while(cat_iiterator_has_next(iter)) {
//		CusSymbol *symbol = (CusSymbol *) cat_iiterator_next(iter);
//		cus_symbol_set_stack_type(symbol, stack_type);
//	}
//	cat_unref_ptr(iter);
//}
//
//CusSymbol *cus_parser_context_ext_find_symbol(CusParserContext *parser_context, CatStringWo *symname) {
//	CusSymbol *result = NULL;
//	CusParserContextExtPrivate *priv = cus_parser_context_ext_get_instance_private((CusParserContextExt *) parser_context);
//	CatIIterator *iter = (CatIIterator *) cat_array_wo_iterator(priv->e_terminals);
//	while(cat_iiterator_has_next(iter)) {
//		CusSymbol *symbol = (CusSymbol *) cat_iiterator_next(iter);
//		if (cat_string_wo_equal(cus_symbol_get_name(symbol), symname)) {
//			result = symbol;
//			break;
//		}
//	}
//	cat_unref_ptr(iter);
//	cat_log_debug("searching:%o, result=%o", symname, result);
//	return result;
//}
//
//
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	CusParserContextExt *instance = CUS_PARSER_CONTEXT_EXT(self);
//	CusParserContextExtPrivate *priv = cus_parser_context_ext_get_instance_private(instance);
//	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
//
//	cat_string_wo_format(append_to, "%s[%p]", iname, self);
//}
//
//static void l_stringable_iface_init(CatIStringableInterface *iface) {
//	iface->print = l_stringable_print;
//}
//
///********************* end CatIStringable implementation *********************/
