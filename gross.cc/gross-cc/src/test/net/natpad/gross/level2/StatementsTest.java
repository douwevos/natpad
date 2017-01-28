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

public class StatementsTest extends Base {


	@Test
	public void simpleTest() throws IOException {

		CCStateMachine stateMachine = createStateMachine(getClass().getResourceAsStream("statements.gro"));
		
		TerminalExt tID = extractTerminal(stateMachine.model, "ID");
		TerminalExt tVAR = extractTerminal(stateMachine.model, "VAR");
		TerminalExt tSEMI = extractTerminal(stateMachine.model, "SEMI");
		TerminalExt tCOMMA = extractTerminal(stateMachine.model, "COMMA");
		TerminalExt tFUNCTION = extractTerminal(stateMachine.model, "FUNCTION");
		TerminalExt tLBRACE = extractTerminal(stateMachine.model, "LBRACE");
		TerminalExt tRBRACE = extractTerminal(stateMachine.model, "RBRACE");
		TerminalExt tLPAREN = extractTerminal(stateMachine.model, "LPAREN");
		TerminalExt tRPAREN = extractTerminal(stateMachine.model, "RPAREN");
		
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();

		
		List<Terminal> scannerTerminals = Arrays.asList();
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(tVAR, tID, tSEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 2);

		scannerTerminals = Arrays.asList(tVAR, tID, tSEMI, tVAR, tID, tSEMI, tVAR, tID, tSEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 2);

		scannerTerminals = Arrays.asList(tVAR, tID, tCOMMA, tID, tCOMMA, tID, tSEMI, tVAR, tID, tSEMI, tVAR, tID, tSEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 2);

		
		scannerTerminals = Arrays.asList(tVAR, tID, tCOMMA, tID, tCOMMA, tID, tSEMI, 
				tFUNCTION, tID, tLPAREN, tRPAREN,tLBRACE, tVAR, tID, tSEMI, tRBRACE);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 4);

		
	}





	
}
