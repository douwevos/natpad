///*
//   File:    jagclazznotifyrequest.c
//   Project: jaguar
//   Author:  Douwe Vos
//   Date:    Aug 21, 2012
//   Web:     http://www.natpad.net/
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2012 Douwe Vos.
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
//
//
//#include "jagclazznotifyrequest.h"
//#include "../source/jagastparsercontext.h"
//#include "../../module/jagmonnodesourcefolder.h"
//#include "../../parser/jagscanner.h"
//#include "../../parser/generated/jagparser.h"
//#include <viper.h>
//#include <monkey.h>
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "JagClazzNotifyRequest"
//#include <logging/catlog.h>
//
//struct _JagClazzNotifyRequestPrivate {
//	JagClazzNotify *clazz_notify;
//	JagNameMap *name_map;
//};
//
//G_DEFINE_TYPE (JagClazzNotifyRequest, jag_clazz_notify_request, WOR_TYPE_REQUEST)
//
//static gpointer parent_class = NULL;
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//static void l_run_request(WorRequest *request);
//
//static void jag_clazz_notify_request_class_init(JagClazzNotifyRequestClass *clazz) {
//	parent_class = g_type_class_peek_parent(clazz);
//	g_type_class_add_private(clazz, sizeof(JagClazzNotifyRequestPrivate));
//
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//
//	WorRequestClass *worm_clazz = WOR_REQUEST_CLASS(clazz);
//	worm_clazz->runRequest = l_run_request;
//}
//
//static void jag_clazz_notify_request_init(JagClazzNotifyRequest *instance) {
//	JagClazzNotifyRequestPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_CLAZZ_NOTIFY_REQUEST, JagClazzNotifyRequestPrivate);
//	instance->priv = priv;
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	JagClazzNotifyRequest *instance = JAG_CLAZZ_NOTIFY_REQUEST(object);
//	JagClazzNotifyRequestPrivate *priv = instance->priv;
//	cat_unref_ptr(priv->clazz_notify);
//	cat_unref_ptr(priv->name_map);
//	G_OBJECT_CLASS(parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//JagClazzNotifyRequest *jag_clazz_notify_request_new(JagClazzNotify *clazz_notify) {
//	JagClazzNotifyRequest *result = g_object_new(JAG_TYPE_CLAZZ_NOTIFY_REQUEST, NULL);
//	cat_ref_anounce(result);
//	JagClazzNotifyRequestPrivate *priv = result->priv;
//	priv->clazz_notify = cat_ref_ptr(clazz_notify);
//	JagMonNodeSourceFolder *jag_source_folder = jag_clazz_notify_get_source_folder(clazz_notify);
//	JagNameMap *name_map = NULL;
//	if (jag_source_folder) {
//		name_map = jag_mon_node_source_folder_get_name_map(jag_source_folder);
//	}
//	priv->name_map = cat_ref_ptr(name_map);
//	wor_request_construct((WorRequest *) result);
//	return result;
//}
//
//
//
//static void l_run_request(WorRequest *request) {
//	JagClazzNotifyRequestPrivate *priv = JAG_CLAZZ_NOTIFY_REQUEST_GET_PRIVATE(request);
//
//	cat_log_debug("running request:req=%p, clazz_notify=%p", request, priv->clazz_notify);
//
//	if (priv->clazz_notify==NULL) {
//		cat_log_fatal("clazz_notify is NULL : request=%s", g_type_name_from_instance((GTypeInstance *) request));
//		return;
//	}
//
//	if (priv->name_map==NULL) {
//		cat_log_fatal("name_map is NULL : request=%s", g_type_name_from_instance((GTypeInstance *) request));
//		return;
//	}
//
//
//	JagMonNodeSourceFile *mon_node_clazz_file = jag_clazz_notify_get_node(priv->clazz_notify);
//	MonResourceNode *resource_node = jag_mon_node_source_file_get_linked_node(mon_node_clazz_file);
//
//	VipResource *resource = mon_resource_node_get_resource(resource_node);
//
//	VipResourceNode *vip_resource_node = vip_resource_get_node(resource);
//	VipPath *resource_path = vip_resource_node_create_path(vip_resource_node);
//
//	cat_log_debug("[%p] opening source:%s", request, cat_string_wo_getchars(vip_path_to_string(resource_path)));
//
//	CatFileInputStream *fis = cat_file_input_stream_new(vip_path_to_string(resource_path));
//	cat_unref_ptr(resource_path);
//	CatUtf8InputStreamScanner *stream_scanner = cat_utf8_input_stream_scanner_new(CAT_IINPUT_STREAM(fis));
//	cat_unref_ptr(fis);
//	CatIUtf8Scanner *scanner = CAT_IUTF8_SCANNER(stream_scanner);
//
//	JagScanner *java_scanner = jag_scanner_new_scanner(scanner);
//	java_scanner->create_ast_tokens = TRUE;
//	java_scanner->filter_non_parsable = TRUE;
//
//	JagParser *jav_parser = jag_parser_new(JAG_ISCANNER(java_scanner));
//	JagASTParserContext *parser_context = jag_ast_parser_context_new(JAG_ISCANNER(java_scanner), priv->name_map);
//	JagToken *goal_token = jag_parser_base_parse(JAG_PARSER_BASE(jav_parser), JAG_PARSER_CONTEXT(parser_context));
//	if (goal_token) {
//		cat_log_debug("[%p] got goal token", request);
//	} else {
//		cat_log_debug("[%p] no goal token", request);
//	}
//
//	CatArrayWo *a_clazz_list = jag_ast_parser_context_get_ast_clazz_list(parser_context);
//	cat_log_debug("[%p] clazz_list=%p", request, clazz_list);
//	cat_log_debug("[%p] clazz_list.size=%d", request, cat_array_wo_size(a_clazz_list));
//
//	jag_mon_node_source_file_set_ast_clazz_list(mon_node_clazz_file, clazz_list);
//
//
//
//	cat_unref_ptr(parser_context);
//	cat_unref_ptr(stream_scanner);
////	cat_unref_ptr(goal_token);
//	cat_unref_ptr(jav_parser);
//	cat_unref_ptr(java_scanner);
//
//}
//
//
//
