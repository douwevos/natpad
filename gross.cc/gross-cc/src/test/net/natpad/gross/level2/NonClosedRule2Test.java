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

public class NonClosedRule2Test extends Base {

	@Test
	public void simpleTest() throws IOException {
		CCStateMachine stateMachine = createStateMachine(getClass().getResourceAsStream("non_closed_rule2.gro"));
		CCModel model = stateMachine.model;
		
		TerminalExt TRY = extractTerminal(model, "TRY");
		TerminalExt CATCH = extractTerminal(model, "CATCH");
		TerminalExt ASS_IS = extractTerminal(model, "ASS_IS");
		TerminalExt ID = extractTerminal(model, "ID");
		TerminalExt NUMBER = extractTerminal(model, "NUMBER");
		TerminalExt SEMI = extractTerminal(model, "SEMI");
		TerminalExt LBRACE = extractTerminal(model, "LBRACE");
		TerminalExt RBRACE = extractTerminal(model, "RBRACE");
		TerminalExt LPAREN = extractTerminal(model, "LPAREN");
		TerminalExt RPAREN = extractTerminal(model, "RPAREN");
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();
		
		List<Terminal> scannerTerminals = Arrays.asList(TRY, LBRACE, ID, ASS_IS, NUMBER, SEMI, RBRACE, CATCH, LPAREN, ID, RPAREN, LBRACE, ID, ASS_IS, NUMBER, SEMI, RBRACE );
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

	}
	
}
