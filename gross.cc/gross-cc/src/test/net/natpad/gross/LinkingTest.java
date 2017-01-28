package net.natpad.gross;

import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import org.junit.Test;

import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.runtime.DefaultTokenFactory;
import net.natpad.gross.runtime.Terminal;

public class LinkingTest extends Base {

	
	@Test
	public void simpleTest() throws IOException {

		CCStateMachine stateMachine = createStateMachine(getClass().getResourceAsStream("linking.gro"));
		
		TerminalExt LAT, LBT, LOA, LTA,LTB;
		LAT = extractTerminal(stateMachine.model, "LAT");
		LBT = extractTerminal(stateMachine.model, "LBT");
		LOA = extractTerminal(stateMachine.model, "LOA");
		LTA = extractTerminal(stateMachine.model, "LTA");
		LTB = extractTerminal(stateMachine.model, "LTB");
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();

		
		List<Terminal> scannerTerminals = Arrays.asList(LTA);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(LTA, LAT);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(LTB,LOA);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(LTB,LOA,LBT);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);
	}
	
}
