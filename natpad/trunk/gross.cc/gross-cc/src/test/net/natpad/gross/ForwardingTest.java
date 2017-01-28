package net.natpad.gross;

import java.util.Arrays;
import java.util.List;

import org.junit.Test;

import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.NonTerminalExt;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.runtime.DefaultTokenFactory;
import net.natpad.gross.runtime.ParserContext.ReduceAction;
import net.natpad.gross.runtime.Terminal;

public class ForwardingTest extends Base {


	TerminalExt tSEMI, tID, tBAR, tCOLON, tRIGHT_DOUBLE_ARROW, tSTAR, tPLUS, tEXCL, tLPAREN, tRPAREN;

	
	
	private CCStateMachine setupStateMachine() {

		CCModel model = new CCModel();
		tSEMI = model.addTerminal("SEMI");
		tID = model.addTerminal("ID");
		tBAR = model.addTerminal("BAR");
		tCOLON = model.addTerminal("COLON");
		tRIGHT_DOUBLE_ARROW = model.addTerminal("RIGHT_DOUBLE_ARROW");
		tSTAR = model.addTerminal("STAR");
		tPLUS = model.addTerminal("PLUS");
		tEXCL = model.addTerminal("EXCL");
		tLPAREN = model.addTerminal("LPAREN");
		tRPAREN = model.addTerminal("RPAREN");


		NonTerminalExt ntProduction = model.addNonTerminal("production");
		
		NonTerminalExt ntRhsRuleList = model.addNonTerminal("rhs_rule_list");
		NonTerminalExt ntRhsRule = model.addNonTerminal("rhs_rule");
		NonTerminalExt ntRhsRulePartList = model.addNonTerminal("rhs_rule_part_list");
		NonTerminalExt ntRhsRulePart = model.addNonTerminal("rhs_rule_part");
		NonTerminalExt ntRhsEntryComplete = model.addNonTerminal("rhs_entry_complete");
		NonTerminalExt ntRhsEntry = model.addNonTerminal("rhs_entry");
		NonTerminalExt ntRhsSubList = model.addNonTerminal("rhs_sub_list");
		NonTerminalExt ntRhsSub = model.addNonTerminal("rhs_sub");
		
		
		NonTerminalExt ntId = model.addNonTerminal("nt_id");
		NonTerminalExt ntOptLabel = model.addNonTerminal("opt_label");

		NonTerminalExt ntLabelId = model.addNonTerminal("label_id");

		ReduceAction NO_ACTION = null;
		
		/* production ⇒ nt_id COLON_COLON_EQUALS rhs_list SEMI
		 *              nt_id RIGHT_DOUBLE_ARROW rhs_rules_list SEMI
		 */
//		addRule(model, NO_ACTION, ntProduction, ntId, null, tCOLON_COLON_EQUALS, null, ntRhsList, null, tSEMI, null);
		addRule(model, NO_ACTION, ntProduction, ntId, "lhs", tRIGHT_DOUBLE_ARROW, null, ntRhsRuleList, "rhsrlist", tSEMI, null);

		/* nt_id      ⇒ ID
		 */
		addRule(model, NO_ACTION, ntId, tID, "id");


		/* rhs_rule_list   ⇒ rhs_rule_list RIGHT_DOUBLE_ARROW rhs_rule
		 *                 ⇒ rhs_rule
		 */
		addRule(model, NO_ACTION, ntRhsRuleList, ntRhsRuleList, "rlslist", tRIGHT_DOUBLE_ARROW, null, ntRhsRule, "rule");
		addRule(model, NO_ACTION, ntRhsRuleList, ntRhsRule, "rule");

		
		

		/* rhs_rule        ⇒ rhs_rule_part_list
		 */
		addRule(model, NO_ACTION, ntRhsRule, ntRhsRulePartList, "rrpartlist");

		
		/* rhs_rule_part_list ⇒ rhs_rule_part_list rhs_rule_part
		 *                    ⇒
		 */
		addRule(model, NO_ACTION, ntRhsRulePartList, ntRhsRulePartList, "rrpartlist", ntRhsRulePart, "rrpart");
		addRule(model, NO_ACTION, ntRhsRulePartList);



		/* rhs_rule_part      ⇒ rhs_entry_complete
		 */
		addRule(model, NO_ACTION, ntRhsRulePart, ntRhsEntryComplete, "rec");


		/* rhs_entry_complete  ⇒ rhs_entry
		 *                     ⇒ rhs_entry STAR
		 *                     ⇒ rhs_entry PLUS
		 *                     ⇒ rhs_entry EXCL
		 */
		addRule(model, NO_ACTION, ntRhsEntryComplete, ntRhsEntry, "entry");
		addRule(model, NO_ACTION, ntRhsEntryComplete, ntRhsEntry, "entry", tSTAR, null);
		addRule(model, NO_ACTION, ntRhsEntryComplete, ntRhsEntry, "entry", tPLUS, null);
		addRule(model, NO_ACTION, ntRhsEntryComplete, ntRhsEntry, "entry", tEXCL, null);

		

		
		/* rhs_entry     ⇒ LPAREN rhs_sub_list RPAREN
		 *               ⇒ symbol_id opt_label
		 */
		addRule(model, NO_ACTION, ntRhsEntry, tLPAREN, null, ntRhsSubList, "rsl", tRPAREN, null);
		addRule(model, NO_ACTION, ntRhsEntry, tID, "symid", ntOptLabel, "lab");

		
		
		/* opt_label      ⇒ COLON label_id
		 *                ⇒
		 */
		addRule(model, NO_ACTION, ntOptLabel, tCOLON, null, ntLabelId, "lab");
		addRule(model, NO_ACTION, ntOptLabel);
		
		

		/* label_id       ⇒ ID
		 */
		addRule(model, NO_ACTION, ntLabelId, tID, "id");

		
		/* rhs_sub_list       ⇒ rhs_sub_list BAR rhs_sub
		/*                    ⇒ rhs_sub
		 */
		addRule(model, NO_ACTION, ntRhsSubList, ntRhsSubList, "rsl", tBAR, null, ntRhsSub, "sub");
		addRule(model, NO_ACTION, ntRhsSubList, ntRhsSub, "sub");

		
		/* rhs_sub       ⇒ rhs_sub rhs_entry_complete
		/*               ⇒ rhs_entry_complete
		 */
		addRule(model, (ReduceAction) null, ntRhsSub, ntRhsSub, "sub", ntRhsEntryComplete, "rec");
		addRule(model, NO_ACTION, ntRhsSub, ntRhsEntryComplete, "rec");

		
		CCStateMachine stateMachine = new CCStateMachine(logger, model);
		stateMachine.build();
		return stateMachine;
	}

	@Test
	public void simpleTest() {

		CCStateMachine stateMachine = setupStateMachine();
		
		List<Terminal> scannerTerminals = Arrays.asList(tID, tRIGHT_DOUBLE_ARROW, tID, tSEMI);
//		List<Terminal> scannerTerminals = Arrays.asList(tA,tA,tB,tA,tA,tB);
		super.scanAndAccept(stateMachine, new DefaultTokenFactory(), scannerTerminals, 1);
		
	}

	
}
