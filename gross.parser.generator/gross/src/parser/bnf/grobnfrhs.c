//
///*
//   File:    grobnfrhs.c
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
//#include "grobnfrhs.h"
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "GroBnfRhs"
//#include <logging/catlog.h>
//
//struct _GroBnfRhsPrivate {
//	CatArrayWo *production_parts;
//	GroBnfString *prec_terminal_name;
//};
//
//static void l_stringable_iface_init(CatIStringableInterface *iface);
//
//G_DEFINE_TYPE_WITH_CODE(GroBnfRhs, gro_bnf_rhs, G_TYPE_OBJECT,
//		G_ADD_PRIVATE(GroBnfRhs)
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void gro_bnf_rhs_class_init(GroBnfRhsClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void gro_bnf_rhs_init(GroBnfRhs *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	GroBnfRhs *instance = GRO_BNF_RHS(object);
//	GroBnfRhsPrivate *priv = gro_bnf_rhs_get_instance_private(instance);
//	cat_unref_ptr(priv->production_parts);
//	cat_unref_ptr(priv->prec_terminal_name);
//	G_OBJECT_CLASS(gro_bnf_rhs_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(gro_bnf_rhs_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//
//GroBnfRhs *gro_bnf_rhs_new(CatArrayWo *production_parts) {
//	GroBnfRhs *result = g_object_new(GRO_TYPE_BNF_RHS, NULL);
//	cat_ref_anounce(result);
//	GroBnfRhsPrivate *priv = gro_bnf_rhs_get_instance_private(result);
//	priv->production_parts = cat_ref_ptr(production_parts);
//	priv->prec_terminal_name = NULL;
//	return result;
//}
//
//CatIIterator *gro_bnf_rhs_production_part_iterator(GroBnfRhs *rhs) {
//	GroBnfRhsPrivate *priv = gro_bnf_rhs_get_instance_private(rhs);
//	return cat_array_wo_iterator(priv->production_parts);
//}
//
//GroBnfString *gro_bnf_rhs_get_prec_term_name(GroBnfRhs *rhs) {
//	GroBnfRhsPrivate *priv = gro_bnf_rhs_get_instance_private(rhs);
//	return priv->prec_terminal_name;
//}
//
//void gro_bnf_rhs_set_prec_term_name(GroBnfRhs *rhs, GroBnfString *prec_term_name) {
//	GroBnfRhsPrivate *priv = gro_bnf_rhs_get_instance_private(rhs);
//	priv->prec_terminal_name = cat_ref_ptr(prec_term_name);
//}
//
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroBnfRhs *instance = GRO_BNF_RHS(self);
//	GroBnfRhsPrivate *priv = gro_bnf_rhs_get_instance_private(instance);
//	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
//
//	cat_string_wo_format(append_to, "%s[%p, parts=%o]", iname, self, priv->production_parts);
//}
//
//static void l_stringable_iface_init(CatIStringableInterface *iface) {
//	iface->print = l_stringable_print;
//}
//
///********************* end CatIStringable implementation *********************/
