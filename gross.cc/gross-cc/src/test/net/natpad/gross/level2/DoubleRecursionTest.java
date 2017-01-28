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

public class DoubleRecursionTest extends Base {

	@Test
	public void simpleTest() throws IOException {
		CCStateMachine stateMachine = createStateMachine(getClass().getResourceAsStream("double_recursion.gro"));
		CCModel model = stateMachine.model;
		
		TerminalExt ID = extractTerminal(model, "ID");
		TerminalExt VAR = extractTerminal(model, "VAR");
		TerminalExt SEMI = extractTerminal(model, "SEMI");
		TerminalExt FUNCTION = extractTerminal(model, "FUNCTION");
		TerminalExt LBRACE = extractTerminal(model, "LBRACE");
		TerminalExt RBRACE = extractTerminal(model, "RBRACE");
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();
		
		List<Terminal> scannerTerminals = Arrays.asList(FUNCTION, LBRACE, VAR, ID, SEMI, RBRACE);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		
		scannerTerminals = Arrays.asList(VAR, ID, SEMI, FUNCTION, LBRACE, VAR, ID, SEMI, RBRACE);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);
	}
	
}
