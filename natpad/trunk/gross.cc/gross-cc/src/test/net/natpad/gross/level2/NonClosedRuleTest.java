package net.natpad.gross.level2;

import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import org.junit.Test;

import net.natpad.gross.Base;
import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.runtime.DefaultTokenFactory;
import net.natpad.gross.runtime.Terminal;

public class NonClosedRuleTest extends Base {

	@Test
	public void simpleTest() throws IOException {
		CCStateMachine stateMachine = createStateMachine(getClass().getResourceAsStream("non_closed_rule.gro"));
		CCModel model = stateMachine.model;
		
		TerminalExt ID = extractTerminal(model, "ID");
		TerminalExt NAME = extractTerminal(model, "NAME");
		TerminalExt FUNCTION = extractTerminal(model, "FUNCTION");
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();
		
		List<Terminal> scannerTerminals = Arrays.asList(ID, FUNCTION);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);
		
		scannerTerminals = Arrays.asList(ID);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		
		scannerTerminals = Arrays.asList(ID, NAME, FUNCTION);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

	}
	
}
