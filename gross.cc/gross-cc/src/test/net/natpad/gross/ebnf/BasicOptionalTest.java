package net.natpad.gross.ebnf;

import java.io.File;
import java.io.IOException;

import org.junit.Test;

import net.natpad.gross.Base;
import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.export.JavaExporter;

public class BasicOptionalTest extends Base {

	@Test
	public void simpleTest() throws IOException {
		CCStateMachine stateMachine = createStateMachine(getClass().getResourceAsStream("basic_optional.gro"));
		CCModel model = stateMachine.model;
		
		TerminalExt ID = extractTerminal(model, "ID");
		TerminalExt SEMI = extractTerminal(model, "SEMI");
		
//		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();
//		
//		List<Terminal> scannerTerminals = Arrays.asList(ID, SEMI);
//		scanAndAccept(stateMachine, tokenFactory, scannerTerminals);
//
//		
//		scannerTerminals = Arrays.asList(ID);
//		scanAndAccept(stateMachine, tokenFactory, scannerTerminals);
//		
//		
		
		JavaExporter je = new JavaExporter(stateMachine, null, "Gro", "");
		je.runExport(new File("./src/basic_optional/"));
		
	}

	
	
}
