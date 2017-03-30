/*
   File:    groedparser.c
   Project: gross-editor
   Author:  Douwe Vos
   Date:    Nov 20, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "groedparser.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroEdParser"
#include <logging/catlog.h>

struct _GroEdParserPrivate {
	GroScanner *scanner;
	GroRunITokenFactory *token_factory;
	GroRunModel *model;
	CatArrayWo *token_cash;
	CatArrayWo *messages;
	CatHashMapWo *term_names;
	CatHashMapWo *non_term_names;
	GroAstSpec *ast_specification;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_message_iface_init(GroIMessageHandlerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroEdParser, groed_parser, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroEdParser)
		G_IMPLEMENT_INTERFACE(GRO_TYPE_IMESSAGE_HANDLER, l_message_iface_init)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void groed_parser_class_init(GroEdParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void groed_parser_init(GroEdParser *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroEdParser *instance = GROED_PARSER(object);
	GroEdParserPrivate *priv = groed_parser_get_instance_private(instance);
	cat_unref_ptr(priv->model);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->scanner);
	cat_unref_ptr(priv->token_cash);
	cat_unref_ptr(priv->messages);
	cat_unref_ptr(priv->term_names);
	cat_unref_ptr(priv->non_term_names);
	cat_unref_ptr(priv->ast_specification);
	G_OBJECT_CLASS(groed_parser_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(groed_parser_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroEdParser *groed_parser_new(GroScanner *scanner, GroRunITokenFactory *token_factory, GroRunModel *model) {
	GroEdParser *result = g_object_new(GROED_TYPE_PARSER, NULL);
	cat_ref_anounce(result);
	GroEdParserPrivate *priv = groed_parser_get_instance_private(result);
	priv->scanner = cat_ref_ptr(scanner);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->model = cat_ref_ptr(model);
	priv->token_cash = cat_array_wo_new();
	priv->messages = cat_array_wo_new();
	priv->term_names = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	priv->non_term_names = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	priv->ast_specification = NULL;
	return result;
}


CatArrayWo *groed_parser_get_token_cash(GroEdParser *parser) {
	GroEdParserPrivate *priv = groed_parser_get_instance_private(parser);
	return priv->token_cash;
}

CatHashMapWo *groed_parser_get_non_terminal_map(GroEdParser *parser) {
	GroEdParserPrivate *priv = groed_parser_get_instance_private(parser);
	return priv->non_term_names;
}

CatArrayWo *groed_parser_get_messages(GroEdParser *parser) {
	GroEdParserPrivate *priv = groed_parser_get_instance_private(parser);
	return priv->messages;
}


static gint l_message_compare(gconstpointer a, gconstpointer b) {
	GroRunLocation *loc_a = grorun_message_get_location((GroRunMessage *) a);
	GroRunLocation *loc_b = grorun_message_get_location((GroRunMessage *) b);
	int col_a, col_b;
	long row_a, row_b;
	grorun_location_get_begin(loc_a, &col_a, &row_a);
	grorun_location_get_begin(loc_b, &col_b, &row_b);

	if (row_a<row_b) {
		return -1;
	} else if (row_a>row_b) {
		return 1;
	} else if (col_a<col_b) {
		return -1;
	} else if (col_a>col_b) {
		return 1;
	}
	return 0;
}

static void l_analyze_spec(GroEdParser *parser, GroAstSpec *spec);


GroAstSpec *groed_parser_run(GroEdParser *parser) {
	GroEdParserPrivate *priv = groed_parser_get_instance_private(parser);

	GroAstSpec *result = NULL;

	GroRunIScanner *scanner = GRORUN_ISCANNER(parser);

//	GroActions *actions = gro_actions_new();

	GroAstActions *actions = gro_ast_actions_new();

	GroRunContext *context = grorun_context_new(priv->token_factory, priv->model, GRORUN_IACTION_HANDLER(actions));
	grorun_parser_do_parse(context, scanner);

	GroRunFork *fork = grorun_context_accepted_fork_first(context);
	if (fork==NULL) {
		if (grorun_context_error_fork_count(context)>0) {
			fork = grorun_context_error_fork_at(context, 0);
		}
	}
	cat_log_debug("#############################################################++++ fork=%O", fork);
	if (fork) {


		GroRunStackEntry *entry = grorun_fork_get_entry(fork, 1);
		if (entry) {
			GroRunIToken *main_lah = grorun_stack_entry_get_lookahead(entry);
			cat_log_debug("*********** %O", main_lah);

			GObject *val = grorun_itoken_get_value(main_lah);
			cat_log_debug("*********** %O", val);
//			if (GRO_IS_BNF_SPEC(val)) {
//				result = (GroBnfSpec *) val;
//				cat_ref_ptr(result);
//				l_analyze_specification(parser, result);
//			} else
			if (GROAST_IS_SPEC(val)) {
				result = (GroAstSpec *) val;
				l_analyze_spec(parser, result);
				priv->ast_specification = result;
				cat_ref_ptr(result);

			}

		}

		GroRunMessage *messages = grorun_fork_get_messages(fork);
		cat_log_debug("messages=%O", messages);
		if (messages) {
			while(messages) {
				cat_array_wo_append(priv->messages, (GObject *) messages);
				messages = grorun_message_get_parent_message(messages);
			}
		}


		if (cat_array_wo_size(priv->messages)>0) {
			cat_array_wo_sort(priv->messages, (GCompareFunc) l_message_compare);
		}
	}


	while(TRUE) {
		GroRunIToken *token = grorun_iscanner_next((GroRunIScanner *) priv->scanner);

		if (token) {
			if (gro_scanner_is_eof(priv->scanner, token)) {
				cat_unref_ptr(token);
				break;
			}
			cat_array_wo_append(priv->token_cash, (GObject *) token);
			cat_unref_ptr(token);
		} else {
			break;
		}
	}

	cat_unref_ptr(context);

	return result;
}


static void l_symbol_marker(GroEdParser *parser, GroRunIToken *tk_part);

static void l_analyze_spec(GroEdParser *parser, GroAstSpec *spec) {
	GroEdParserPrivate *priv = groed_parser_get_instance_private(parser);

	GroPModelBuilder *builder = grop_model_builder_new(spec, (GroIMessageHandler *) parser);
	grop_model_builder_do_build(builder);


	/* analyze for unique terminal names, mark double declared names */

	if (groast_spec_tk_td(spec)!=NULL) {
		GroAstTerminalDeclarationList *decl_list = groast_spec_get_td(spec);
		cat_log_detail("decl_list=%O", decl_list);
		CatArrayWo *decl_enlisted = groast_terminal_declaration_list_enlist(decl_list);

		CatIIterator *iter = cat_array_wo_iterator(decl_enlisted);
		while(cat_iiterator_has_next(iter)) {
			GroRunIToken *tk_term_decl = (GroRunIToken *) cat_iiterator_next(iter);
			if (tk_term_decl==NULL) {
				cat_log_detail("tk_term_decl==NULL");
				continue;
			}
			GroAstTerminalDeclaration *term_decl = (GroAstTerminalDeclaration *) grorun_itoken_get_value(tk_term_decl);
			cat_log_detail("term_decl=%O", term_decl);
			CatArrayWo *term_enlisted = groast_terminal_declaration_enlist(term_decl);
			if (term_enlisted) {
				CatIIterator *titer = cat_array_wo_iterator(term_enlisted);
				while(cat_iiterator_has_next(titer)) {
					GroRunIToken *tk_term_name = (GroRunIToken *) cat_iiterator_next(titer);
					cat_log_detail("tk_term_name=%O", tk_term_name);
					CatStringWo *term_name = (CatStringWo *) grorun_itoken_get_value(tk_term_name);
					cat_log_detail("term_name=%O", term_name);
					GroRunIToken *tk_first_decl = (GroRunIToken *) cat_hash_map_wo_get(priv->term_names, term_name);
					if (tk_first_decl!=NULL) {
//						GroRunLocation *location = grorun_full_token_get_location((GroRunFullToken *) tk_term_name);
//						CatStringWo *msg_text = cat_string_wo_new_with("Symbol already defined");
//						GroRunMessage *message = grorun_message_new(NULL, msg_text, location);
//						cat_log_debug("message=%O", message);
//						cat_array_wo_append(priv->messages, (GObject *) message);
//						cat_unref_ptr(message);
					} else {
						cat_hash_map_wo_put(priv->term_names, (GObject *) term_name, (GObject *) tk_term_name);
					}
				}
				cat_unref_ptr(titer);
				cat_unref_ptr(term_enlisted);
			}
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(decl_enlisted);
	}


	/* analyze for unique non terminal names, mark double declared names */
	if (groast_spec_tk_pl(spec)!=NULL) {
		GroAstProductionDefinitionList *prod_def_list = groast_spec_get_pl(spec);
		CatArrayWo *prod_def_array = groast_production_definition_list_enlist(prod_def_list);
		CatIIterator *iter = cat_array_wo_iterator(prod_def_array);
		while(cat_iiterator_has_next(iter)) {
			GroRunIToken *tk_prod_def = (GroRunIToken *) cat_iiterator_next(iter);
			GroAstProductionDefinition *prod_def = (GroAstProductionDefinition *) grorun_itoken_get_value(tk_prod_def);
			GroRunIToken *tk_lhs = groast_production_definition_tk_lhsName(prod_def);
			CatStringWo *lhs = (CatStringWo *) grorun_itoken_get_value(tk_lhs);
			cat_log_detail("tk_lhs=%O", tk_lhs);
			GroRunIToken *tk_ent_decl = (GroRunIToken *) cat_hash_map_wo_get(priv->non_term_names, lhs);
			GroRunIToken *tk_et_decl = (GroRunIToken *) cat_hash_map_wo_get(priv->term_names, lhs);
			if (tk_ent_decl!=NULL || tk_et_decl!=NULL) {
//				GroRunLocation *location = grorun_full_token_get_location((GroRunFullToken *) tk_lhs);
//				GroRunMessage *message = grorun_message_new(NULL, cat_string_wo_new_with("Symbol already defined"), location);
//				cat_array_wo_append(priv->messages, (GObject *) message);
//				cat_log_debug("message=%O", message);
//				cat_unref_ptr(message);
			} else {
				cat_hash_map_wo_put(priv->non_term_names, (GObject *) lhs, (GObject *) tk_lhs);
			}
		}
		cat_unref_ptr(iter);


		/* Detect symbols in RHS of all production rules */
		iter = cat_array_wo_iterator(prod_def_array);
		while(cat_iiterator_has_next(iter)) {
			GroRunIToken *tk_prod_def = (GroRunIToken *) cat_iiterator_next(iter);
			GroAstProductionDefinition *prod_def = (GroAstProductionDefinition *) grorun_itoken_get_value(tk_prod_def);
			GroRunIToken *tk_rhs_rule_list = groast_production_definition_tk_ruleList(prod_def);
			l_symbol_marker(parser, tk_rhs_rule_list);
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(prod_def_array);
	}


	if (groast_spec_tk_tl(spec)!=NULL) {
		GroAstTypeDefinitionList *type_def_list = groast_spec_get_tl(spec);
		CatArrayWo *type_def_enlisted = groast_type_definition_list_enlist(type_def_list);
		CatIIterator *iter = cat_array_wo_iterator(type_def_enlisted);
		while(cat_iiterator_has_next(iter)) {
			GroRunIToken *tk_type_def = (GroRunIToken *) cat_iiterator_next(iter);
			GroAstTypeDefinition *type_def = (GroAstTypeDefinition *) grorun_itoken_get_value(tk_type_def);
			GroAstIdList *id_list = groast_type_definition_get_symids(type_def);
			if (id_list) {
				CatArrayWo *id_enlisted = groast_id_list_enlist(id_list);
				CatIIterator *id_iter = cat_array_wo_iterator(id_enlisted);
				while(cat_iiterator_has_next(id_iter)) {
					GroRunIToken *tk_sym_id = (GroRunIToken *) cat_iiterator_next(id_iter);
					CatStringWo *sym_id = (CatStringWo *) grorun_itoken_get_value(tk_sym_id);
					GroRunLeftRight lr = grorun_itoken_get_indexes(tk_sym_id);
					GroRunIToken *tk_leaf = (GroRunIToken *) cat_array_wo_get(priv->token_cash, lr.left);
					GObject *t = cat_hash_map_wo_get(priv->term_names, sym_id);
					cat_log_detail("in-term-names=%O", t);
					if (t!=NULL) {
						grorun_full_token_set_user_index((GroRunFullToken *) tk_leaf, GROED_SYM_MARKED_TERMINAL);
					} else {
						t = cat_hash_map_wo_get(priv->non_term_names, sym_id);
						if (t!=NULL) {
							grorun_full_token_set_user_index((GroRunFullToken *) tk_leaf, GROED_SYM_MARKED_NON_TERMINAL);
						} else {
							GroRunLocation *location = grorun_itoken_get_location(tk_leaf);
							GroRunMessage *message = grorun_message_new(NULL, cat_string_wo_new_with("Symbol undefined"), location);
							cat_array_wo_append(priv->messages, (GObject *) message);
							cat_unref_ptr(message);
						}
					}
				}
				cat_unref_ptr(id_iter);
				cat_unref_ptr(id_enlisted);
			}

		}
		cat_unref_ptr(iter);
		cat_unref_ptr(type_def_enlisted);
	}
}



static void l_symbol_marker(GroEdParser *parser, GroRunIToken *tk_part) {
	if (tk_part==NULL) {
		return;
	}
	GroEdParserPrivate *priv = groed_parser_get_instance_private(parser);
	GObject *part = grorun_itoken_get_value(tk_part);
	if (GROAST_IS_IRHS_ENLISTABLE(part)) {
		CatArrayWo *sub_parts = groast_irhs_enlistable_enlist((GroAstIRhsEnlistable *) part);
		CatIIterator *iter = cat_array_wo_iterator(sub_parts);
		cat_unref_ptr(sub_parts);
		while(cat_iiterator_has_next(iter)) {
			GroRunIToken *sub_part = (GroRunIToken *) cat_iiterator_next(iter);
			l_symbol_marker(parser, sub_part);
		}
		cat_unref_ptr(iter);
	} else if (GROAST_IS_SYMBOL_PART(part)) {
		GroAstSymbolPart *sym_part = (GroAstSymbolPart *) part;
		GroRunIToken *tk_sym_id = groast_symbol_part_tk_symName(sym_part);
		CatStringWo *sym_id = (CatStringWo *) grorun_itoken_get_value(tk_sym_id);
		if (sym_id==NULL) {
			return;
		}
		GroRunLeftRight lr = grorun_itoken_get_indexes(tk_sym_id);
		GroRunIToken *tk_leaf = (GroRunIToken *) cat_array_wo_get(priv->token_cash, lr.left);
		cat_log_detail("tk_leaf=%O", tk_leaf);
		if (tk_leaf!=NULL) {
			GObject *t = cat_hash_map_wo_get(priv->term_names, sym_id);
			cat_log_detail("in-term-names=%O", t);
			if (t!=NULL) {
				grorun_full_token_set_user_index((GroRunFullToken *) tk_leaf, GROED_SYM_MARKED_TERMINAL);
			} else {
				t = cat_hash_map_wo_get(priv->non_term_names, sym_id);
				if (t!=NULL) {
					grorun_full_token_set_user_index((GroRunFullToken *) tk_leaf, GROED_SYM_MARKED_NON_TERMINAL);
				} else {
					GroRunLocation *location = grorun_itoken_get_location(tk_leaf);
					GroRunMessage *message = grorun_message_new(NULL, cat_string_wo_new_with("Symbol undefined"), location);
					cat_array_wo_append(priv->messages, (GObject *) message);
					cat_unref_ptr(message);
				}
			}
		}
//	} else if (GRO_IS_BNF_RHS_UNARY_PART(part)) {
//		GroIBnfRhsRulePart *sub = gro_bnf_rhs_unary_part_get_part((GroBnfRhsUnaryPart *) part);
//		l_symbol_marker(parser, sub);
	} else {
//		cat_log_debug("tk_part=%O", tk_part);
//		cat_log_debug("part=%O", part);
	}
}




/********************* start GroRunIScanner implementation *********************/


static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	GroEdParser *parser = GROED_PARSER(self);
	GroEdParserPrivate *priv = groed_parser_get_instance_private(parser);

	GroRunIToken *token = NULL;
	while(TRUE) {
		token = grorun_iscanner_next((GroRunIScanner *) priv->scanner);

		if (token!=NULL) {
			cat_log_debug("token=%O", token);
			if (gro_scanner_is_eof(priv->scanner, token)) {
				break;
			}

			cat_array_wo_append(priv->token_cash, (GObject *) token);

			if (grorun_full_token_get_user_index((GroRunFullToken *) token)==GRO_SYM_EOL_COMMENT) {
				cat_unref_ptr(token);
				continue;
			}
		}
		break;
	}
	return token;
}


static void l_scanner_iface_init(GroRunIScannerInterface *iface) {
	iface->next = l_scanner_next;
}

/********************* end GroRunIScanner implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

static void l_message(GroIMessageHandler *self, CatStringWo *msg_text, GroRunLocation *location) {
	GroEdParser *parser = GROED_PARSER(self);
	GroEdParserPrivate *priv = groed_parser_get_instance_private(parser);
	GroRunMessage *run_message = grorun_message_new(NULL, msg_text, location);
	cat_log_error("run_message=%O", run_message);
	cat_array_wo_append(priv->messages, (GObject *) run_message);
	cat_unref_ptr(run_message);
}


static void l_message_iface_init(GroIMessageHandlerInterface *iface) {
	iface->message = l_message;
}
