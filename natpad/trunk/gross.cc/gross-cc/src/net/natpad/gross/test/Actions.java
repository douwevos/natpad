package net.natpad.gross.test;
import net.natpad.gross.runtime.Symbol;
import net.natpad.gross.runtime.Terminal;
import net.natpad.gross.runtime.NonTerminal;
import net.natpad.gross.runtime.Token;
import net.natpad.gross.runtime.Fork;
import net.natpad.gross.runtime.StackEntry;

import net.natpad.gross.definition.bnf.CodeDefinition;
import net.natpad.gross.definition.bnf.CodeDefinitionList;
import net.natpad.gross.definition.bnf.IdList;
import net.natpad.gross.definition.bnf.Imports;
import net.natpad.gross.definition.bnf.ProductionDefinition;
import net.natpad.gross.definition.bnf.ProductionDefinitionList;
import net.natpad.gross.definition.bnf.ProductionRuleReference;
import net.natpad.gross.definition.bnf.RhsRule;
import net.natpad.gross.definition.bnf.RhsRuleList;
import net.natpad.gross.definition.bnf.RhsRulePart;
import net.natpad.gross.definition.bnf.RhsRulePartList;
import net.natpad.gross.definition.bnf.RhsSub;
import net.natpad.gross.definition.bnf.RhsSubList;
import net.natpad.gross.definition.bnf.RhsSymbolPart;
import net.natpad.gross.definition.bnf.RhsUnaryPart;
import net.natpad.gross.definition.bnf.RhsUnaryPart.UnaryOpp;
import net.natpad.gross.definition.bnf.Specification;
import net.natpad.gross.definition.bnf.TerminalDeclaration;
import net.natpad.gross.definition.bnf.TerminalDeclarationList;
import net.natpad.gross.definition.bnf.TypeDefinition;
import net.natpad.gross.definition.bnf.TypeDefinitionList;


public class Actions {

	public <T> T extract(Fork fork, int offset) {
		T result = null;
		StackEntry fe = fork.getEntry(offset);
		if (fe!=null) {
			result = (T) fe.lookahead.getValue();
		}
		return result;
	}

	public Object doAction(Fork fork, int rhsCount, Symbol lhs, int actionId) {
		Object result = null;
		switch(actionId) {
			case 0 : { /* term_decl_list ⟶ term_decl_list:tdlist term_decl:td */
				TerminalDeclarationList tdlist = extract(fork, 2);
				TerminalDeclaration td = extract(fork, 1);
				result = new TerminalDeclarationList(tdlist, td); 
			} break;
			case 1 : { /* production_list ⟶ production:prod */
				ProductionDefinition prod = extract(fork, 1);
				result = new ProductionDefinitionList(null, prod); 
			} break;
			case 2 : { /* production_list ⟶ production_list:plist production:prod */
				ProductionDefinitionList plist = extract(fork, 2);
				ProductionDefinition prod = extract(fork, 1);
				result = new ProductionDefinitionList(plist, prod); 
			} break;
			case 3 : { /* ids ⟶ ID:id */
				String id = extract(fork, 1);
				result = new IdList(null, id); 
			} break;
			case 4 : { /* rhs_rule_list ⟶ rhs_rule:rule */
				RhsRule rule = extract(fork, 1);
				result = new RhsRuleList(null, rule); 
			} break;
			case 5 : { /* imports_code ⟶ IMPORTS CODE_TEXT:ct */
				String ct = extract(fork, 1);
				result = new Imports(ct); 
			} break;
			case 6 : { /* term_decl ⟶ TERMINAL ids:ids SEMI */
				IdList ids = extract(fork, 2);
				result = new TerminalDeclaration(ids); 
			} break;
			case 7 : { /* production ⟶ ID:lhsName rhs_rule_list:ruleList SEMI */
				String lhsName = extract(fork, 3);
				RhsRuleList ruleList = extract(fork, 2);
				result = new ProductionDefinition(lhsName, ruleList); 
			} break;
			case 8 : { /* rhs_rule_list ⟶ rhs_rule_list:ruleList rhs_rule:rule */
				RhsRuleList ruleList = extract(fork, 2);
				RhsRule rule = extract(fork, 1);
				result = new RhsRuleList(ruleList, rule); 
			} break;
			case 9 : { /* rhs_rule ⟶ RIGHT_DOUBLE_ARROW rhs_rule_part_list:rulePartList */
				RhsRulePartList rulePartList = extract(fork, 1);
				result = new RhsRule(null, rulePartList); 
			} break;
			case 10 : { /* spec ⟶ term_decl_list:td production_list:pl imports_code:im type_definition_list:tl code_definition_list:cl */
				TerminalDeclarationList td = extract(fork, 5);
				ProductionDefinitionList pl = extract(fork, 4);
				Imports im = extract(fork, 3);
				TypeDefinitionList tl = extract(fork, 2);
				CodeDefinitionList cl = extract(fork, 1);
				result = new Specification(td, pl, im, tl, cl); 
			} break;
			case 11 : { /* type_definition_list ⟶ type_definition_list:tdList type_definition:typeDef */
				TypeDefinitionList tdList = extract(fork, 2);
				TypeDefinition typeDef = extract(fork, 1);
				result = new TypeDefinitionList(tdList, typeDef); 
			} break;
			case 12 : { /* ids ⟶ ids:ids COMMA ID:id */
				IdList ids = extract(fork, 3);
				String id = extract(fork, 1);
				result = new IdList(ids, id); 
			} break;
			case 13 : { /* rhs_rule_part_list ⟶ rhs_rule_part_list:rulePartList rhs_rule_part:rulePart */
				RhsRulePartList rulePartList = extract(fork, 2);
				RhsRulePart rulePart = extract(fork, 1);
				result = new RhsRulePartList(rulePartList, rulePart); 
			} break;
			case 14 : { /* rhs_rule_part ⟶ rhs_entry_complete:entry */
				RhsRulePart entry = extract(fork, 1);
				result = entry; 
			} break;
			case 15 : { /* rhs_entry_complete ⟶ rhs_entry:entry */
				RhsRulePart entry = extract(fork, 1);
				result = entry; 
			} break;
			case 16 : { /* code_definition_list ⟶ code_definition_list:cdList code_definition:codeDef */
				CodeDefinitionList cdList = extract(fork, 2);
				CodeDefinition codeDef = extract(fork, 1);
				result = new CodeDefinitionList(cdList, codeDef); 
			} break;
			case 17 : { /* rhs_entry_complete ⟶ rhs_entry:entry STAR */
				RhsRulePart entry = extract(fork, 2);
				result = new RhsUnaryPart(entry, UnaryOpp.ZERO_OR_MORE); 
			} break;
			case 18 : { /* rhs_entry_complete ⟶ rhs_entry:entry PLUS */
				RhsRulePart entry = extract(fork, 2);
				result = new RhsUnaryPart(entry, UnaryOpp.ONE_OR_MORE); 
			} break;
			case 19 : { /* rhs_entry_complete ⟶ rhs_entry:entry EXCL */
				RhsRulePart entry = extract(fork, 2);
				result = new RhsUnaryPart(entry, UnaryOpp.OPTIONAL); 
			} break;
			case 20 : { /* rhs_sub_list ⟶ rhs_sub:sub */
				RhsSub sub = extract(fork, 1);
				result = new RhsSubList(null, sub); 
			} break;
			case 21 : { /* rhs_sub ⟶ rhs_entry_complete:entry */
				RhsRulePart entry = extract(fork, 1);
				result = new RhsSub(null, entry); 
			} break;
			case 22 : { /* rhs_entry ⟶ ID:symName opt_label:label */
				String symName = extract(fork, 2);
				String label = extract(fork, 1);
				result = new RhsSymbolPart(symName, label); 
			} break;
			case 23 : { /* rhs_rule ⟶ INDEX_MARK ID:imark RIGHT_DOUBLE_ARROW rhs_rule_part_list:rulePartList */
				String imark = extract(fork, 3);
				RhsRulePartList rulePartList = extract(fork, 1);
				result = new RhsRule(imark, rulePartList); 
			} break;
			case 24 : { /* prod_rule_ref ⟶ ID:lhsName */
				String lhsName = extract(fork, 1);
				result = new ProductionRuleReference(lhsName); 
			} break;
			case 25 : { /* rhs_entry ⟶ LPAREN rhs_sub_list:sub RPAREN */
				RhsSubList sub = extract(fork, 2);
				result = sub; 
			} break;
			case 26 : { /* rhs_sub ⟶ rhs_sub:sub rhs_entry_complete:entry */
				RhsSub sub = extract(fork, 2);
				RhsRulePart entry = extract(fork, 1);
				result = new RhsSub(sub, entry); 
			} break;
			case 27 : { /* opt_label ⟶ COLON ID:label */
				String label = extract(fork, 1);
				result = label; 
			} break;
			case 28 : { /* code_definition ⟶ CODE prod_rule_ref:ruleRef CODE_TEXT:codeText */
				ProductionRuleReference ruleRef = extract(fork, 2);
				String codeText = extract(fork, 1);
				result = new CodeDefinition(0, ruleRef, codeText); 
			} break;
			case 29 : { /* type_definition ⟶ TYPE ID:symtype ids:symids SEMI */
				String symtype = extract(fork, 3);
				IdList symids = extract(fork, 2);
				result = new TypeDefinition(symtype, symids); 
			} break;
			case 30 : { /* rhs_sub_list ⟶ rhs_sub_list:subList BAR rhs_sub:sub */
				RhsSubList subList = extract(fork, 3);
				RhsSub sub = extract(fork, 1);
				result = new RhsSubList(subList, sub); 
			} break;
			case 31 : { /* prod_rule_ref ⟶ ID:lhsName INDEX_MARK ID:label */
				String lhsName = extract(fork, 3);
				String label = extract(fork, 1);
				result = new ProductionRuleReference(lhsName, label); 
			} break;
			case 32 : { /* prod_rule_ref ⟶ ID:lhsName INDEX_MARK NUMBER:offset */
				String lhsName = extract(fork, 3);
				Long offset = extract(fork, 1);
				result = new ProductionRuleReference(lhsName, offset.intValue()); 
			} break;
			case 33 : { /* prod_rule_ref ⟶ ID:lhsName LSQUARE ID:label RSQUARE */
				String lhsName = extract(fork, 4);
				String label = extract(fork, 2);
				result = new ProductionRuleReference(lhsName, label); 
			} break;
		}
		return result;
	}
}
