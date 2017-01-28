//
///*
//   File:    grobnfcodepart.c
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
//#include "grobnfcodepart.h"
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "GroBnfCodePart"
//#include <logging/catlog.h>
//
//struct _GroBnfCodePartPrivate {
//	CatStringWo *code;
//};
//
//static void l_stringable_iface_init(CatIStringableInterface *iface);
//
//G_DEFINE_TYPE_WITH_CODE(GroBnfCodePart, gro_bnf_code_part, GRO_TYPE_BNF_PRODUCTION_PART,
//		G_ADD_PRIVATE(GroBnfCodePart)
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void gro_bnf_code_part_class_init(GroBnfCodePartClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void gro_bnf_code_part_init(GroBnfCodePart *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	GroBnfCodePart *instance = GRO_BNF_CODE_PART(object);
//	GroBnfCodePartPrivate *priv = gro_bnf_code_part_get_instance_private(instance);
//	cat_unref_ptr(priv->code);
//	G_OBJECT_CLASS(gro_bnf_code_part_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(gro_bnf_code_part_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//
//GroBnfCodePart *gro_bnf_code_part_new(CatStringWo *code) {
//	GroBnfCodePart *result = g_object_new(GRO_TYPE_BNF_CODE_PART, NULL);
//	cat_ref_anounce(result);
//	GroBnfCodePartPrivate *priv = gro_bnf_code_part_get_instance_private(result);
//	gro_bnf_production_part_construct((GroBnfProductionPart *) result);
//	priv->code = cat_ref_ptr(code);
//	return result;
//}
//
//
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
////	GroBnfCodePart *instance = GRO_BNF_CODE_PART(self);
////	GroBnfCodePartPrivate *priv = gro_bnf_code_part_get_instance_private(instance);
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
