//
///*
//   File:    grobnfstacktype.c
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
//#include "grobnfstacktype.h"
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "GroBnfStackType"
//#include <logging/catlog.h>
//
//struct _GroBnfStackTypePrivate {
//	GroBnfString *as_text;
//};
//
//static void l_stringable_iface_init(CatIStringableInterface *iface);
//
//G_DEFINE_TYPE_WITH_CODE(GroBnfStackType, gro_bnf_stack_type, G_TYPE_OBJECT,
//		G_ADD_PRIVATE(GroBnfStackType)
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void gro_bnf_stack_type_class_init(GroBnfStackTypeClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void gro_bnf_stack_type_init(GroBnfStackType *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	GroBnfStackType *instance = GRO_BNF_STACK_TYPE(object);
//	GroBnfStackTypePrivate *priv = gro_bnf_stack_type_get_instance_private(instance);
//	cat_unref_ptr(priv->as_text);
//	G_OBJECT_CLASS(gro_bnf_stack_type_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(gro_bnf_stack_type_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//
//GroBnfStackType *gro_bnf_stack_type_new(GroBnfString *as_text) {
//	GroBnfStackType *result = g_object_new(GRO_TYPE_BNF_STACK_TYPE, NULL);
//	cat_ref_anounce(result);
//	GroBnfStackTypePrivate *priv = gro_bnf_stack_type_get_instance_private(result);
//	priv->as_text = cat_ref_ptr(as_text);
//	return result;
//}
//
//
//CatStringWo *gro_bnf_stack_type_get_as_text(GroBnfStackType *stack_type) {
//	GroBnfStackTypePrivate *priv = gro_bnf_stack_type_get_instance_private(stack_type);
//	return gro_bnf_string_get_text(priv->as_text);
//}
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroBnfStackType *instance = GRO_BNF_STACK_TYPE(self);
//	GroBnfStackTypePrivate *priv = gro_bnf_stack_type_get_instance_private(instance);
//	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
//
//	cat_string_wo_format(append_to, "%s[%O]", iname, priv->as_text);
//}
//
//static void l_stringable_iface_init(CatIStringableInterface *iface) {
//	iface->print = l_stringable_print;
//}
//
///********************* end CatIStringable implementation *********************/
