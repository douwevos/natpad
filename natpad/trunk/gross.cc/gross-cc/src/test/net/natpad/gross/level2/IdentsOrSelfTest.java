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

public class IdentsOrSelfTest extends Base {

	@Test
	public void simpleTest() throws IOException {
		CCStateMachine stateMachine = createStateMachine(getClass().getResourceAsStream("idents_or_self.gro"));
		CCModel model = stateMachine.model;
		
		TerminalExt AS = extractTerminal(model, "AS");
		TerminalExt IDENT = extractTerminal(model, "IDENT");
		TerminalExt COMMA = extractTerminal(model, "COMMA");
		TerminalExt SELF = extractTerminal(model, "SELF");
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();
		
		List<Terminal> scannerTerminals = Arrays.asList(IDENT, COMMA, IDENT);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(IDENT, COMMA, IDENT, AS, IDENT);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

	}
}
