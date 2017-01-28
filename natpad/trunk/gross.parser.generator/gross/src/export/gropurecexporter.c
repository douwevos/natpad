//
///*
//   File:    gropurecexporter.c
//   Project: gross
//   Author:  Douwe Vos
//   Date:    Jun 7, 2016
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
//#include "gropurecexporter.h"
//#include "gropurecsymbolexporter.h"
//#include "gropurecactionsexporter.h"
//#include "gropurectablesexporter.h"
//#include <gio/gio.h>
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "GroPureCExporter"
//#include <logging/catlog.h>
//
//struct _GroPureCExporterPrivate {
//	CatStringWo *output_path;
//};
//
//static void l_stringable_iface_init(CatIStringableInterface *iface);
//
//G_DEFINE_TYPE_WITH_CODE(GroPureCExporter, gro_pure_c_exporter, G_TYPE_OBJECT,
//		G_ADD_PRIVATE(GroPureCExporter)
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void gro_pure_c_exporter_class_init(GroPureCExporterClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void gro_pure_c_exporter_init(GroPureCExporter *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
////	GroPureCExporter *instance = GRO_PURE_C_EXPORTER(object);
////	GroPureCExporterPrivate *priv = gro_pure_c_exporter_get_instance_private(instance);
//	G_OBJECT_CLASS(gro_pure_c_exporter_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(gro_pure_c_exporter_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//
//GroPureCExporter *gro_pure_c_exporter_new(CatStringWo *output_path) {
//	GroPureCExporter *result = g_object_new(GRO_TYPE_PURE_C_EXPORTER, NULL);
//	cat_ref_anounce(result);
//	GroPureCExporterPrivate *priv = gro_pure_c_exporter_get_instance_private(result);
//	priv->output_path = cat_ref_ptr(output_path);
//	return result;
//}
//
//
//static void l_init_name(GroClassName *cname, const CatStringWo *prefix_hyp, const CatStringWo *c_name_hyp, const CatStringWo *c_name_lower) {
//	cname->prefix_hyp = cat_string_wo_clone(prefix_hyp, CAT_CLONE_DEPTH_AS_ANCHORED);
//	cname->prefix_up = cat_string_wo_clone(prefix_hyp, CAT_CLONE_DEPTH_FULL);
//	cat_string_wo_to_uppercase(cname->prefix_up);
//	cname->prefix_lower = cat_string_wo_clone(prefix_hyp, CAT_CLONE_DEPTH_FULL);
//	cat_string_wo_to_lowercase(cname->prefix_lower);
//
//	cname->name_hyp = cat_string_wo_clone(c_name_hyp, CAT_CLONE_DEPTH_AS_ANCHORED);
//	cname->name_lower = cat_string_wo_clone(c_name_lower, CAT_CLONE_DEPTH_AS_ANCHORED);
//	cname->name_up = cat_string_wo_clone(cname->name_lower, CAT_CLONE_DEPTH_FULL);
//	cat_string_wo_to_uppercase(cname->name_up);
//	cname->name_up = cat_string_wo_anchor(cname->name_up, 0);
//}
//
//
//void gro_pure_c_exporter_do_export(GroPureCExporter *exporter, GroModel *model, GroStateMachine *state_machine) {
//	GroPureCExporterPrivate *priv = gro_pure_c_exporter_get_instance_private(exporter);
//
//	CatStringWo *base_path = priv->output_path;
//	GFile *file = g_file_new_for_path(cat_string_wo_getchars(base_path));
//	GError *error = NULL;
//	gboolean result = g_file_make_directory_with_parents(file, NULL, &error);
//	if (!result && error!=NULL) {
//		if (error->code==G_IO_ERROR_EXISTS) {
//			result = TRUE;
//		}
//	}
//
//	if (!result) {
//		cat_log_error("Creating directory for output files failed:%s", error->message);
//		return;
//	}
//
//	const CatStringWo *prefix_hyp = cat_string_wo_new_data("Gte");
//
//	GroClassName csymbolname, cactionsname, ctablesname;
//	l_init_name(&csymbolname, prefix_hyp, cat_string_wo_new_data("Symbol"), cat_string_wo_new_data("symbol"));
//	l_init_name(&cactionsname, prefix_hyp, cat_string_wo_new_data("ParserActions"), cat_string_wo_new_data("parser_actions"));
//	l_init_name(&ctablesname, prefix_hyp, cat_string_wo_new_data("ParserTables"), cat_string_wo_new_data("parser_tables"));
//
//	gro_pure_c_export_symbols(model, state_machine, file, &csymbolname);
//
//	gro_pure_c_export_actions(model, state_machine, file, &cactionsname);
//
//	gro_pure_c_export_tables(model, state_machine, file, &ctablesname);
//}
//
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
////	GroPureCExporter *instance = GRO_PURE_C_EXPORTER(self);
////	GroPureCExporterPrivate *priv = gro_pure_c_exporter_get_instance_private(instance);
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
