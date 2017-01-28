//
///*
//   File:    grobnfproduction.c
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
//#include "grobnfproduction.h"
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "GroBnfProduction"
//#include <logging/catlog.h>
//
//struct _GroBnfProductionPrivate {
//	GroBnfString *lhs_name;
//	GroBnfRhs *rhs;
//};
//
//static void l_stringable_iface_init(CatIStringableInterface *iface);
//
//G_DEFINE_TYPE_WITH_CODE(GroBnfProduction, gro_bnf_production, G_TYPE_OBJECT,
//		G_ADD_PRIVATE(GroBnfProduction)
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void gro_bnf_production_class_init(GroBnfProductionClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void gro_bnf_production_init(GroBnfProduction *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
////	GroBnfProduction *instance = GRO_BNF_PRODUCTION(object);
////	GroBnfProductionPrivate *priv = gro_bnf_production_get_instance_private(instance);
//	G_OBJECT_CLASS(gro_bnf_production_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(gro_bnf_production_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//
//GroBnfProduction *gro_bnf_production_new(GroBnfString *lhs_name, GroBnfRhs *rhs) {
//	GroBnfProduction *result = g_object_new(GRO_TYPE_BNF_PRODUCTION, NULL);
//	cat_ref_anounce(result);
//	GroBnfProductionPrivate *priv = gro_bnf_production_get_instance_private(result);
//	priv->lhs_name = cat_ref_ptr(lhs_name);
//	priv->rhs = cat_ref_ptr(rhs);
//	return result;
//}
//
//GroBnfString *gro_bnf_production_get_lhs_string(GroBnfProduction *bnf_prod) {
//	GroBnfProductionPrivate *priv = gro_bnf_production_get_instance_private(bnf_prod);
//	return priv->lhs_name;
//}
//
//GroBnfRhs *gro_bnf_production_get_rhs(GroBnfProduction *bnf_prod) {
//	GroBnfProductionPrivate *priv = gro_bnf_production_get_instance_private(bnf_prod);
//	return priv->rhs;
//}
//
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroBnfProduction *instance = GRO_BNF_PRODUCTION(self);
//	GroBnfProductionPrivate *priv = gro_bnf_production_get_instance_private(instance);
//	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
//
//	cat_string_wo_format(append_to, "%s[%p, lhs=%o, rhs=%o]", iname, self, priv->lhs_name, priv->rhs);
//}
//
//static void l_stringable_iface_init(CatIStringableInterface *iface) {
//	iface->print = l_stringable_print;
//}
//
///********************* end CatIStringable implementation *********************/
