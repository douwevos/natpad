///*
//   File:    jagautocompleterequest.c
//   Project: jaguar
//   Author:  Douwe Vos
//   Date:    Jun 16, 2014
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2014 Douwe Vos.
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
//#include "jagautocompleterequest.h"
//#include "../editor/jageditor.h"
//#include "../parser/jagscanner.h"
//#include "../parser/generated/jagparser.h"
//#include "../parser/generated/runtime/jagparsercontext.h"
//#include "../parser/generated/runtime/jagtoken.h"
//#include "../indexer/source/jgiparsercontext.h"
//#include "../indexer/jgiastutil.h"
//#include "../indexer/model/jgitokenrange.h"
//#include "../moose/library/jaglibrarycontentwo.h"
//#include "../moose/srcfolder/jagsrcfoldercontentwo.h"
//#include "../moose/srcfile/jagsrcfilecontentwo.h"
//#include "../moose/package/jagpackagecontent.h"
//
//#include "../parser/jagasttoken.h"
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "JagAutoCompleteRequest"
//#include <logging/catlog.h>
//
//struct _JagAutoCompleteRequestPrivate {
//	ElkEditor *elk_editor;
//	CatArrayWo *e_completions;
//	AstSnapshot *snapshot;
//	AstCursor *cursor;
//
//	JgiParserContext *parser_context;
//
//	MooNodeWo *moo_root_node;
//};
//
//
//G_DEFINE_TYPE(JagAutoCompleteRequest, jag_auto_complete_request, WOR_TYPE_REQUEST)
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//static void l_run_request(WorRequest *request);
//
//static void jag_auto_complete_request_class_init(JagAutoCompleteRequestClass *clazz) {
//	g_type_class_add_private(clazz, sizeof(JagAutoCompleteRequestPrivate));
//
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//
//	WorRequestClass *wor_clazz = WOR_REQUEST_CLASS(clazz);
//	wor_clazz->runRequest = l_run_request;
//}
//
//static void jag_auto_complete_request_init(JagAutoCompleteRequest *instance) {
//	JagAutoCompleteRequestPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_AUTO_COMPLETE_REQUEST, JagAutoCompleteRequestPrivate);
//	instance->priv = priv;
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	JagAutoCompleteRequest *instance = JAG_AUTO_COMPLETE_REQUEST(object);
//	JagAutoCompleteRequestPrivate *priv = instance->priv;
//	cat_unref_ptr(priv->e_completions);
//	cat_unref_ptr(priv->cursor);
//	cat_unref_ptr(priv->elk_editor);
//	cat_unref_ptr(priv->moo_root_node);
//	cat_unref_ptr(priv->parser_context);
//	cat_unref_ptr(priv->snapshot);
//	G_OBJECT_CLASS(jag_auto_complete_request_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(jag_auto_complete_request_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//
//JagAutoCompleteRequest *jag_auto_complete_request_new(ElkEditor *elk_editor) {
//	JagAutoCompleteRequest *result = g_object_new(JAG_TYPE_AUTO_COMPLETE_REQUEST, NULL);
//	cat_ref_anounce(result);
//	JagAutoCompleteRequestPrivate *priv = result->priv;
//	wor_request_construct((WorRequest *) result);
//	priv->elk_editor = cat_ref_ptr(elk_editor);
//	priv->e_completions = cat_array_wo_new();
//	AstEditor *ast_editor = AST_EDITOR(elk_editor);
//	priv->snapshot = ast_document_get_snapshot(ast_editor->ast_document);
//	priv->cursor = cat_ref_ptr(ast_editor->ast_view->document_view->cursor->cursor);
//	priv->parser_context = NULL;
//	priv->moo_root_node = NULL;
//	return result;
//}
//
//
//static MooNodeWo *l_get_moose_root_node(JagAutoCompleteRequest *request) {
//	JagAutoCompleteRequestPrivate *priv = JAG_AUTO_COMPLETE_REQUEST_GET_PRIVATE(request);
//
//	if (priv->moo_root_node==NULL) {
//		JagIndexer *indexer = jag_editor_get_indexer((JagEditor *) priv->elk_editor);
//		if (indexer) {
//			MooService *moo_service = jag_indexer_get_moo_service(indexer);
//			priv->moo_root_node = moo_service_get_root_node_ref(moo_service);
//		}
//	}
//	return priv->moo_root_node;
////	MooIdPath *path = jag_editor_get_moo_node_path((JagEditor *) priv->elk_editor);
//}
//
//static CatArrayWo *l_get_moose_src_folder_nodes(JagAutoCompleteRequest *request) {
//	JagAutoCompleteRequestPrivate *priv = JAG_AUTO_COMPLETE_REQUEST_GET_PRIVATE(request);
//	MooNodeWo *root_node = l_get_moose_root_node(request);
//	cat_log_debug("root_node=%o", root_node);
//	if (root_node==NULL) {
//		return NULL;
//	}
//	MooIdPath *id_path = jag_editor_get_moo_node_path((JagEditor *) priv->elk_editor);
//	cat_log_debug("id_path=%o", id_path);
//	if (id_path==NULL) {
//		return NULL;
//	}
//
//	MooNodeWo *module_node = moo_id_path_get_at(id_path, root_node, 0);
//	cat_log_debug("module_node=%o", module_node);
//	if (module_node==NULL) {
//		return NULL;
//	}
//
//	CatArrayWo *e_result = cat_array_wo_new();
//	int count = moo_node_wo_child_count(module_node);
//	for(count--; count>=0; count--) {
//		MooNodeWo *child_node = moo_node_wo_child_at(module_node, count);
//		cat_log_debug("child_node=%o", child_node);
//		JagSrcFolderContentWo *content = (JagSrcFolderContentWo *) moo_node_wo_get_content(child_node, jag_src_folder_content_wo_key());
//		if (content == NULL) {
//			continue;
//		}
//		cat_array_wo_append(e_result, (GObject *) child_node);
//	}
//	return e_result;
//}
//
//
//static CatArrayWo *l_get_moose_library_nodes(JagAutoCompleteRequest *request) {
//	JagAutoCompleteRequestPrivate *priv = JAG_AUTO_COMPLETE_REQUEST_GET_PRIVATE(request);
//	MooNodeWo *root_node = l_get_moose_root_node(request);
//	cat_log_debug("root_node=%o", root_node);
//	if (root_node==NULL) {
//		return NULL;
//	}
//	MooIdPath *id_path = jag_editor_get_moo_node_path((JagEditor *) priv->elk_editor);
//	cat_log_debug("id_path=%o", id_path);
//	if (id_path==NULL) {
//		return NULL;
//	}
//
//	MooNodeWo *module_node = moo_id_path_get_at(id_path, root_node, 0);
//	cat_log_debug("module_node=%o", module_node);
//	if (module_node==NULL) {
//		return NULL;
//	}
//
//	CatArrayWo *e_result = cat_array_wo_new();
//	int count = moo_node_wo_child_count(module_node);
//	for(count--; count>=0; count--) {
//		MooNodeWo *child_node = moo_node_wo_child_at(module_node, count);
//		cat_log_debug("child_node=%o", child_node);
//		JagLibraryContentWo *content = (JagLibraryContentWo *) moo_node_wo_get_content(child_node, jag_library_content_wo_key());
//		if (content == NULL) {
//			continue;
//		}
//		cat_array_wo_append(e_result, (GObject *) child_node);
//	}
//	return e_result;
//}
//
//static CatStringWo *l_create_fqn_name(JgiTokenRange *name_token_range, int row, int column) {
//	CatStringWo *e_result = cat_string_wo_new();
//	CatIIterator *iter = jgi_token_range_iterator(name_token_range);
//	while(cat_iiterator_has_next(iter)) {
//		JagToken *token = (JagToken *) cat_iiterator_next(iter);
//		CatStringWo *a_token_text = CAT_STRING_WO(token->value);
//		if (a_token_text==NULL) {
//			if (token->sym==JAG_SYMBOL_TERM_DOT) {
//				a_token_text = cat_string_wo_new_with(".");
//			} else {
//				cat_ref_ptr(a_token_text);
//			}
//		} else {
//			cat_ref_ptr(a_token_text);
//		}
//
//		if (a_token_text!=NULL) {
//			if (row<0 || row>token->right_row || (row==token->right_row && column>=token->right)) {
//				cat_string_wo_append(e_result, a_token_text);
//			} else {
//				int sublen = column - token->left;
//				CatStringWo *e_subpart = cat_string_wo_new_sub(a_token_text, 0, sublen);
//				e_subpart = cat_string_wo_anchor(e_subpart, 0);
//				cat_string_wo_append(e_result, e_subpart);
//				cat_unref_ptr(e_subpart);
//			}
//			cat_unref_ptr(a_token_text);
//		}
//
//	}
//	cat_unref_ptr(iter);
//	return e_result;
//}
//
//static void l_enlist_import(JagAutoCompleteRequest *request, JgiTokenRange *name_token_range) {
//	JagAutoCompleteRequestPrivate *priv = JAG_AUTO_COMPLETE_REQUEST_GET_PRIVATE(request);
//
//	int import_token_index = jgi_token_range_get_first_index(name_token_range)-1;
//	int semi_index = -1;
//
//	int focus_index = jgi_parser_context_get_focus_index(priv->parser_context);
//	CatArrayWo *e_token_array = jgi_parser_context_get_token_array(priv->parser_context);
//	JagAstToken *ast_token_at_focus = (JagAstToken *) cat_array_wo_get(e_token_array, focus_index);
//
//	if (name_token_range) {
//		CatArrayWo *e_package_name_list = cat_array_wo_new();
//		int row = priv->cursor->row;
//		int column = priv->cursor->column;
//
//		CatStringWo *e_qname_as_text = l_create_fqn_name(name_token_range, row, column);
//		CatStringWo *a_full_name = cat_string_wo_anchor(e_qname_as_text, 0);
//		cat_unref_ptr(e_qname_as_text);
//		int last_dot_idx = cat_string_wo_last_index_of(a_full_name, '.');
//		cat_log_debug("IMPORT %s ... SEMI", cat_string_wo_getchars(a_full_name));
//
//		/* source folders */
//		CatArrayWo *src_folder_nodes = l_get_moose_src_folder_nodes(request);
//		if (src_folder_nodes) {
//			CatIIterator *iter = cat_array_wo_iterator(src_folder_nodes);
//			while(cat_iiterator_has_next(iter)) {
//				MooNodeWo *library_node = (MooNodeWo *) cat_iiterator_next(iter);
//				cat_log_debug("src_folder_node=%o", library_node);
//				int src_folder_count = moo_node_wo_child_count(library_node);
//				for(src_folder_count--; src_folder_count>=0; src_folder_count--) {
//					MooNodeWo *jag_node = moo_node_wo_child_at(library_node, src_folder_count);
//					cat_log_debug("jag_node=%o", jag_node);
//					MooIContent *pack_cont = moo_node_wo_get_content(jag_node, jag_package_content_key());
//					if (pack_cont) {
//						CatStringWo *a_pck_name = moo_node_wo_get_name(jag_node);
//						cat_log_trace("pck_name=%o", pck_name);
//						if (cat_string_wo_length(a_pck_name)>last_dot_idx) {
//							if (cat_string_wo_beginswith(a_pck_name, a_full_name)) {
//								cat_array_wo_append(e_package_name_list, (GObject *) a_pck_name);
//							}
//						} else {
//							int pack_count = moo_node_wo_child_count(jag_node);
//							for(pack_count--; pack_count>=0; pack_count--) {
//								MooNodeWo *jag_sub_node = moo_node_wo_child_at(jag_node, pack_count);
//								MooIContent *srcfile_cont = moo_node_wo_get_content(jag_sub_node, jag_srcfile_content_wo_key());
//								if (srcfile_cont) {
//									CatStringWo *e_nn = cat_string_wo_new();
//									cat_string_wo_append(e_nn, a_pck_name);
//									cat_string_wo_append_char(e_nn, '.');
//									cat_string_wo_append(e_nn, moo_node_wo_get_name(jag_sub_node));
//									CatStringWo *a_tesn = cat_string_wo_anchor(e_nn, 0);
//									cat_log_trace("tesn=%o", a_tesn);
//									if (cat_string_wo_beginswith(a_tesn, a_full_name)) {
//										cat_array_wo_append(e_package_name_list, (GObject *) a_tesn);
//									}
//									cat_unref_ptr(a_tesn);
//								}
//							}
//						}
//					} else {
//						MooIContent *srcfile_cont = moo_node_wo_get_content(jag_node, jag_srcfile_content_wo_key());
//						if (srcfile_cont) {
//
//						}
//					}
//				}
//			}
//			cat_unref_ptr(iter);
//		}
//
//
//		/* jars */
//		CatArrayWo *a_library_nodes = l_get_moose_library_nodes(request);
//		if (a_library_nodes) {
//			CatIIterator *iter = cat_array_wo_iterator(a_library_nodes);
//			while(cat_iiterator_has_next(iter)) {
//				MooNodeWo *src_folder_node = (MooNodeWo *) cat_iiterator_next(iter);
//				cat_log_debug("src_folder_node=%o", src_folder_node);
//				int src_folder_count = moo_node_wo_child_count(src_folder_node);
//				for(src_folder_count--; src_folder_count>=0; src_folder_count--) {
//					MooNodeWo *jag_node = moo_node_wo_child_at(src_folder_node, src_folder_count);
//					cat_log_debug("jag_node=%o", jag_node);
//					MooIContent *pack_cont = moo_node_wo_get_content(jag_node, jag_package_content_key());
//					if (pack_cont) {
//						CatStringWo *a_pck_name = moo_node_wo_get_name(jag_node);
//						cat_log_trace("pck_name=%o", a_pck_name);
//						if (cat_string_wo_length(a_pck_name)>last_dot_idx) {
//							if (cat_string_wo_beginswith(a_pck_name, a_full_name)) {
//								cat_array_wo_append(e_package_name_list, (GObject *) a_pck_name);
//							}
//						} else {
//							int pack_count = moo_node_wo_child_count(jag_node);
//							for(pack_count--; pack_count>=0; pack_count--) {
//								MooNodeWo *jag_sub_node = moo_node_wo_child_at(jag_node, pack_count);
//								MooIContent *srcfile_cont = moo_node_wo_get_content(jag_sub_node, jag_srcfile_content_wo_key());
//								if (srcfile_cont) {
//									CatStringWo *e_nn = cat_string_wo_new();
//									cat_string_wo_append(e_nn, a_pck_name);
//									cat_string_wo_append_char(e_nn, '.');
//									cat_string_wo_append(e_nn, moo_node_wo_get_name(jag_sub_node));
//									CatStringWo *a_tesn = cat_string_wo_anchor(e_nn, 0);
//									cat_log_trace("tesn=%o", a_tesn);
//									if (cat_string_wo_beginswith(a_tesn, a_full_name)) {
//										cat_array_wo_append(e_package_name_list, (GObject *) a_tesn);
//									}
//									cat_unref_ptr(a_tesn);
//								}
//							}
//						}
//					} else {
//						MooIContent *srcfile_cont = moo_node_wo_get_content(jag_node, jag_srcfile_content_wo_key());
//						if (srcfile_cont) {
//
//						}
//					}
//				}
//			}
//			cat_unref_ptr(iter);
//		}
//
//
//
//		semi_index = focus_index+1;
//		int trow = JAG_TOKEN(ast_token_at_focus)->left_row;
//		while(semi_index<cat_array_wo_size(e_token_array)) {
//			JagToken *semi_token = (JagToken *) cat_array_wo_get(e_token_array, semi_index);
//			if (semi_token->sym==JAG_SYMBOL_TERM_SEMICOLON) {
//				break;
//			}
//			if (semi_token->left_row!=trow) {
//				semi_index--;
//				break;
//			}
//			semi_index++;
//		}
//		if (semi_index>=cat_array_wo_size(e_token_array)) {
//			semi_index = cat_array_wo_size(e_token_array)-1;
//		}
//
//
//		JagToken *semi_token = (JagToken *) cat_array_wo_get(e_token_array, semi_index);
//		JagToken *package_token = (JagToken *) cat_array_wo_get(e_token_array, import_token_index);
//
//		AstCursor *cursor_start = ast_cursor_new(package_token->left, package_token->left_row);
//		AstCursor *cursor_end = ast_cursor_new(semi_token->right, semi_token->right_row);
//
//		CatIIterator *iter = cat_array_wo_iterator(e_package_name_list);
//		while(cat_iiterator_has_next(iter)) {
//			CatStringWo *a_pkg_name = (CatStringWo *) cat_iiterator_next(iter);
//			CatStringWo *e_replace = cat_string_wo_new_with("import ");
//			cat_string_wo_append(e_replace, a_pkg_name);
//			cat_string_wo_append_chars(e_replace, ";");
//			CatStringWo *a_full_statement = cat_string_wo_anchor(e_replace, 0);
//			ElkCompletionReplaceEntry *entry = elk_completion_replace_entry_new(a_pkg_name, cursor_start, cursor_end, a_full_statement);
//			cat_array_wo_append(priv->e_completions, (GObject *) entry);
//			cat_unref_ptr(entry);
//			cat_unref_ptr(a_full_statement);
////				priv->e_completions_available = TRUE;
//			cat_unref_ptr(a_pkg_name);
//		}
//		cat_unref_ptr(iter);
//		cat_unref_ptr(cursor_start);
//		cat_unref_ptr(cursor_end);
//		cat_unref_ptr(e_package_name_list);
//	}
//}
//
//
//
//static void l_enlist_identifier(JagAutoCompleteRequest *request) {
//	JagAutoCompleteRequestPrivate *priv = JAG_AUTO_COMPLETE_REQUEST_GET_PRIVATE(request);
//
//	int focus_index = jgi_parser_context_get_focus_index(priv->parser_context);
//	CatArrayWo *e_token_array = jgi_parser_context_get_token_array(priv->parser_context);
//
//	cat_log_on_debug({
//		JagAstToken *ast_token_at_focus = (JagAstToken *) cat_array_wo_get(token_array, focus_index);
//		CatStringWo *dt = jag_ast_token_as_string(ast_token_at_focus);
//		cat_log_debug("enlisting for identifier: %s", cat_string_wo_getchars(dt));
//	});
//
//
//	JgiTokenRange *token_range = jgi_ast_util_extract_name_root(e_token_array, focus_index);
//	jgi_token_range_dump(token_range);
//
//
//	int prec_index = jgi_token_range_get_first_index(token_range)-1;
//	if (prec_index<0) {
//		return;
//	}
//	JagAstToken *prec_token = (JagAstToken *) cat_array_wo_get(e_token_array, prec_index);
//
//	switch(JAG_TOKEN(prec_token)->sym) {
//		case JAG_SYMBOL_TERM_IMPORT : {
//			l_enlist_import(request, token_range);
//		} break;
//	}
//
//
//
//
////	JagAstToken *name_root = jgi_ast_util_extract_name_root(ast_token_at_focus);
////	jgi_ast_util_dump_reverse(ast_token_at_focus, name_root);
//
//
//
//}
//
//
//
//
//void l_dump_stack(JagParserContext *parser_context) {
//	int stackIdx;
//	CatStringWo *e_indent = cat_string_wo_new();
//	for(stackIdx=0; stackIdx<cat_array_wo_size(parser_context->e_stack); stackIdx++) {
//		JagToken *token = (JagToken *) cat_array_wo_get(parser_context->e_stack, stackIdx);
//		if (token!=NULL) {
//			cat_log_debug("stack[%d] : %ssym=%d, state=%d, text=%s, l/r=%d#%d/%d#%d", stackIdx, cat_string_wo_getchars(e_indent), token->sym, token->parse_state, token->symbol_text, token->left, token->left_row, token->right, token->right_row);
//		}
//		if (!token->is_terminal) {
////		if (JAG_IS_AST_TOKEN(token)) {
//			cat_string_wo_append_chars(e_indent, "  ");
//		}
//	}
//	cat_unref_ptr(e_indent);
//}
//
//static void l_enlist(JagAutoCompleteRequest *request) {
//	JagAutoCompleteRequestPrivate *priv = JAG_AUTO_COMPLETE_REQUEST_GET_PRIVATE(request);
//
////	priv->indexer_project = l_get_indexer_project(request);
//
//	CatIUtf8Scanner *scanner = CAT_IUTF8_SCANNER(ast_snapshot_scanner_new(priv->snapshot));
//
//	JagScanner *java_scanner = jag_scanner_new(scanner);
//	java_scanner->filter_non_parsable = TRUE;
//	java_scanner->create_ast_tokens = TRUE;
//	JagParser *jav_parser = jag_parser_new(JAG_ISCANNER(java_scanner));
//
//	int row = priv->cursor->row;
//	int column = priv->cursor->column;
//
//	cat_log_debug("for row=%d column=%d", row, column);
//
//
//	MooService *moo_service = elk_editor_get_moose_service(priv->elk_editor);
//
//	JgiParserContext *parser_context = jgi_parser_context_new(JAG_ISCANNER(java_scanner), (VipISequence *) moo_service);
//	priv->parser_context = parser_context;
//	jgi_parser_context_enlist_tokens(parser_context, row, column);
//
////	priv->indexer_parser_context = parser_context;
//	jag_parser_base_parse(JAG_PARSER_BASE(jav_parser), JAG_PARSER_CONTEXT(parser_context));
//	l_dump_stack(JAG_PARSER_CONTEXT(parser_context));
//
//	jgi_parser_context_dump_stack_with_focus(parser_context);
//
//
//	int focus_index = jgi_parser_context_get_focus_index(parser_context);
//	CatArrayWo *e_token_array = jgi_parser_context_get_token_array(parser_context);
//	JagToken *token_at_focus = (JagToken *) cat_array_wo_get(e_token_array, focus_index);
//
//	if (token_at_focus) {
//		switch(token_at_focus->sym) {
//			case JAG_SYMBOL_TERM_IDENTIFIER :
//			case JAG_SYMBOL_TERM_DOT : {
//				l_enlist_identifier(request);
//			} break;
////				l_enlist_identifier(request);
////			} break;
//			default : {
//				cat_log_warn("symbol not handled yet");
//			} break;
//		}
//	}
//
//	cat_unref_ptr(jav_parser);
//	cat_unref_ptr(java_scanner);
//	cat_unref_ptr(scanner);
//}
//
//
//
//struct _EditorCompletionsForShow {
//	ElkEditor *editor;
//	CatArrayWo *a_completion_entries;
//};
//
//
//typedef struct _EditorCompletionsForShow EditorCompletionsForShow;
//
//static gboolean show_completions_cb(gpointer *show_as_object) {
//#ifdef DO_GDK_THREADS
//	gdk_threads_enter();
//#endif
//	EditorCompletionsForShow *show = (EditorCompletionsForShow *) show_as_object;
//	elk_editor_show_auto_completion_popup(show->editor, show->a_completion_entries);
////	cat_unref_ptr(show->completion_entries);
//	cat_unref_ptr(show->editor);
//	g_free(show);
//#ifdef DO_GDK_THREADS
//	gdk_threads_leave();
//#endif
//	return FALSE;
//}
//
//
//
//static void l_run_request(WorRequest *wrequest) {
//	JagAutoCompleteRequest *request = JAG_AUTO_COMPLETE_REQUEST(wrequest);
//	l_enlist(request);
//
//	JagAutoCompleteRequestPrivate *priv = JAG_AUTO_COMPLETE_REQUEST_GET_PRIVATE(request);
//
//	elk_editor_enlist_template_completions(priv->elk_editor, priv->e_completions);
//
//	CatArrayWo *a_completion_entries = cat_array_wo_anchor(priv->e_completions, 0);
//
//	EditorCompletionsForShow *show = g_new(EditorCompletionsForShow, 1);
//	show->a_completion_entries = a_completion_entries;
//	show->editor = cat_ref_ptr(priv->elk_editor);
//	g_idle_add((GSourceFunc) show_completions_cb, (gpointer) show);
//}
//
//
//
//
//
