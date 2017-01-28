package net.natpad.gross.definition.bnf;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.NonTerminalExt;
import net.natpad.gross.build.Production;
import net.natpad.gross.build.ProductionPart;
import net.natpad.gross.build.SymbolWithType;
import net.natpad.gross.definition.bnf.RhsUnaryPart.UnaryOpp;
import net.natpad.gross.runtime.Symbol;

public class Specification {

	public final TerminalDeclaration[] termDeclList;
	public final ProductionDefinition[] prodList;
	AstDescriptionList astDescList;
	Imports imports;
	TypeDefinitionList typeDefList;
	CodeDefinitionList codeDefList;
	
	public Specification(TerminalDeclarationList termDeclList, ProductionDefinitionList prodList, Imports imports, TypeDefinitionList typeDefList, CodeDefinitionList codeDefList) {
		this.termDeclList = termDeclList==null ? null : termDeclList.enlist();
		this.prodList = prodList.enlist();
		this.astDescList = null;
		this.imports = imports;
		this.typeDefList = typeDefList;
		this.codeDefList = codeDefList;
	}

	
	public Specification(TerminalDeclarationList termDeclList, ProductionDefinitionList prodList, AstDescriptionList astDescList) {
		this.termDeclList = termDeclList==null ? null : termDeclList.enlist();
		this.prodList = prodList.enlist();
		this.astDescList = astDescList;
		this.imports = null;
		this.typeDefList = null;
		this.codeDefList = null;
	}

	
	ProductionPart handleSymbolPart(CCModel model, RhsSymbolPart sym_part) {
		String sym_name = sym_part.symbolId;
		Symbol symbol = model.getSymbolByName(sym_name);
		ProductionPart result = null;

		if (symbol==null) {
			throw new RuntimeException("symbol with name "+sym_name+" not found");
		} else {
			result = new ProductionPart(symbol, sym_part.label);
		}
		return result;
	}
	
	

	void l_append_part(List<ProductionPartList> output_pp_arrays, ProductionPart part) {
		for(ProductionPartList single_prod_rhs : output_pp_arrays) {
			single_prod_rhs.list.add(part);
		}
	}
	
	
	public static class ProductionPartList {
		public String label;
		public int offset;
		public List<ProductionPart> list = new ArrayList<ProductionPart>();
		public List<ProductionPart> excluded;
		
		public ProductionPartList(int offset2) {
			offset = offset2;
		}

		ProductionPartList duplicate() {
			ProductionPartList result = new ProductionPartList(offset);
			result.label = label;
			result.list.addAll(list);
			if (excluded!=null) {
				result.excluded = new ArrayList<ProductionPart>();
				result.excluded.addAll(excluded);
			}
			return result;
		}
		
		
		public void exclude(ProductionPart pp) {
			if (excluded==null) {
				excluded = new ArrayList<ProductionPart>();
			}
			excluded.add(pp);
		}
	}


	
	public List<ProductionPartList> handleEntries(CCModel model, Iterator<RhsRulePart> iter, AtomicInteger offsetSequence, String label) {
		boolean result = true;
		List<ProductionPartList> output_pp_arrays = new ArrayList<ProductionPartList>();
		
		ProductionPartList e = new ProductionPartList(-1);
		if (offsetSequence!=null) {
			e.offset = offsetSequence.incrementAndGet();
		}
		e.label = label;
		output_pp_arrays.add(e);
		while(iter.hasNext()) {
			RhsRulePart rhsEntry = iter.next();
			
			
			if (rhsEntry instanceof RhsSubList) {
				/* this is a sub list  (A B | B | C D E) */
				
				RhsSubList rhsSubList = (RhsSubList) rhsEntry;
				List<RhsRulePart> subparts = rhsSubList.enlist();
				List<ProductionPartList> sub_pp_arrays = handleEntries(model, subparts.iterator(), null, null);
				sub_pp_arrays.remove(0);
				NonTerminalExt subLhs = model.addSubProductions(sub_pp_arrays);

				l_append_part(output_pp_arrays, new ProductionPart(subLhs, null));

			} else if (rhsEntry instanceof RhsUnaryPart) {
				/* this is a unary operation  A+, (C | D)*, (F)? */
				RhsUnaryPart rhs_unary = (RhsUnaryPart) rhsEntry;
				RhsRulePart un_entry = rhs_unary.rulePart;
				switch(rhs_unary.opperation) {
					case ONE_OR_MORE :
					case ZERO_OR_MORE : {
						/* the * operator means 0 or more repetitions */

						Symbol sym_to_rep = null;
						if (un_entry instanceof RhsSymbolPart) {
							/* This is a single symbol which will be repeated */
							String symId = ((RhsSymbolPart) un_entry).symbolId;
							sym_to_rep = model.getSymbolByName(symId);
							if (sym_to_rep==null) {
								throw new RuntimeException("symbol with name "+symId+" not found");
							}
						} else if (un_entry instanceof RhsEnlistable) {
							/* This is a complex sub list which is repeated. For this we will create an additional non terminal.
							 */
//							CatIIterator *sub_iter = cat_array_wo_iterator((CatArrayWo *) un_entry);
//							CatArrayWo *sub_pp_arrays = l_handle_entries(model, msg_handler, sub_iter);
//							cat_unref_ptr(sub_iter);
//							cat_array_wo_remove_first(sub_pp_arrays, NULL);
//							sym_to_rep = (GroMSymbol *) gro_model_add_sub_productions(model, sub_pp_arrays);
						}

						if (sym_to_rep!=null) {
							/* lets insert new rules
							 *
							 *    sub_nterm ::=
							 *    			  sub_nterm sym_to_rep
							 *    			| <sym_to_rep> (in case it is +)
							 *    			;
							 *
							 */
							NonTerminalExt sub_nterm = model.createSubNonTerminal();
							
							List<ProductionPart> pp_one = new ArrayList<ProductionPart>();
							ProductionPart pa1 = new ProductionPart(sub_nterm);
							pp_one.add(pa1);
							ProductionPart pa2 = new ProductionPart(sym_to_rep);
							pp_one.add(pa2);
							model.addProduction(-1, null, sub_nterm, pp_one, null);
							List<ProductionPart> pp_two = new ArrayList<ProductionPart>();
							if (rhs_unary.opperation==UnaryOpp.ONE_OR_MORE) {
								ProductionPart pa3 = new ProductionPart(sym_to_rep);
								pp_two.add(pa3);
							}
							model.addProduction(-1, null, sub_nterm, pp_two, null);
							ProductionPart pp_res = new ProductionPart(sub_nterm);
							l_append_part(output_pp_arrays, pp_res);
//						} else {
//							result = FALSE;
						}
					} break;
					case OPTIONAL : {
						ProductionPart psym_part = null;
						/* the ? operator means an optional entry */
						if (un_entry instanceof RhsSymbolPart) {
							/* This is a single symbol which is optional */
							String symId = ((RhsSymbolPart) un_entry).symbolId;
							Symbol opt_sym = model.getSymbolByName(symId);
							if (opt_sym==null) {
								throw new RuntimeException("symbol with name "+symId+" not found");
							}
							psym_part = handleSymbolPart(model, (RhsSymbolPart) un_entry);


						} else if (un_entry instanceof RhsEnlistable) {
							/* This is a complex sub list which is repeated. For this we will create an additional non terminal.
							 */

							
							
							List<RhsRulePart> enlist = ((RhsEnlistable) un_entry).enlist();
							List<ProductionPartList> sub_pp_arrays = handleEntries(model, enlist.iterator(), null, null);
							sub_pp_arrays.remove(0);
							if (sub_pp_arrays.size()==1) {
								ProductionPartList partList = sub_pp_arrays.get(0);
								if (partList.list.size()==1 && (partList.excluded==null || partList.excluded.isEmpty())) {
									ProductionPart productionPart = partList.list.get(0);
									psym_part = productionPart;
								}
							}
							
							if (psym_part==null) {
								NonTerminalExt sub_nt = model.addSubProductions(sub_pp_arrays);
								psym_part = new ProductionPart(sub_nt);
							}
							//							CatIIterator *sub_iter = cat_array_wo_iterator((CatArrayWo *) un_entry);
//							CatArrayWo *sub_pp_arrays = l_handle_entries(model, msg_handler, sub_iter);
//							cat_unref_ptr(sub_iter);
//							cat_array_wo_remove_first(sub_pp_arrays, NULL);
//							opt_sym = (Symbol *) gro_model_add_sub_productions(model, sub_pp_arrays);
						}


						

						int pp_count = output_pp_arrays.size();
						for(int idx=0; idx<pp_count; idx++) {
							ProductionPartList productionPartList = output_pp_arrays.get(idx);
							ProductionPartList duplicate = productionPartList.duplicate();
							
							duplicate.list.add(psym_part);
							output_pp_arrays.add(duplicate);
							productionPartList.exclude(psym_part);
						}


//						if (opt_sym) {
//							/* lets insert new rules
//							 *
//							 *    sub_nterm ::=
//							 *    			  opt_sym
//							 *    			|
//							 *    			;
//							 *
//							 */
//							GroMNonTerminal *sub_nterm = gro_model_create_sub_non_terminal(model);
//
//							CatArrayWo *pp_one = cat_array_wo_new();
//							GroMProdSymbolPart *pa1 = gro_m_prod_symbol_part_new(opt_sym, NULL);
//							cat_array_wo_append(pp_one, (GObject *) pa1);
//							cat_unref_ptr(pa1);
//							gro_model_add_production(model, sub_nterm, pp_one, -1, -1);
//							CatArrayWo *pp_two = cat_array_wo_new();
//							gro_model_add_production(model, sub_nterm, pp_two, -1, -1);
//							cat_unref_ptr(pp_one);
//							cat_unref_ptr(pp_two);
//
//							GroMProdSymbolPart *pp_res = gro_m_prod_symbol_part_new((Symbol *) sub_nterm, NULL);
//							l_append_part(output_pp_arrays, pp_res);
//							cat_unref_ptr(pp_res);
//						} else {
//							result = FALSE;
//						}

					} break;

				}
			} else if (rhsEntry instanceof RhsSub) {
				/* this is entry list in a sub list A B C */
				
				RhsSub sub = (RhsSub) rhsEntry;
				List<ProductionPartList> sub_pp_arrays = handleEntries(model, sub.enlist().iterator(), null, label);
				output_pp_arrays.addAll(sub_pp_arrays);
				
//				CatIIterator *sub_iter = gro_bnf_rhs_production_part_iterator((GroBnfRhs *) rhs_entry);
//				CatArrayWo *sub_pp_arrays = l_handle_entries(model, msg_handler, sub_iter);
//				cat_unref_ptr(sub_iter);
//				cat_array_wo_append_all(output_pp_arrays, sub_pp_arrays);
//				cat_unref_ptr(sub_pp_arrays);
//
			} else if (rhsEntry instanceof RhsRule) {
				RhsRule rule = (RhsRule) rhsEntry;
				List<ProductionPartList> sub_pp_arrays = handleEntries(model, rule.enlist().iterator(), offsetSequence, rule.label);
				output_pp_arrays.addAll(sub_pp_arrays);
			} else if (rhsEntry instanceof RhsSymbolPart) {
				ProductionPart psym_part = handleSymbolPart(model, (RhsSymbolPart) rhsEntry);
				if (psym_part!=null) {
					l_append_part(output_pp_arrays, psym_part);
				} else {
					result = false;
				}
			} else {
				throw new RuntimeException("unknown entry type:"+rhsEntry);
//			} else if (GRO_IS_BNF_RHS_ACTION_CODE(rhs_entry)) {
//				GroBnfRhsActionCode *code_part = (GroBnfRhsActionCode *) rhs_entry;
//				GroMProdActionPart *pact_part = gro_m_prod_action_part_new(gro_bnf_rhs_action_code_get_code(code_part));
//				l_append_part(output_pp_arrays, pact_part);
			}
		}
		return output_pp_arrays;
	}

	
	
	public CCModel createModel() {
		CCModel model = new CCModel();
		
//		model.setAstDescriptionList(astDescriptionList);
		
		/* declare terminals */
		for(TerminalDeclaration termDecl : termDeclList) {
			List<String> names = termDecl.enlist();
			for(String termName : names) {
				model.addTerminal(termName);
			}
		}

		/* declare non-terminals */
		for (ProductionDefinition productionDefinition : prodList) {
			String lhsId = productionDefinition.lhsId;
			model.addNonTerminal(lhsId);
		}

		
		
		for(ProductionDefinition productionDefinition : prodList) {
//			System.out.println("productionDefinition="+productionDefinition);
			String lhsId = productionDefinition.lhsId;
			NonTerminalExt lhsNTerm = model.getNonTerminalByName(lhsId);
			
			List<RhsRulePart> rules = productionDefinition.rhsRuleList.enlist();
			
			List<ProductionPartList> handleEntries = handleEntries(model, rules.iterator(), new AtomicInteger(-1), null);
			handleEntries.remove(0);
			for (ProductionPartList partList : handleEntries) {
				model.addProduction(partList.offset, partList.label, lhsNTerm, partList.list, partList.excluded);
			}
		}
		
		
		if (imports!=null) {
			model.setHeaderCodeText(imports.codeText);
		}

		if (typeDefList!=null) {
			List<TypeDefinition> tdlist = typeDefList.enlist();
			for(TypeDefinition typeDefinition : tdlist) {
				for(String symName : typeDefinition.symbolIdList.enlist()) {
					SymbolWithType swt = (SymbolWithType) model.getSymbolByName(symName);
					if (swt==null) {
						throw new RuntimeException("Can not define type of symbol with name "+symName+" since such a symbol can not be found.");
					}
					swt.setType(typeDefinition.type);
				}
			}
		}

		
		if (codeDefList!=null) {
			List<CodeDefinition> cdlist = codeDefList.enlist();
			for(CodeDefinition codeDefinition : cdlist) {
				ProductionRuleReference prodRuleRef = codeDefinition.prodRuleRef;
				ArrayList<Production> prods = findProductionByRef(model, prodRuleRef);
				for(Production prod : prods) {
				//			System.out.println("codeDefinition="+codeDefinition+", prod="+prod);
					prod.setActionCode(codeDefinition.codeText);
				}
			}
		}
		
		return model;
	}

	private ArrayList<Production> findProductionByRef(CCModel model, ProductionRuleReference prodRuleRef) {
		ArrayList<Production> result = new ArrayList<Production>();
		NonTerminalExt nonTerminal = model.getNonTerminalByName(prodRuleRef.productionLhsName);
		
		if (prodRuleRef.label==null && prodRuleRef.offset==null) {
			boolean notASingleRule = false;
			int offset = 0;
			for(int prodIdx=0; prodIdx<nonTerminal.productionCount(); prodIdx++) {
				Production production = nonTerminal.productionAt(prodIdx);
				if (prodIdx==0) {
					offset = production.offset;
				} else {
					if (offset!=production.offset) {
						notASingleRule = true;
					}
				}
				result.add(production);
			}
			if (notASingleRule) {
				System.err.println("Non-terminal is defined by multiple rules: reference is ambigous:"+prodRuleRef);
			}
		} else if (prodRuleRef.label!=null) {
			for(int prodIdx=0; prodIdx<nonTerminal.productionCount(); prodIdx++) {
				Production production = nonTerminal.productionAt(prodIdx);
				if (prodRuleRef.label.equals(production.label)) {
					result.add(production);
				}
			}
		} else if (prodRuleRef.offset!=null) {
			for(int prodIdx=0; prodIdx<nonTerminal.productionCount(); prodIdx++) {
				Production production = nonTerminal.productionAt(prodIdx);
				if (prodRuleRef.offset.intValue()==production.offset) {
					result.add(production);
				}
			}
		}
		return result;
	}
	
}
