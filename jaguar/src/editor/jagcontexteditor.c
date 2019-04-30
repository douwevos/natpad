/*
   File:    jagcontexteditor.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Oct 19, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "jagcontexteditor.h"
#include "jageditorpanel.h"
#include "../indexer/jgiastutil.h"
#include "../indexer/model/jgitokenrange.h"
#include "../moose/library/jaglibrarycontentwo.h"
#include "../moose/srcfolder/jagsrcfoldercontentwo.h"
#include "../moose/srcfile/jagsrcfilecontentwo.h"
#include "../moose/package/jagpackagecontent.h"
#include <jaguarparser.h>
#include <elk.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagContextEditor"
#include <logging/catlog.h>

struct _JagContextEditorPrivate {
	JagIndexer *indexer;
	MooNodeWo *moo_root_node;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_ac_content_provider_iface_init(DraIAcContentProviderInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagContextEditor, jag_context_editor, DRA_TYPE_CONTEXT_EDITOR,
		G_ADD_PRIVATE(JagContextEditor)
		G_IMPLEMENT_INTERFACE(DRA_TYPE_IAC_CONTENT_PROVIDER, l_ac_content_provider_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void l_open_declaration(DraContextEditor *context_editor);

static void jag_context_editor_class_init(JagContextEditorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraContextEditorClass *context_class = DRA_CONTEXT_EDITOR_CLASS(clazz);
	context_class->openDeclaration = l_open_declaration;
}

static void jag_context_editor_init(JagContextEditor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagContextEditor *instance = JAG_CONTEXT_EDITOR(object);
	JagContextEditorPrivate *priv = jag_context_editor_get_instance_private(instance);
	cat_unref_ptr(priv->indexer);
	G_OBJECT_CLASS(jag_context_editor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_context_editor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagContextEditor *jag_context_editor_new(DraEditorPanel *editor_panel, JagIndexer *indexer) {
	JagContextEditor *result = g_object_new(JAG_TYPE_CONTEXT_EDITOR, NULL);
	cat_ref_anounce(result);
	JagContextEditorPrivate *priv = jag_context_editor_get_instance_private(result);
	dra_context_editor_construct((DraContextEditor *) result, editor_panel);
	priv->indexer = cat_ref_ptr(indexer);
	priv->moo_root_node = NULL;
	return result;
}



static MooNodeWo *l_get_moose_root_node(JagContextEditor *context_editor) {
	JagContextEditorPrivate *priv = jag_context_editor_get_instance_private(context_editor);

	if (priv->moo_root_node==NULL) {
		JagIndexer *indexer = priv->indexer;
		if (indexer) {
			MooService *moo_service = jag_indexer_get_moo_service(indexer);
			priv->moo_root_node = moo_service_get_root_node_ref(moo_service);
		}
	}
	return priv->moo_root_node;
}


static MooNodeWo *l_get_module_moo_node(JagContextEditor *context_editor, DraEditor *editor) {
	MooNodeWo *root_node = l_get_moose_root_node(context_editor);
	if (root_node==NULL) {
		return NULL;
	}

	DraEditorPanel *editor_panel = dra_editor_get_panel(editor);

	MooNodeWo *moo_node = elk_editor_panel_get_moose_node((ElkEditorPanel *) editor_panel);

	MooIdPath *path = moo_node_wo_find_path(root_node, moo_node_wo_get_unique_id(moo_node));
	MooNodeWo *module_node = moo_id_path_get_at(path, root_node, 0);

	return module_node;
}

static CatArrayWo *l_get_moose_src_folder_nodes(JagContextEditor *context_editor, DraAcContext *ac_context) {
	MooNodeWo *module_node = l_get_module_moo_node(context_editor, (DraEditor *) dra_ac_context_get_editor(ac_context));
	cat_log_debug("module_node=%o", module_node);
	if (module_node==NULL) {
		return NULL;
	}

	CatArrayWo *e_result = cat_array_wo_new();
	int count = moo_node_wo_child_count(module_node);
	for(count--; count>=0; count--) {
		MooNodeWo *child_node = moo_node_wo_child_at(module_node, count);
		cat_log_debug("child_node=%o", child_node);
		JagSrcFolderContentWo *content = (JagSrcFolderContentWo *) moo_node_wo_get_content(child_node, jag_src_folder_content_wo_key());
		if (content == NULL) {
			continue;
		}
		cat_array_wo_append(e_result, (GObject *) child_node);
	}
	return e_result;
}


static CatArrayWo *l_get_moose_library_nodes(JagContextEditor *context_editor, DraAcContext *ac_context) {
	MooNodeWo *module_node = l_get_module_moo_node(context_editor, (DraEditor *) dra_ac_context_get_editor(ac_context));
	cat_log_debug("module_node=%o", module_node);
	if (module_node==NULL) {
		return NULL;
	}

	CatArrayWo *e_result = cat_array_wo_new();
	int count = moo_node_wo_child_count(module_node);
	for(count--; count>=0; count--) {
		MooNodeWo *child_node = moo_node_wo_child_at(module_node, count);
		cat_log_debug("child_node=%o", child_node);
		JagLibraryContentWo *content = (JagLibraryContentWo *) moo_node_wo_get_content(child_node, jag_library_content_wo_key());
		if (content == NULL) {
			continue;
		}
		cat_array_wo_append(e_result, (GObject *) child_node);
	}
	return e_result;
}



static CatStringWo *l_create_fqn_name(JgiTokenRange *name_token_range, int row, int column) {
	CatStringWo *e_result = cat_string_wo_new();
	CatIIterator *iter = jgi_token_range_iterator(name_token_range);
	while(cat_iiterator_has_next(iter)) {
		JagPToken *token = (JagPToken *) cat_iiterator_next(iter);
		cat_log_error("token=%O", token);
//		cat_log_debug("sym=%d, state=%d, text=%s, l/r=%d#%d/%d#%d", token->kind, token->parse_state, token->symbol_text, token->left, token->left_row, token->right, token->right_row);
		CatStringWo *a_token_text = NULL;
		if (token->value) {
			JagPName *token_text = JAGP_NAME(token->value);
			a_token_text = jagp_name_get_string(token_text);
		}
		cat_log_debug("a_token_text=%o", a_token_text);
		if (a_token_text==NULL) {
			if (token->kind==JAGP_KIND_DOT) {
				a_token_text = cat_string_wo_new_with(".");
			} else {
				cat_ref_ptr(a_token_text);
			}
		} else {
			cat_ref_ptr(a_token_text);
		}

		if (a_token_text!=NULL) {
			int l_col, r_col;
			long long l_row, r_row;
			jagp_cursor_values(token->cur_start, &l_row, &l_col);
			jagp_cursor_values(token->cur_end, &r_row, &r_col);
			if (row<0 || row>r_row || (row==r_row && column>=r_col)) {
				cat_string_wo_append(e_result, a_token_text);
			} else {
				int sublen = column - l_col;
				CatStringWo *e_subpart = cat_string_wo_new_sub(a_token_text, 0, sublen);
				e_subpart = cat_string_wo_anchor(e_subpart, 0);
				cat_string_wo_append(e_result, e_subpart);
				cat_unref_ptr(e_subpart);
			}
			cat_unref_ptr(a_token_text);
		}

	}
	cat_unref_ptr(iter);
	cat_log_debug("e_result %o", e_result);
	return e_result;
}

static void l_enlist_import(JagContextEditor *context_editor, DraAcContext *ac_context, JgiTokenRange *name_token_range, CatArrayWo *tokens, int focus_index) {
	int import_token_index = jgi_token_range_get_first_index(name_token_range)-1;
	int semi_index = -1;

	JagPToken *ast_token_at_focus = (JagPToken *) cat_array_wo_get(tokens, focus_index);

	if (name_token_range) {
		CatArrayWo *e_package_name_list = cat_array_wo_new();
		long long int row = 0;
		int column = 0;
		dra_ac_context_get_cursor_location(ac_context, &column, &row);


		CatStringWo *e_qname_as_text = l_create_fqn_name(name_token_range, row, column);
		CatStringWo *a_full_name = cat_string_wo_anchor(e_qname_as_text, 0);
		cat_log_error("IMPORT %o ... SEMI", a_full_name);

		/* source folders */
		CatArrayWo *src_folder_nodes = l_get_moose_src_folder_nodes(context_editor, ac_context);
		if (src_folder_nodes) {
			CatIIterator *iter = cat_array_wo_iterator(src_folder_nodes);
			while(cat_iiterator_has_next(iter)) {
				MooNodeWo *library_node = (MooNodeWo *) cat_iiterator_next(iter);
				cat_log_debug("src_folder_node=%o", library_node);
				int src_folder_count = moo_node_wo_child_count(library_node);
				for(src_folder_count--; src_folder_count>=0; src_folder_count--) {
					MooNodeWo *jag_node = moo_node_wo_child_at(library_node, src_folder_count);
					cat_log_debug("jag_node=%o", jag_node);
					MooIContent *pack_cont = moo_node_wo_get_content(jag_node, jag_package_content_key());
					if (pack_cont) {
						CatStringWo *a_pck_name = moo_node_wo_get_name(jag_node);
						cat_log_trace("pck_name=%o", a_pck_name);
//						if (cat_string_wo_length(a_pck_name)>last_dot_idx) {
//							if (cat_string_wo_beginswith(a_pck_name, a_full_name)) {
//								cat_array_wo_append(e_package_name_list, (GObject *) a_pck_name);
//							}
//						}
						int pack_count = moo_node_wo_child_count(jag_node);
						for(pack_count--; pack_count>=0; pack_count--) {
							MooNodeWo *jag_sub_node = moo_node_wo_child_at(jag_node, pack_count);
							MooIContent *srcfile_cont = moo_node_wo_get_content(jag_sub_node, jag_srcfile_content_wo_key());
							if (srcfile_cont) {
								CatStringWo *e_nn = cat_string_wo_new();
								cat_string_wo_append(e_nn, a_pck_name);
								cat_string_wo_append_char(e_nn, '.');
								CatStringWo *sub_name = moo_node_wo_get_name(jag_sub_node);
								if (cat_string_wo_endswith_chars_len(sub_name, ".java", 5)) {
									cat_string_wo_append_chars_len(e_nn, cat_string_wo_getchars(sub_name), cat_string_wo_length(sub_name)-5);
								} else if (cat_string_wo_endswith_chars_len(sub_name, ".class", 6)) {
									cat_string_wo_append_chars_len(e_nn, cat_string_wo_getchars(sub_name), cat_string_wo_length(sub_name)-6);
								} else {
									cat_unref_ptr(e_nn);
								}
								if (e_nn) {
									CatStringWo *a_tesn = cat_string_wo_anchor(e_nn, 0);
									cat_log_trace("tesn=%O, a_full_name=%O", a_tesn, a_full_name);
									if (cat_string_wo_beginswith(a_tesn, a_full_name)) {
										cat_array_wo_append(e_package_name_list, (GObject *) a_tesn);
									}
									cat_unref_ptr(a_tesn);
								}
							}
						}

					} else {
						MooIContent *srcfile_cont = moo_node_wo_get_content(jag_node, jag_srcfile_content_wo_key());
						if (srcfile_cont) {

						}
					}
				}
			}
			cat_unref_ptr(iter);
		}


		/* jars */
		CatArrayWo *a_library_nodes = l_get_moose_library_nodes(context_editor, ac_context);
		if (a_library_nodes) {
			CatIIterator *iter = cat_array_wo_iterator(a_library_nodes);
			while(cat_iiterator_has_next(iter)) {
				MooNodeWo *src_folder_node = (MooNodeWo *) cat_iiterator_next(iter);
				cat_log_debug("src_folder_node=%o", src_folder_node);
				int src_folder_count = moo_node_wo_child_count(src_folder_node);
				for(src_folder_count--; src_folder_count>=0; src_folder_count--) {
					MooNodeWo *jag_node = moo_node_wo_child_at(src_folder_node, src_folder_count);
					cat_log_debug("jag_node=%o", jag_node);
					MooIContent *pack_cont = moo_node_wo_get_content(jag_node, jag_package_content_key());
					if (pack_cont) {
						CatStringWo *a_pck_name = moo_node_wo_get_name(jag_node);
						cat_log_trace("pck_name=%o", a_pck_name);
//						if (cat_string_wo_length(a_pck_name)>last_dot_idx) {
//							if (cat_string_wo_beginswith(a_pck_name, a_full_name)) {
//								cat_array_wo_append(e_package_name_list, (GObject *) a_pck_name);
//							}
//						} else {
							int pack_count = moo_node_wo_child_count(jag_node);
							for(pack_count--; pack_count>=0; pack_count--) {
								MooNodeWo *jag_sub_node = moo_node_wo_child_at(jag_node, pack_count);
								MooIContent *srcfile_cont = moo_node_wo_get_content(jag_sub_node, jag_srcfile_content_wo_key());
								if (srcfile_cont) {
									CatStringWo *e_nn = cat_string_wo_new();
									cat_string_wo_append(e_nn, a_pck_name);
									cat_string_wo_append_char(e_nn, '.');

									CatStringWo *sub_name = moo_node_wo_get_name(jag_sub_node);
									if (cat_string_wo_endswith_chars_len(sub_name, ".java", 5)) {
										cat_string_wo_append_chars_len(e_nn, cat_string_wo_getchars(sub_name), cat_string_wo_length(sub_name)-5);
									} else if (cat_string_wo_endswith_chars_len(sub_name, ".class", 6)) {
										cat_string_wo_append_chars_len(e_nn, cat_string_wo_getchars(sub_name), cat_string_wo_length(sub_name)-6);
									} else {
										cat_unref_ptr(e_nn);
									}
									if (e_nn) {
										CatStringWo *a_tesn = cat_string_wo_anchor(e_nn, 0);
										cat_log_trace("tesn=%o", a_tesn);
										if (cat_string_wo_beginswith(a_tesn, a_full_name)) {
											cat_array_wo_append(e_package_name_list, (GObject *) a_tesn);
										}
										cat_unref_ptr(a_tesn);
									}
								}
							}
//						}
					} else {
						MooIContent *srcfile_cont = moo_node_wo_get_content(jag_node, jag_srcfile_content_wo_key());
						if (srcfile_cont) {

						}
					}
				}
			}
			cat_unref_ptr(iter);
		}



		semi_index = focus_index+1;
		long long trow=0;
		int tcol = 0;
		jagp_cursor_values(ast_token_at_focus->cur_start, &trow, &tcol);
		while(semi_index<cat_array_wo_size(tokens)) {
			JagPToken *semi_token = (JagPToken *) cat_array_wo_get(tokens, semi_index);
			if (semi_token->kind==JAGP_KIND_SEMI) {
				break;
			}
			long long row;
			int col;
			jagp_cursor_values(semi_token->cur_start, &row, &col);
			if (row!=trow) {
				semi_index--;
				break;
			}
			semi_index++;
		}
		if (semi_index>=cat_array_wo_size(tokens)) {
			semi_index = cat_array_wo_size(tokens)-1;
		}


		JagPToken *semi_token = (JagPToken *) cat_array_wo_get(tokens, semi_index);
		JagPToken *package_token = (JagPToken *) cat_array_wo_get(tokens, import_token_index);

		int st_col, pt_col;
		long long st_row, pt_row;
		jagp_cursor_values(package_token->cur_start, &pt_row, &pt_col);
		jagp_cursor_values(semi_token->cur_end, &st_row, &st_col);

		ChaCursorWo *cursor_start = dra_ac_context_create_cursor(ac_context, pt_col, pt_row);
		ChaCursorWo *cursor_end = dra_ac_context_create_cursor(ac_context, st_col, st_row);

		CatIIterator *iter = cat_array_wo_iterator(e_package_name_list);
		cat_log_debug("e_package_name_list.size=%d", cat_array_wo_size(e_package_name_list));
		while(cat_iiterator_has_next(iter)) {
			CatStringWo *a_pkg_name = (CatStringWo *) cat_iiterator_next(iter);
			CatStringWo *e_replace = cat_string_wo_new_with("import ");
			cat_string_wo_append(e_replace, a_pkg_name);
			cat_string_wo_append_chars(e_replace, ";");
			cat_log_debug("e_replace=%O", e_replace);
			CatStringWo *a_full_statement = cat_string_wo_anchor(e_replace, 0);
			DraAcReplaceEntry *entry = dra_ac_replace_entry_new(a_pkg_name, a_full_statement);
			dra_ac_add_entry(ac_context, (DraAcEntry *) entry);
			cat_unref_ptr(entry);
			cat_unref_ptr(a_full_statement);
//				priv->e_completions_available = TRUE;
//			cat_unref_ptr(a_pkg_name);
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(cursor_start);
		cat_unref_ptr(cursor_end);
		cat_unref_ptr(e_package_name_list);
	}
}

static void l_enlist_identifier(JagContextEditor *context_editor, DraAcContext *ac_context, JagPParser *parser, CatArrayWo *tokens, int focus_index) {
	JgiTokenRange *token_range = jgi_ast_util_extract_name_root(tokens, focus_index);
	jgi_token_range_dump(token_range);
	int prec_index = jgi_token_range_get_first_index(token_range)-1;
	if (prec_index<0) {
		return;
	}
	JagPToken *prec_token = (JagPToken *) cat_array_wo_get(tokens, prec_index);
	switch(prec_token->kind) {
		case JAGP_KIND_IMPORT : {
			l_enlist_import(context_editor, ac_context, token_range, tokens, focus_index);
			cat_log_warn("would enlist imports now");
		} break;
		default : {

		}
	}

}

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

int l_index_of_token(CatArrayWo *tokens, long long row, int column) {
	int idx;
	int count = cat_array_wo_size(tokens);
	long long row_start, row_end;
	int column_start, column_end;
	for(idx=0; idx<count; idx++) {
		JagPToken *token = (JagPToken *) cat_array_wo_get(tokens, idx);
		jagp_cursor_values(token->cur_start, &row_start, &column_start);
		if (row_start>row) {
			return -1;
		} else if (row_start==row && column<column_start) {
			continue;
		}
		jagp_cursor_values(token->cur_end, &row_end, &column_end);
		if (row_end<row) {
			continue;
		} else if (row_end==row && column>column_end) {
			continue;
		}
		return idx;
	}
	return -1;
}

static void l_enlist(JagContextEditor *context_editor, DraAcContext *ac_context) {
//	priv->indexer_project = l_get_indexer_project(request);

	ChaRevisionReader *revision_reader = dra_ac_context_create_revision_reader(ac_context);
	cha_revision_reader_set_forced_line_end(revision_reader, CHA_LINE_END_LF);

	CatIUtf8Scanner *scanner = CAT_IUTF8_SCANNER(revision_reader);
	JagPNames *names = jagp_names_new();
	JagPTokenizer *tokenizer = jagp_tokenizer_new(scanner, names);
	JagPILexer *lexer = (JagPILexer *) jagp_lexer_impl_new(tokenizer);
	JagPParser *parser = jagp_parser_new(lexer, names);
	jagp_parser_run(parser);

	JagPJCCompilationUnit *compilation_unit = jagp_parser_get_compilation_unit(parser);
	cat_log_on_debug({
		jagp_jctree_dump((JagPJCTree *) compilation_unit, cat_string_wo_new());
	});

	CatArrayWo *tokens = jagp_lexer_impl_get_all_tokens((JagPLexerImpl *) lexer);

	long long int row = 0;
	int column = 0;
	dra_ac_context_get_cursor_location(ac_context, &column, &row);
	int focus_index = l_index_of_token(tokens, row, column);
	if (focus_index>=0) {
		JagPToken *token_at_focus = (JagPToken *) cat_array_wo_get(tokens, focus_index);
		if (token_at_focus) {
			switch(token_at_focus->kind) {
				case JAGP_KIND_IDENTIFIER :
				case JAGP_KIND_DOT : {
					l_enlist_identifier(context_editor, ac_context, parser, tokens, focus_index);
				} break;
		////				l_enlist_identifier(request);
		////			} break;
					default : {
		//				cat_log_warn("symbol not handled yet");
					} break;
				}
		}
	}
	cat_unref_ptr(parser);
	cat_unref_ptr(names);
	cat_unref_ptr(tokenizer);
	cat_unref_ptr(lexer);


//	JagScanner *java_scanner = jag_scanner_new(scanner);
//	java_scanner->filter_non_parsable = TRUE;
//	java_scanner->create_ast_tokens = TRUE;
//	JagParser *jav_parser = jag_parser_new(JAG_ISCANNER(java_scanner));
//
//	long long int row = 0;
//	int column = 0;
//	dra_ac_context_get_cursor_location(ac_context, &column, &row);
//
//	cat_log_debug("for row=%d column=%d", row, column);
//
//
//	MooService *moo_service = jag_indexer_get_moo_service(priv->indexer);
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
//	int focus_index = jgi_parser_context_get_focus_index(parser_context);
//	CatArrayWo *e_token_array = jgi_parser_context_get_token_array(parser_context);
//	JagToken *token_at_focus = (JagToken *) cat_array_wo_get(e_token_array, focus_index);
//
//	if (token_at_focus) {
//		switch(token_at_focus->sym) {
//			case JAG_SYMBOL_TERM_IDENTIFIER :
//			case JAG_SYMBOL_TERM_DOT : {
//				l_enlist_identifier(context_editor, ac_context);
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
}


static void l_open_declaration(DraContextEditor *context_editor) {
//	DraEditorPanel *editor_panel = (JagEditorPanel *) dra_context_editor_get_panel(context_editor);
}

/********************* start DraIAcContentProvider implementation *********************/

static void l_enlist_content(DraIAcContentProvider *content_provider, DraAcContext *ac_context) {
	l_enlist((JagContextEditor *) content_provider, ac_context);
}


static void l_ac_content_provider_iface_init(DraIAcContentProviderInterface *iface) {
	iface->enlistContent = l_enlist_content;
}

/********************* end DraIAcContentProvider implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagContextEditor *instance = JAG_CONTEXT_EDITOR(self);
	JagContextEditorPrivate *priv = jag_context_editor_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p indexer=%p]", iname, self, priv->indexer);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
