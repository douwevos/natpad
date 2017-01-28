/*
   File:    grobnfspec.c
   Project: gross
   Author:  Douwe Vos
   Date:    May 22, 2016
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

#include "../file/grobnfspec.h"

//#include "grobnfstacktype.h"
#include "../../model/gromprodactionpart.h"
#include "../../model/gromprodsymbolpart.h"
#include "../../model/gromproduction.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroBnfSpec"
#include <logging/catlog.h>

struct _GroBnfSpecPrivate {
	GroBnfTerminalDeclarationList *term_decl_list;
	GroBnfProductionDefinitionList *prod_def_list;
	GroBnfImports *imports;
	GroBnfTypeDefinitionList *type_def_list;
	GroBnfCodeDefinitionList *code_def_list;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroBnfSpec, gro_bnf_spec, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroBnfSpec)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_bnf_spec_class_init(GroBnfSpecClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_bnf_spec_init(GroBnfSpec *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroBnfSpec *instance = GRO_BNF_SPEC(object);
	GroBnfSpecPrivate *priv = gro_bnf_spec_get_instance_private(instance);
	cat_unref_ptr(priv->code_def_list);
	cat_unref_ptr(priv->imports);
	cat_unref_ptr(priv->prod_def_list);
	cat_unref_ptr(priv->term_decl_list);
	cat_unref_ptr(priv->type_def_list);
	G_OBJECT_CLASS(gro_bnf_spec_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_bnf_spec_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroBnfSpec *gro_bnf_spec_new(GroBnfTerminalDeclarationList *term_decl_list, GroBnfProductionDefinitionList *prod_def_list, GroBnfImports *imports, GroBnfTypeDefinitionList *type_def_list, GroBnfCodeDefinitionList *code_def_list) {
	GroBnfSpec *result = g_object_new(GRO_TYPE_BNF_SPEC, NULL);
	cat_ref_anounce(result);
	GroBnfSpecPrivate *priv = gro_bnf_spec_get_instance_private(result);
	priv->term_decl_list = cat_ref_ptr(term_decl_list);
	priv->prod_def_list = cat_ref_ptr(prod_def_list);
	priv->imports = cat_ref_ptr(imports);
	priv->type_def_list = cat_ref_ptr(type_def_list);
	priv->code_def_list = cat_ref_ptr(code_def_list);
	return result;
}

//
//static gboolean l_create_terminals(GroModel *model, GroIMessageHandler *msg_handler, GroBnfSymbolDefList *symbol_def_list) {
//	gboolean result = TRUE;
//	GroBnfStackType *stack_type = gro_bnf_symbol_def_list_get_stack_type(symbol_def_list);
//	CatStringWo *st_text = stack_type==NULL ? NULL : gro_bnf_stack_type_get_as_text(stack_type);
//	gboolean is_terminal_list = gro_bnf_symbol_def_list_is_terminal_list(symbol_def_list);
//	CatArrayWo *string_list = gro_bnf_symbol_def_list_get_name_list(symbol_def_list);
//	CatIIterator *iter = cat_array_wo_iterator(string_list);
//	while(cat_iiterator_has_next(iter)) {
//		GroBnfString *item_string = (GroBnfString *) cat_iiterator_next(iter);
//		if (is_terminal_list) {
//			gboolean is_new = TRUE;
//			cat_log_debug("term:%o", item_string);
//			GroMTerminal *added = gro_model_add_terminal(model, item_string, st_text, &is_new);
//			if (!is_new) {
//				cat_log_warn("terminal with name '%O' was already added", item_string);
//				CatStringWo *msg = cat_string_wo_new();
//				cat_string_wo_format(msg, "terminal with name '%O' was already added", gro_bnf_string_get_text(item_string));
//				gro_imessage_handler_message(msg_handler, msg, gro_bnf_string_get_start(item_string), gro_bnf_string_get_end(item_string));
//				cat_unref_ptr(msg);
//				result = FALSE;
//			} else if (added==NULL) {
//				CatStringWo *msg = cat_string_wo_new();
//				cat_string_wo_format(msg, "symbol with name '%O' was already added", gro_bnf_string_get_text(item_string));
//				gro_imessage_handler_message(msg_handler, msg, gro_bnf_string_get_start(item_string), gro_bnf_string_get_end(item_string));
//				cat_unref_ptr(msg);
//				result = FALSE;
//			}
//		} else {
//			gro_model_add_non_terminal(model, item_string, st_text);
//			cat_log_debug("non-term:%o", item_string);
//		}
//	}
//	cat_unref_ptr(iter);
//
//	return result;
//}
//
//static void l_append_part(CatArrayWo *output_pp_arrays, GObject *part) {
//	CatIIterator *iter = cat_array_wo_iterator(output_pp_arrays);
//	while(cat_iiterator_has_next(iter)) {
//		CatArrayWo *single_prod_rhs = (CatArrayWo *) cat_iiterator_next(iter);
//		cat_array_wo_append(single_prod_rhs, part);
//	}
//	cat_unref_ptr(iter);
//}
//
//
//static GroMProdSymbolPart *l_handle_symbol_part(GroModel *model, GroIMessageHandler *msg_handler, GObject *rhs_entry) {
//	GroBnfSymbolPart *sym_part = (GroBnfSymbolPart *) rhs_entry;
//	GroBnfString *sym_string = gro_bnf_symbol_part_get_symbol_string(sym_part);
//	CatStringWo *sym_name = gro_bnf_string_get_text(sym_string);
//	GroMSymbol *symbol = gro_model_get_symbol_by_name(model, sym_name);
//	GroMProdSymbolPart *result = NULL;
//
//	if (symbol==NULL) {
//		CatStringWo *msg = cat_string_wo_new();
//		cat_string_wo_format(msg, "non terminal with name '%O' not found", sym_name);
//		gro_imessage_handler_message(msg_handler, msg, gro_bnf_string_get_start(sym_string), gro_bnf_string_get_end(sym_string));
//		cat_unref_ptr(msg);
//	} else {
//		result = gro_m_prod_symbol_part_new(symbol, gro_bnf_symbol_part_get_label(sym_part));
//	}
//	return result;
//}
//
//static CatArrayWo *l_handle_entries(GroModel *model, GroIMessageHandler *msg_handler, CatIIterator *iter) {
//	gboolean result = TRUE;
//	CatArrayWo *output_pp_arrays = cat_array_wo_new();
//	CatArrayWo *e = cat_array_wo_new();
//	cat_array_wo_append(output_pp_arrays, (GObject *) e);
//	cat_unref_ptr(e);
//	while(cat_iiterator_has_next(iter)) {
//		GObject *rhs_entry = cat_iiterator_next(iter);
//
//		if (CAT_IS_ARRAY_WO(rhs_entry)) {
//			/* this is a sub list  (A B | B | C D E) */
//
//			CatIIterator *sub_iter = cat_array_wo_iterator((CatArrayWo *) rhs_entry);
//			CatArrayWo *sub_pp_arrays = l_handle_entries(model, msg_handler, sub_iter);
//			cat_unref_ptr(sub_iter);
//			GroMNonTerminal *sub_lhs = gro_model_add_sub_productions(model, sub_pp_arrays);
//			GroMProdSymbolPart *psym_part = gro_m_prod_symbol_part_new((GroMSymbol *) sub_lhs, NULL);
//			l_append_part(output_pp_arrays, (GObject *) psym_part);
//			cat_unref_ptr(sub_pp_arrays);
//
//		} else if (GRO_IS_BNF_RHS_ENTRY_UNARY(rhs_entry)) {
//			/* this is a unary operation  A+, (C | D)*, (F)? */
//			GroBnfRhsEntryUnary *rhs_unary = (GroBnfRhsEntryUnary *) rhs_entry;
//			GObject *un_entry = gro_bnf_rhs_entry_unary_get_entry(rhs_unary);
//			GroBnfRhsEntryUnaryOpp operator = gro_bnf_rhs_entry_unary_get_operation(rhs_unary);
//			switch(operator) {
//				case GRO_ENTRY_UNARY_PLUS :
//				case GRO_ENTRY_UNARY_STAR : {
//					/* the * operator means 0 or more repetitions */
//
//					GroMSymbol *sym_to_rep = NULL;
//
//					if (GRO_IS_BNF_SYMBOL_PART(un_entry)) {
//						/* This is a single symbol which will be repeated */
//						GroBnfSymbolPart *sym_part = (GroBnfSymbolPart *) un_entry;
//						GroBnfString *sym_string = gro_bnf_symbol_part_get_symbol_string(sym_part);
//						CatStringWo *sym_name = gro_bnf_string_get_text(sym_string);
//						sym_to_rep = gro_model_get_symbol_by_name(model, sym_name);
//
//						if (sym_to_rep==NULL) {
//							CatStringWo *msg = cat_string_wo_new();
//							cat_string_wo_format(msg, "non terminal with name '%O' not found", sym_name);
//							gro_imessage_handler_message(msg_handler, msg, gro_bnf_string_get_start(sym_string), gro_bnf_string_get_end(sym_string));
//							cat_unref_ptr(msg);
//						}
//					} else if (CAT_IS_ARRAY_WO(un_entry)) {
//						/* This is a complex sub list which is repeated. For this we will create an additional non terminal.
//						 */
//						CatIIterator *sub_iter = cat_array_wo_iterator((CatArrayWo *) un_entry);
//						CatArrayWo *sub_pp_arrays = l_handle_entries(model, msg_handler, sub_iter);
//						cat_unref_ptr(sub_iter);
//						cat_array_wo_remove_first(sub_pp_arrays, NULL);
//						sym_to_rep = (GroMSymbol *) gro_model_add_sub_productions(model, sub_pp_arrays);
//					}
//
//					if (sym_to_rep) {
//						/* lets insert new rules
//						 *
//						 *    sub_nterm ::=
//						 *    			  sub_nterm sym_to_rep
//						 *    			| <sym_to_rep> (in case it is +)
//						 *    			;
//						 *
//						 */
//						GroMNonTerminal *sub_nterm = gro_model_create_sub_non_terminal(model);
//
//						CatArrayWo *pp_one = cat_array_wo_new();
//						GroMProdSymbolPart *pa1 = gro_m_prod_symbol_part_new((GroMSymbol *) sub_nterm, NULL);
//						cat_array_wo_append(pp_one, (GObject *) pa1);
//						cat_unref_ptr(pa1);
//						GroMProdSymbolPart *pa2 = gro_m_prod_symbol_part_new(sym_to_rep, NULL);
//						cat_array_wo_append(pp_one, (GObject *) pa2);
//						cat_unref_ptr(pa2);
//						gro_model_add_production(model, sub_nterm, pp_one, -1, -1);
//						CatArrayWo *pp_two = cat_array_wo_new();
//						if (operator==GRO_ENTRY_UNARY_PLUS) {
//							GroMProdSymbolPart *pa3 = gro_m_prod_symbol_part_new(sym_to_rep, NULL);
//							cat_array_wo_append(pp_two, (GObject *) pa3);
//							cat_unref_ptr(pa3);
//						}
//						gro_model_add_production(model, sub_nterm, pp_two, -1, -1);
//						cat_unref_ptr(pp_one);
//						cat_unref_ptr(pp_two);
//
//						GroMProdSymbolPart *pp_res = gro_m_prod_symbol_part_new((GroMSymbol *) sub_nterm, NULL);
//						l_append_part(output_pp_arrays, pp_res);
//						cat_unref_ptr(pp_res);
//					} else {
//						result = FALSE;
//					}
//				} break;
//				case GRO_ENTRY_UNARY_EXCL : {
//					GroMSymbol *opt_sym = NULL;
//					/* the ? operator means an optional entry */
//					if (GRO_IS_BNF_SYMBOL_PART(un_entry)) {
//						/* This is a single symbol which is optional */
//						GroBnfSymbolPart *sym_part = (GroBnfSymbolPart *) un_entry;
//						GroBnfString *sym_string = gro_bnf_symbol_part_get_symbol_string(sym_part);
//						CatStringWo *sym_name = gro_bnf_string_get_text(sym_string);
//						opt_sym = gro_model_get_symbol_by_name(model, sym_name);
//
//						if (opt_sym==NULL) {
//							CatStringWo *msg = cat_string_wo_new();
//							cat_string_wo_format(msg, "non terminal with name '%O' not found", sym_name);
//							gro_imessage_handler_message(msg_handler, msg, gro_bnf_string_get_start(sym_string), gro_bnf_string_get_end(sym_string));
//							cat_unref_ptr(msg);
//						}
//
//					} else if (CAT_IS_ARRAY_WO(un_entry)) {
//						/* This is a complex sub list which is repeated. For this we will create an additional non terminal.
//						 */
//						CatIIterator *sub_iter = cat_array_wo_iterator((CatArrayWo *) un_entry);
//						CatArrayWo *sub_pp_arrays = l_handle_entries(model, msg_handler, sub_iter);
//						cat_unref_ptr(sub_iter);
//						cat_array_wo_remove_first(sub_pp_arrays, NULL);
//						opt_sym = (GroMSymbol *) gro_model_add_sub_productions(model, sub_pp_arrays);
//					}
//
//
//
//					if (opt_sym) {
//						/* lets insert new rules
//						 *
//						 *    sub_nterm ::=
//						 *    			  opt_sym
//						 *    			|
//						 *    			;
//						 *
//						 */
//						GroMNonTerminal *sub_nterm = gro_model_create_sub_non_terminal(model);
//
//						CatArrayWo *pp_one = cat_array_wo_new();
//						GroMProdSymbolPart *pa1 = gro_m_prod_symbol_part_new(opt_sym, NULL);
//						cat_array_wo_append(pp_one, (GObject *) pa1);
//						cat_unref_ptr(pa1);
//						gro_model_add_production(model, sub_nterm, pp_one, -1, -1);
//						CatArrayWo *pp_two = cat_array_wo_new();
//						gro_model_add_production(model, sub_nterm, pp_two, -1, -1);
//						cat_unref_ptr(pp_one);
//						cat_unref_ptr(pp_two);
//
//						GroMProdSymbolPart *pp_res = gro_m_prod_symbol_part_new((GroMSymbol *) sub_nterm, NULL);
//						l_append_part(output_pp_arrays, pp_res);
//						cat_unref_ptr(pp_res);
//					} else {
//						result = FALSE;
//					}
//
//				} break;
//
//			}
//
//		} else if (GRO_IS_BNF_RHS(rhs_entry)) {
//			/* this is entry list in a sub list A B C */
//			CatIIterator *sub_iter = gro_bnf_rhs_production_part_iterator((GroBnfRhs *) rhs_entry);
//			CatArrayWo *sub_pp_arrays = l_handle_entries(model, msg_handler, sub_iter);
//			cat_unref_ptr(sub_iter);
//			cat_array_wo_append_all(output_pp_arrays, sub_pp_arrays);
//			cat_unref_ptr(sub_pp_arrays);
//
//		} else if (GRO_IS_BNF_SYMBOL_PART(rhs_entry)) {
//			GroMProdSymbolPart *psym_part = l_handle_symbol_part(model, msg_handler, rhs_entry);
//			if (psym_part) {
//				l_append_part(output_pp_arrays, psym_part);
//			} else {
//				result = FALSE;
//			}
//		} else if (GRO_IS_BNF_RHS_ACTION_CODE(rhs_entry)) {
//			GroBnfRhsActionCode *code_part = (GroBnfRhsActionCode *) rhs_entry;
//			GroMProdActionPart *pact_part = gro_m_prod_action_part_new(gro_bnf_rhs_action_code_get_code(code_part));
//			l_append_part(output_pp_arrays, pact_part);
//		}
//	}
//	return output_pp_arrays;
//}
//
//
//gboolean l_create_production(GroModel *model, GroIMessageHandler *msg_handler, GroBnfProduction *bnf_prod) {
//	gboolean result = TRUE;
//
//	GroBnfString *lh_sym_string = gro_bnf_production_get_lhs_string(bnf_prod);
//	CatStringWo *lh_sym_name = gro_bnf_string_get_text(lh_sym_string);
//	GroMNonTerminal *lhs_non_term = gro_model_get_non_terminal_by_name(model, lh_sym_name);
//	if (lhs_non_term==NULL) {
//		CatStringWo *msg = cat_string_wo_new();
//		cat_log_error("non terminal with name '%o' not found", lh_sym_name);
//		cat_string_wo_format(msg, "non terminal with name '%o' not found", lh_sym_name);
//		gro_imessage_handler_message(msg_handler, msg, gro_bnf_string_get_start(lh_sym_string), gro_bnf_string_get_end(lh_sym_string));
//		cat_unref_ptr(msg);
//		return FALSE;
//	}
//
//	GroBnfRhs *rhs = gro_bnf_production_get_rhs(bnf_prod);
//	CatIIterator *pp_iter = gro_bnf_rhs_production_part_iterator(rhs);
//	CatArrayWo *base_pp = l_handle_entries(model, msg_handler, pp_iter);
//
//
////	GroBnfRhs *rhs = gro_bnf_production_get_rhs(bnf_prod);
////	CatArrayWo *base_pp = cat_array_wo_new();
////	CatIIterator *pp_iter = gro_bnf_rhs_production_part_iterator(rhs);
////	while(cat_iiterator_has_next(pp_iter)) {
////		GObject *rhs_entry = cat_iiterator_next(pp_iter);
////		if (GRO_IS_BNF_SYMBOL_PART(rhs_entry)) {
////			GroBnfSymbolPart *sym_part = (GroBnfSymbolPart *) rhs_entry;
////			GroBnfString *sym_string = gro_bnf_symbol_part_get_symbol_string(sym_part);
////			CatStringWo *sym_name = gro_bnf_string_get_text(sym_string);
////			GroMSymbol *symbol = gro_model_get_symbol_by_name(model, sym_name);
////
////			if (symbol==NULL) {
////				CatStringWo *msg = cat_string_wo_new();
////				cat_string_wo_format(msg, "non terminal with name '%O' not found", sym_name);
////				gro_imessage_handler_message(msg_handler, msg, gro_bnf_string_get_start(sym_string), gro_bnf_string_get_end(sym_string));
////				cat_unref_ptr(msg);
////				result = FALSE;
////			} else {
////				GroMProdSymbolPart *psym_part = gro_m_prod_symbol_part_new(symbol, gro_bnf_symbol_part_get_label(sym_part));
////				cat_array_wo_append(base_pp, (GObject *) psym_part);
////			}
////		} else if (GRO_IS_BNF_RHS_ACTION_CODE(rhs_entry)) {
////			GroBnfRhsActionCode *code_part = (GroBnfRhsActionCode *) rhs_entry;
////			GroMProdActionPart *pact_part = gro_m_prod_action_part_new(gro_bnf_rhs_action_code_get_code(code_part));
////			cat_array_wo_append(base_pp, (GObject *) pact_part);
////		}
////	}
//	cat_unref_ptr(pp_iter);
//
//	GroBnfString *prec_term_name = gro_bnf_rhs_get_prec_term_name(rhs);
//	GroMTerminal *prec_term = NULL;
//	int prod_prec_num = -1;
//	int prod_prec_side = -1;
//	if (prec_term_name) {
//		prec_term = gro_model_get_terminal_by_name(model, gro_bnf_string_get_text(prec_term_name));
//		if (prec_term) {
//// TODO
//		} else {
//			CatStringWo *msg = cat_string_wo_new();
//			cat_string_wo_format(msg, "precedence terminal with name '%O' not found", gro_bnf_string_get_text(prec_term_name));
//			gro_imessage_handler_message(msg_handler, msg, gro_bnf_string_get_start(prec_term_name), gro_bnf_string_get_end(prec_term_name));
//			cat_unref_ptr(msg);
//			result = FALSE;
//		}
//	}
//
//	if (result) {
//		CatIIterator *iiter = cat_array_wo_iterator(base_pp);
//		while(cat_iiterator_has_next(iiter)) {
//			CatArrayWo *sub_pp = cat_iiterator_next(iiter);
//			gro_model_add_production(model, lhs_non_term, sub_pp, prod_prec_num, prod_prec_side);
//		}
//		cat_unref_ptr(iiter);
////		gro_model_add_production(model, lhs_non_term, base_pp, prod_prec_num, prod_prec_side);
//	}
//
//	return result;
//}


GroModel *gro_bnf_spec_create_model(GroIMessageHandler *msg_handler, GroBnfSpec *spec) {
	GroBnfSpecPrivate *priv = gro_bnf_spec_get_instance_private(spec);
	GroModel *result = gro_model_new();
//
//	CatIIterator *iter = cat_array_wo_iterator(priv->symbol_def_list);
//	gboolean success = TRUE;
//	while(cat_iiterator_has_next(iter)) {
//		GObject *def_line = cat_iiterator_next(iter);
//		if (GRO_IS_BNF_SYMBOL_DEF_LIST(def_line)) {
//			if (!l_create_terminals(result, msg_handler, (GroBnfSymbolDefList *) def_line)) {
//				success = FALSE;
//			}
//
//		}
//	}
//	cat_unref_ptr(iter);
//
//	GroBnfString *start_string = NULL;
//	if (priv->start_production_string==NULL) {
//		GroBnfProduction *fp = (GroBnfProduction *) cat_array_wo_get_first(priv->production_list);
//		if (fp==NULL) {
//			CatStringWo *msg = cat_string_wo_new_with("empty production list can not find start symbol");
//			gro_imessage_handler_message(msg_handler, msg, NULL, NULL);
//			cat_unref_ptr(msg);
//			success = FALSE;
//		} else {
//			start_string = gro_bnf_production_get_lhs_string(fp);
//		}
//	} else {
//		start_string = priv->start_production_string;
//	}
//
//
//	GroMNonTerminal *start_non_term = gro_model_get_non_terminal_by_name(result, gro_bnf_string_get_text(start_string));
//
//	if (start_non_term==NULL) {
//		cat_log_error("start_name non-terminal not found:%o", gro_bnf_string_get_text(start_string));
//		return NULL;
//	}
//
//	CatArrayWo *base_pp = cat_array_wo_new();
//	const CatStringWo *start_val = cat_string_wo_new_data("start_val");
//	GroMProdSymbolPart *bp1 = gro_m_prod_symbol_part_new((GroMSymbol *) start_non_term, gro_bnf_string_new(start_val, NULL, NULL));
//	GroMProdSymbolPart *bp2 = gro_m_prod_symbol_part_new((GroMSymbol *) gro_model_get_terminal_eof(result), NULL);
//	const CatStringWo *code_string = cat_string_wo_new_data("RESULT = start_val;");
//	GroMProdActionPart *bp3 = gro_m_prod_action_part_new(gro_bnf_string_new(code_string, NULL, NULL));
//
//	cat_array_wo_append(base_pp, (GObject *) bp1);
//	cat_array_wo_append(base_pp, (GObject *) bp2);
//	cat_array_wo_append(base_pp, (GObject *) bp3);
//
//	GroMNonTerminal *dollar_start = gro_model_get_non_terminal_dollar_start(result);
//
//	priv->start_production = gro_model_add_production(result, dollar_start, base_pp, -1, -1);
//
//	cat_log_debug("created start-production:%o", priv->start_production);
//
//
//	iter = cat_array_wo_iterator(priv->production_list);
//	while(cat_iiterator_has_next(iter)) {
//		GObject *entry = cat_iiterator_next(iter);
//		if (GRO_IS_BNF_PRODUCTION(entry)) {
//			if (!l_create_production(result, msg_handler, (GroBnfProduction *) entry)) {
//				success = FALSE;
//			}
//		}
//	}
//	cat_unref_ptr(iter);
//
//	if (!success) {
//		cat_unref_ptr(result);
//	}

	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroBnfSpec *instance = GRO_BNF_SPEC(self);
	GroBnfSpecPrivate *priv = gro_bnf_spec_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
