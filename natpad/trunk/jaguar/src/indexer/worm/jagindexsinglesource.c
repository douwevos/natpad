/*
   File:    jagindexsinglesource.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 5, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "jagindexsinglesource.h"
#include "../../moose/srcfile/jagsrcfilecontentwo.h"
#include "../source/jgiparsercontext.h"
#include <jaguarparser.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagIndexSingleSource"
#include <logging/catlog.h>

struct _JagIndexSingleSourcePrivate {
	MooIdPath *jag_source_id_path;
	MooService *moo_service;
};


G_DEFINE_TYPE(JagIndexSingleSource, jag_index_single_source, WOR_TYPE_REQUEST)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void jag_index_single_source_class_init(JagIndexSingleSourceClass *clazz) {
	g_type_class_add_private(clazz, sizeof(JagIndexSingleSourcePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *worm_clazz = WOR_REQUEST_CLASS(clazz);
	worm_clazz->runRequest = l_run_request;
}

static void jag_index_single_source_init(JagIndexSingleSource *instance) {
	JagIndexSingleSourcePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_INDEX_SINGLE_SOURCE, JagIndexSingleSourcePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagIndexSingleSource *instance = JAG_INDEX_SINGLE_SOURCE(object);
	JagIndexSingleSourcePrivate *priv = instance->priv;
	cat_unref_ptr(priv->jag_source_id_path);
	cat_unref_ptr(priv->moo_service);
	G_OBJECT_CLASS(jag_index_single_source_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_index_single_source_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagIndexSingleSource *jag_index_single_source_new(MooService *moo_service, MooIdPath *jag_source_id_path) {
	JagIndexSingleSource *result = g_object_new(JAG_TYPE_INDEX_SINGLE_SOURCE, NULL);
	cat_ref_anounce(result);
	JagIndexSingleSourcePrivate *priv = result->priv;
	wor_request_construct((WorRequest *) result);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->jag_source_id_path = cat_ref_sink_ptr(jag_source_id_path);
	return result;
}

static void l_populate_model(JagIndexSingleSource *index_ss, MooNodeWo *e_main, JagPJCCompilationUnit *compilation_unit) {

	JagIndexSingleSourcePrivate *priv = JAG_INDEX_SINGLE_SOURCE_GET_PRIVATE(index_ss);
	VipISequence *moo_sequence = (VipISequence *) priv->moo_service;

//	switch(JAG_TOKEN(ast_token)->sym) {
//	case JAG_SYMBOL_NONTERM_CLASS_HEADER : {
//		JagAstToken *ast_class_name_token = JAG_AST_TOKEN(cat_array_wo_get(ast_token->e_children, 2));
//		JagToken *token = JAG_TOKEN(ast_class_name_token);
//		if (token->is_terminal && (CAT_IS_STRING_WO(token->value))) {
//			CatStringWo *token_text = CAT_STRING_WO(token->value);
//			MooNodeWo *e_class_node = moo_node_wo_new(vip_isequence_next(priv->moo_sequence), cat_ref_ptr(token_text), NULL, NULL, 0);
//			moo_node_wo_append_child(priv->e_main, e_class_node);
//			cat_unref_ptr(e_class_node);
//		}
//	} break;
//	}

	CatIIterator *defiter = cat_array_wo_iterator(compilation_unit->defs);
	while(cat_iiterator_has_next(defiter)) {
		GObject *def = cat_iiterator_next(defiter);
		if (JAGP_IS_JCCLASS_DECL(def)) {
			JagPJCClassDecl *classdcl = JAGP_JCCLASS_DECL(def);
			if ((classdcl->modifiers->flags & JAGP_FLAG_INTERFACE)!=0) {
				continue;
			}
			CatStringWo *token_text = jagp_name_get_string(classdcl->name);
			MooNodeWo *e_class_node = moo_node_wo_new(vip_isequence_next(moo_sequence), cat_ref_ptr(token_text), NULL, NULL, 0);
			moo_node_wo_append_child(e_main, e_class_node);
			cat_unref_ptr(e_class_node);
		}
	}
	cat_unref_ptr(defiter);

}



static void l_run_request(WorRequest *request) {
	JagIndexSingleSource *index_ss = JAG_INDEX_SINGLE_SOURCE(request);
	JagIndexSingleSourcePrivate *priv = JAG_INDEX_SINGLE_SOURCE_GET_PRIVATE(request);
	cat_log_debug("indexing source:%o", priv->jag_source_id_path);


	MooNodeWo *root_node = moo_service_get_root_node_ref(priv->moo_service);

	MooNodeWo *jag_source_node_wo = moo_id_path_get_tail(priv->jag_source_id_path, root_node);
	if (jag_source_node_wo!=NULL) {
		cat_log_debug("jag_source_node_wo=%o", jag_source_node_wo);


//		MooResourceContentWo *res_content = (MooResourceContentWo *) moo_node_wo_get_content(jag_source_node_wo, moo_resource_content_wo_key());
//		moo_resource_content_wo_get_version(res_content);
		JagSrcfileContentWo *jag_sourcefile_content = (JagSrcfileContentWo *) moo_node_wo_get_content(jag_source_node_wo, jag_srcfile_content_wo_key());

		JagIndexSourceContent *index_context = jag_srcfile_content_wo_get_index_context(jag_sourcefile_content);
		int srcfile_content_wo_version = jag_srcfile_content_wo_get_version(jag_sourcefile_content);
		if (!jag_index_source_content_need_to_update(index_context, srcfile_content_wo_version)) {
			cat_unref_ptr(root_node);
			return;
		}

		VipIFile *file = jag_srcfile_content_wo_get_file(jag_sourcefile_content);
		CatStringWo *a_name = vip_iresource_get_name(VIP_IRESOURCE(file));
		cat_log_debug("name=%o", a_name);
		if (cat_string_wo_endswith_chars_len(a_name, ".java", 5)) {
			CatIInputStream *in_stream = vip_ifile_open_input_stream(file);
			CatUtf8InputStreamScanner *scanner = cat_utf8_input_stream_scanner_new(in_stream);

			JagPNames *names = jagp_names_new();
			JagPTokenizer *tokenizer = jagp_tokenizer_new(scanner, names);
			JagPILexer *lexer = (JagPILexer *) jagp_lexer_impl_new(tokenizer);
			JagPParser *parser = jagp_parser_new(lexer, names);
			jagp_parser_run(parser);

			JagPJCCompilationUnit *compilation_unit = jagp_parser_get_compilation_unit(parser);
//			jagp_jctree_dump((JagPJCTree *) compilation_unit, cat_string_wo_new());

			CatArrayWo *tokens = jagp_lexer_impl_get_all_tokens((JagPLexerImpl *) lexer);




//			// TODO scanner should convert LFCRs and CRLFs to LFs otherwise rows will be calculated incorrectly
//			JagScanner *jag_scanner =  jag_scanner_new((CatIUtf8Scanner *) scanner);
//			jag_scanner->create_ast_tokens = TRUE;
//			jag_scanner->filter_non_parsable = TRUE;
//			JgiParserContext *context = jgi_parser_context_new((JagIScanner *) jag_scanner, (VipISequence *) priv->moo_service);
//			JagParser *cupParser = jag_parser_new((JagIScanner *) jag_scanner);
//			JagParserBase *cupBase = JAG_PARSER_BASE(cupParser);
//			JagToken *token = jag_parser_base_parse(cupBase, (JagParserContext *) context);
//			jag_token_dump(token);
//			cat_log_debug("token=%o", token);

//			MooNodeWo *sourcefile_root_node = jgi_parser_context_get_root_node(context);

			VipISequence *moo_sequence = (VipISequence *) priv->moo_service;
			MooNodeWo *e_main = moo_node_wo_new(vip_isequence_next(moo_sequence), NULL, NULL, NULL, 0);

			l_populate_model(index_ss, e_main, compilation_unit);
			MooNodeWo *sourcefile_root_node = e_main;

			MooTransaction *tx = moo_service_create_transaction((GObject *) request, priv->moo_service);
			while(moo_transaction_retry(tx)) {
				moo_transaction_begin(tx);
				MooNodeWo *root_node = moo_transaction_get_tx_root_node(tx);
				MooNodeWo *e_root_node = moo_node_wo_ensure_editable(root_node, NULL);
				MooNodeWo *e_jag_source_node_wo = moo_id_path_create_editable_node(priv->jag_source_id_path, e_root_node);
				if (e_jag_source_node_wo!=NULL) {
					if (jag_srcfile_content_wo_get_version(jag_sourcefile_content)>srcfile_content_wo_version) {
						cat_unref_ptr(e_root_node);
						break;
					}
					MooNodeListWo *e_node_list = moo_node_wo_get_editable_children(e_jag_source_node_wo);
					int count = moo_node_list_wo_count(e_node_list);
					while(count>0) {
						count--;
						moo_node_list_wo_remove(e_node_list, count);
					}

					count = moo_node_wo_child_count(sourcefile_root_node);
					int idx;
					for(idx=0; idx<count; idx++) {
						MooNodeWo *child = moo_node_wo_child_at(sourcefile_root_node, idx);
						moo_node_list_wo_append(e_node_list, child);
					}

					if (moo_transaction_commit(tx, e_root_node)) {
						break;
					}
				} else {
					cat_unref_ptr(e_root_node);
					break;
				}
			}

//			cat_unref_ptr(cupParser);
//			cat_unref_ptr(context);
//			cat_unref_ptr(jag_scanner);
			cat_unref_ptr(tx);
			cat_unref_ptr(scanner);
			cat_unref_ptr(in_stream);

		}
	}
	cat_unref_ptr(root_node);

}

