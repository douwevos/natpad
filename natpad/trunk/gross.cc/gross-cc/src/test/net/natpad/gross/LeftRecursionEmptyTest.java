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

public class LeftRecursionEmptyTest extends Base {

	TerminalExt tID;

	
	/*
	 * list   ⇒ list ID
	 *        ⇒
	 */
	private CCStateMachine setupStateMachine() {
		CCModel model = new CCModel();
		tID = model.addTerminal("ID");
		NonTerminalExt ntList = model.addNonTerminal("list");

		addRule(model, ntList, ntList, tID);
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

		scannerTerminals = Arrays.asList(tID);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(tID, tID, tID, tID);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

	}




	
}
