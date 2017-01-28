package net.natpad.gross;

import java.io.FileReader;
import java.io.IOException;

import org.junit.Test;

import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.Production;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.build.parser.Actions;
import net.natpad.gross.build.parser.GrossParserTables;
import net.natpad.gross.definition.GrossScanner;
import net.natpad.gross.definition.bnf.Specification;
import net.natpad.gross.runtime.DefaultLogger;
import net.natpad.gross.runtime.GrossLogger.Level;
import net.natpad.gross.runtime.FullTokenFactory;
import net.natpad.gross.runtime.Parser;
import net.natpad.gross.runtime.ParserContext;
import net.natpad.gross.runtime.StateModel;

public class ProgramTest extends Base {

	TerminalExt tMUL, tDIV, tADD, tSUB, tNUM;

	
	Production e1,e2,e3,f1,f2,f3,t1;
	
	
	/*
	 * expr   ⇒ expr MUL fact
	 *        ⇒ expr DIV fact
	 *        ⇒ fact
	 * fact   ⇒ fact ADD term
	 *        ⇒ fact SUB term
	 *        ⇒ term
	 * term   ⇒ NUM
	 */
	private CCStateMachine setupStateMachine() throws IOException {

		
//		FileReader reader = new FileReader("./src/test/program.gro");
//		FileReader reader = new FileReader("/home/superman/cpp-workspace/trunk/gross.cc/gross-cc/etc/gross_grammar.gro");
		FileReader reader = new FileReader("./etc/gross_grammar.gro");

		
		GrossParserTables parserTables = new GrossParserTables();
		StateModel stateModel = new StateModel(parserTables.symbols, parserTables.cfgText, parserTables.stateCount);

		FullTokenFactory tokenFactory = new FullTokenFactory();
		GrossScanner scanner = new GrossScanner(tokenFactory, reader, stateModel);

		Actions actionHandler = new Actions();
		
		ParserContext parserContext = new ParserContext(tokenFactory, stateModel, actionHandler);

		
		DefaultLogger logger = new DefaultLogger(System.out, Level.WARN);
		
		Parser parser = new Parser(logger);
		Specification specification = parser.parse(parserContext, scanner);

		CCModel buildModel = specification.createModel();
		CCStateMachine stateMachine = new CCStateMachine(logger, buildModel);
		
//		stateMachine.build();
		return stateMachine;
	}

	@Test
	public void simpleTest() throws IOException {

		CCStateMachine stateMachine = setupStateMachine();
		stateMachine.build();
	}

	
}
