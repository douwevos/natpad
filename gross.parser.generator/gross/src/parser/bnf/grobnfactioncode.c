//
///*
//   File:    grobnfactioncode.c
//   Project: gross
//   Author:  Douwe Vos
//   Date:    Jun 30, 2016
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
//#include "grobnfactioncode.h"
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "GroBnfActionCode"
//#include <logging/catlog.h>
//
//struct _GroBnfActionCodePrivate {
//	GroBnfString *code;
//};
//
//static void l_stringable_iface_init(CatIStringableInterface *iface);
//
//G_DEFINE_TYPE_WITH_CODE(GroBnfActionCode, gro_bnf_action_code, G_TYPE_OBJECT,
//		G_ADD_PRIVATE(GroBnfActionCode)
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void gro_bnf_action_code_class_init(GroBnfActionCodeClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void gro_bnf_action_code_init(GroBnfActionCode *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
////	GroBnfActionCode *instance = GRO_BNF_ACTION_CODE(object);
////	GroBnfActionCodePrivate *priv = gro_bnf_action_code_get_instance_private(instance);
//	G_OBJECT_CLASS(gro_bnf_action_code_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(gro_bnf_action_code_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//
//GroBnfActionCode *gro_bnf_action_code_new(GroBnfString *code) {
//	GroBnfActionCode *result = g_object_new(GRO_TYPE_BNF_ACTION_CODE, NULL);
//	cat_ref_anounce(result);
//	GroBnfActionCodePrivate *priv = gro_bnf_action_code_get_instance_private(result);
//	priv->code = cat_ref_ptr(code);
//	return result;
//}
//
//CatStringWo *gro_bnf_action_code_get_code_string(GroBnfActionCode *action_code) {
//	GroBnfActionCodePrivate *priv = gro_bnf_action_code_get_instance_private(action_code);
//	return gro_bnf_string_get_text(priv->code);
//}
//
//
//
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroBnfActionCode *instance = GRO_BNF_ACTION_CODE(self);
//	GroBnfActionCodePrivate *priv = gro_bnf_action_code_get_instance_private(instance);
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
