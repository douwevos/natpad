package net.natpad.gross;

import java.util.Arrays;
import java.util.List;

import org.junit.Test;

import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.NonTerminalExt;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.runtime.DefaultTokenFactory;
import net.natpad.gross.runtime.Terminal;

public class LeftRecursionEmpty2Test extends Base {

	TerminalExt tID, tJAN;

	/*
	 * list   ⇒ list JAN ID
	 *        ⇒
	 */
	private CCStateMachine setupStateMachine() {
		CCModel model = new CCModel();
		tID = model.addTerminal("ID");
		tJAN = model.addTerminal("JAN");
		NonTerminalExt ntList = model.addNonTerminal("list");

		addRule(model, ntList, ntList, tJAN, tID);
		addRule(model, ntList);

		CCStateMachine stateMachine = new CCStateMachine(logger, model);
		stateMachine.build();
		return stateMachine;
	}

	
	@Test
	public void simpleTest() {
		CCStateMachine stateMachine = setupStateMachine();
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();
		
		List<Terminal> scannerTerminals = Arrays.asList();
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(tJAN, tID);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(tJAN, tID, tJAN, tID, tJAN, tID, tJAN, tID);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);
	}
}
