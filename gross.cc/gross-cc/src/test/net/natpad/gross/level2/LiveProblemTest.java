package net.natpad.gross.level2;

import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import org.junit.Test;

import net.natpad.gross.Base;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.runtime.DefaultTokenFactory;
import net.natpad.gross.runtime.Terminal;

public class LiveProblemTest extends Base {


	@Test
	public void simpleTest() throws IOException {

		CCStateMachine stateMachine = createStateMachine(getClass().getResourceAsStream("live_problem.gro"));
		
		TerminalExt KWIMPORT = extractTerminal(stateMachine.model, "KWIMPORT");
		TerminalExt NAME = extractTerminal(stateMachine.model, "NAME");
		TerminalExt NEWLINE = extractTerminal(stateMachine.model, "NEWLINE");
		TerminalExt KWDEF = extractTerminal(stateMachine.model, "KWDEF");
		TerminalExt LPARREN = extractTerminal(stateMachine.model, "LPARREN");
		TerminalExt RPARREN = extractTerminal(stateMachine.model, "RPARREN");
		TerminalExt COLON = extractTerminal(stateMachine.model, "COLON");
		TerminalExt INDENT = extractTerminal(stateMachine.model, "INDENT");
		TerminalExt DEDENT = extractTerminal(stateMachine.model, "DEDENT");
		TerminalExt KWRETURN = extractTerminal(stateMachine.model, "KWRETURN");
		TerminalExt DOT = extractTerminal(stateMachine.model, "DOT");
		
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();

		
		List<Terminal> scannerTerminals = Arrays.asList(
//				KWIMPORT, NAME, NEWLINE
//				, KWIMPORT, NAME,DOT,NAME, NEWLINE
//				, NEWLINE
//				, 
//				KWDEF, NAME, LPARREN, RPARREN, COLON, NEWLINE
//				, INDENT, KWRETURN, NAME, DOT,NAME,DOT,NAME, LPARREN, RPARREN, NEWLINE
//				, DEDENT, NEWLINE
//				, NEWLINE
//				, KWDEF, NAME, LPARREN, RPARREN, COLON, NEWLINE
//				, INDENT, KWRETURN, NAME,DOT,NAME,DOT,NAME, LPARREN, RPARREN, NEWLINE
//				, DEDENT, NEWLINE
				

				NEWLINE, KWIMPORT, NAME, NEWLINE,
				KWIMPORT, NAME, DOT, NAME, NEWLINE, 
				NEWLINE, 
				KWDEF, NAME, LPARREN, RPARREN, COLON, NEWLINE,
				INDENT, KWRETURN, NAME, DOT, NAME, DOT, NAME, LPARREN, RPARREN, NEWLINE, 
				DEDENT, NEWLINE,
				NEWLINE, 
				KWDEF, NAME, LPARREN, RPARREN, COLON, NEWLINE, 
				INDENT,  KWRETURN, NAME,  DOT, NAME, DOT, NAME, LPARREN, RPARREN, NEWLINE, 
				DEDENT, NEWLINE, 
				NEWLINE
				
				);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, (Integer) null);

		
//		scannerTerminals = Arrays.asList(LEFTPAREN, IDENTIFIER, COMMA, IDENTIFIER, RIGHTPAREN );
//		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);
		
	}
	
}

