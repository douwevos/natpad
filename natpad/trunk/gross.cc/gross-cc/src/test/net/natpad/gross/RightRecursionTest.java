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

public class RightRecursionTest extends Base {

	TerminalExt tID;

	
	/*
	 * list   ⇒ ID list 
	 *        ⇒ (ID);
	 */
	private CCStateMachine setupStateMachine(boolean empty) {
		CCModel model = new CCModel();
		tID = model.addTerminal("ID");
		NonTerminalExt ntList = model.addNonTerminal("list");

		addRule(model, ntList, tID, ntList);
		if (empty) {
			addRule(model, ntList);
		} else { 
			addRule(model, ntList, tID);
		}
		
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
