//
///*
//   File:    grobnfsymboldeflist.c
//   Project: gross
//   Author:  Douwe Vos
//   Date:    May 22, 2016
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
//#include <logging/catlogdefs.h>
//
//#include "grobnfsymboldeflist.h"
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "GroBnfSymbolDefList"
//#include <logging/catlog.h>
//
//struct _GroBnfSymbolDefListPrivate {
//	gboolean is_terminal_list;
//	CatArrayWo *name_list;
//	GroBnfStackType *stack_type;
//};
//
//static void l_stringable_iface_init(CatIStringableInterface *iface);
//
//G_DEFINE_TYPE_WITH_CODE(GroBnfSymbolDefList, gro_bnf_symbol_def_list, G_TYPE_OBJECT,
//		G_ADD_PRIVATE(GroBnfSymbolDefList)
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void gro_bnf_symbol_def_list_class_init(GroBnfSymbolDefListClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void gro_bnf_symbol_def_list_init(GroBnfSymbolDefList *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	GroBnfSymbolDefList *instance = GRO_BNF_SYMBOL_DEF_LIST(object);
//	GroBnfSymbolDefListPrivate *priv = gro_bnf_symbol_def_list_get_instance_private(instance);
//	cat_unref_ptr(priv->name_list);
//	cat_unref_ptr(priv->stack_type);
//	G_OBJECT_CLASS(gro_bnf_symbol_def_list_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(gro_bnf_symbol_def_list_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//
//GroBnfSymbolDefList *gro_bnf_symbol_def_list_new(CatArrayWo *name_list, GroBnfStackType *stack_type, gboolean is_terminal_list) {
//	GroBnfSymbolDefList *result = g_object_new(GRO_TYPE_BNF_SYMBOL_DEF_LIST, NULL);
//	cat_ref_anounce(result);
//	GroBnfSymbolDefListPrivate *priv = gro_bnf_symbol_def_list_get_instance_private(result);
//	priv->is_terminal_list = is_terminal_list;
//	priv->name_list = cat_ref_ptr(name_list);
//	priv->stack_type = cat_ref_ptr(stack_type);
//	return result;
//}
//
//GroBnfStackType *gro_bnf_symbol_def_list_get_stack_type(GroBnfSymbolDefList *list) {
//	GroBnfSymbolDefListPrivate *priv = gro_bnf_symbol_def_list_get_instance_private(list);
//	return priv->stack_type;
//}
//
//CatArrayWo *gro_bnf_symbol_def_list_get_name_list(GroBnfSymbolDefList *list) {
//	GroBnfSymbolDefListPrivate *priv = gro_bnf_symbol_def_list_get_instance_private(list);
//	return priv->name_list;
//}
//
//gboolean gro_bnf_symbol_def_list_is_terminal_list(GroBnfSymbolDefList *list) {
//	GroBnfSymbolDefListPrivate *priv = gro_bnf_symbol_def_list_get_instance_private(list);
//	return priv->is_terminal_list;
//}
//
//
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroBnfSymbolDefList *instance = GRO_BNF_SYMBOL_DEF_LIST(self);
//	GroBnfSymbolDefListPrivate *priv = gro_bnf_symbol_def_list_get_instance_private(instance);
//	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
//
//	cat_string_wo_format(append_to, "%s[%p, name-list=%O]", iname, self, priv->name_list);
//}
//
//static void l_stringable_iface_init(CatIStringableInterface *iface) {
//	iface->print = l_stringable_print;
//}
//
///********************* end CatIStringable implementation *********************/
