package net.natpad.gross.ebnf;

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

public class BasicRepititionTest extends Base {

	@Test
	public void simpleTest() throws IOException {
		CCStateMachine stateMachine = createStateMachine(getClass().getResourceAsStream("basic_repitition.gro"));
		CCModel model = stateMachine.model;
		
		TerminalExt ID = extractTerminal(model, "ID");
		TerminalExt SEMI = extractTerminal(model, "SEMI");
		TerminalExt BOOK = extractTerminal(model, "BOOK");
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();
		
		List<Terminal> scannerTerminals = Arrays.asList(ID, BOOK, SEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		
		scannerTerminals = Arrays.asList(ID, BOOK, BOOK, SEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);
		
		
		
//		JavaExporter je = new JavaExporter(stateMachine);
//		je.runExport(new File("./src/basic_optional/"));
//		
	}

	
	
}
