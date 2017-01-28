package net.natpad.gross.level2;

import java.util.Arrays;
import java.util.List;

import org.junit.Test;

import net.natpad.gross.Base;
import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.NonTerminalExt;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.runtime.DefaultTokenFactory;
import net.natpad.gross.runtime.Terminal;

public class RightRecursionTest extends Base {

	TerminalExt tID, tSEMI;

	
	/*
	 * list   ⇒ ID opt_semi list 
	 *        ⇒ (ID opt_semi);
	 */
	private CCStateMachine setupStateMachine(boolean empty) {
		CCModel model = new CCModel();
		tID = model.addTerminal("ID");
		tSEMI = model.addTerminal("SEMI");
		NonTerminalExt ntList = model.addNonTerminal("list");
		NonTerminalExt ntOptSemi = model.addNonTerminal("opt_semi");
		NonTerminalExt ntStat = model.addNonTerminal("stat");

		addRule(model, ntList, ntStat, ntOptSemi, ntList);
		if (empty) {
			addRule(model, ntList);
		} else { 
			addRule(model, ntList, ntStat, ntOptSemi);
		}

		addRule(model, ntOptSemi);
		addRule(model, ntOptSemi, tSEMI);

		addRule(model, ntStat, tID);

		
		CCStateMachine stateMachine = new CCStateMachine(logger, model);
		stateMachine.build();
		return stateMachine;
	}

	
	
	
	@Test
	public void withEmpty() {

		CCStateMachine stateMachine = setupStateMachine(true);
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();

		
		List<Terminal> scannerTerminals = Arrays.asList();
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(tID);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(tID, tID, tID, tID);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

	}


	@Test
	public void noEmpty() {

		CCStateMachine stateMachine = setupStateMachine(false);
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();

		
		List<Terminal> scannerTerminals = Arrays.asList();
		scanAndNoAccept(stateMachine, tokenFactory, scannerTerminals);

		scannerTerminals = Arrays.asList(tID);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(tID, tID, tID, tID);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

	}


	
}
