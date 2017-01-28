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

/* This class tries to test inner recursion
 * 
 *  a = b Z 
 *  b = X c Y c
 *      b Y c   
 * 
 * The first rule of b:  X c Y c ⚫ should besides seeing Z (a = b⚫Z) also see Y (b = b⚫Y c)
 * 
 * A valid rule is XcYcYcZ
 * 
 * These rules could be rewritten as:
 * 
 *   a = X n Z
 *   n = n Y c
 *       c Y c
 */
public class PrefixedLeftRecursionTest extends Base {


	@Test
	public void simpleTest() throws IOException {

		CCStateMachine stateMachine = createStateMachine(getClass().getResourceAsStream("prefixed_left_recursion.gro"));
		
		TerminalExt IDENTIFIER = extractTerminal(stateMachine.model, "IDENTIFIER");
		TerminalExt LEFTPAREN = extractTerminal(stateMachine.model, "LEFTPAREN");
		TerminalExt RIGHTPAREN = extractTerminal(stateMachine.model, "RIGHTPAREN");
		TerminalExt COMMA = extractTerminal(stateMachine.model, "COMMA");

		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();

		
		List<Terminal> scannerTerminals = Arrays.asList(
					LEFTPAREN, IDENTIFIER, COMMA, IDENTIFIER,
						COMMA, IDENTIFIER,
					RIGHTPAREN
				);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		
		scannerTerminals = Arrays.asList(LEFTPAREN, IDENTIFIER, COMMA, IDENTIFIER, RIGHTPAREN );
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);
		
	}
	
}
