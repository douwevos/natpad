package net.natpad.gross;

import java.util.Arrays;
import java.util.List;

import org.junit.Assert;
import org.junit.Test;

import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.NonTerminalExt;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.runtime.DefaultTokenFactory;
import net.natpad.gross.runtime.Fork;
import net.natpad.gross.runtime.ParserContext;
import net.natpad.gross.runtime.Terminal;

public class ErrorTest extends Base {

	TerminalExt tSAY, tOPEN, tSEMI, tID, tNUM, tERROR;

	
	
	/*
	 * statements      ⇒ statements statement
	 *                 ⇒ 
	 * statement       ⇒ open_statement SEMI
	 *                 ⇒ say_statement SEMI
	 *                 ⇒ ⊗ SEMI
	 * open_statement  ⇒ tOPEN tID
	 * say_statement   ⇒ tSAY tID
	 * 
	 */
	private CCStateMachine setupStateMachine() {

		CCModel model = new CCModel();
		tERROR = model.getErrorTerminal();
		tSAY = model.addTerminal("SAY");
		tOPEN = model.addTerminal("OPEN");
		tSEMI = model.addTerminal("SEMI");
		tID = model.addTerminal("ID");
		NonTerminalExt ntStatements = model.addNonTerminal("statements");
		NonTerminalExt ntStatement = model.addNonTerminal("statement");
		NonTerminalExt ntOpenStatement = model.addNonTerminal("open_statement");
		NonTerminalExt ntSayStatement = model.addNonTerminal("say_statement");

		/*
		 * statements      ⇒ statements statement
		 *                 ⇒
		 */ 
		addRule(model, ntStatements, ntStatements, ntStatement);
		addRule(model, ntStatements);
		

		/* statement       ⇒ open_statement SEMI */
		/*                 ⇒ say_statement SEMI */
		/*                 ⇒ ⊗ SEMI */
		addRule(model, ntStatement, ntOpenStatement, tSEMI);
		addRule(model, ntStatement, ntSayStatement, tSEMI);
		addRule(model, ntStatement, model.getErrorTerminal(), tSEMI);


		/* open_statement  ⇒ tOPEN tID */
		addRule(model, ntOpenStatement, tOPEN, tID);

		/* say_statement  ⇒ tSAY tID */
		addRule(model, ntSayStatement, tSAY, tID);

		CCStateMachine stateMachine = new CCStateMachine(logger, model);
		stateMachine.build();
		return stateMachine;
	}

	@Test
	public void simpleSuccessTest() {

		CCStateMachine stateMachine = setupStateMachine();
		
		List<Terminal> scannerTerminals = Arrays.asList(tSAY, tID, tSEMI);
		
		BasicScanner scanner = new BasicScanner(scannerTerminals, stateMachine.model.getEofTerminal());
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();
		ParserContext context = doParse(stateMachine, tokenFactory, scanner);

		List<Fork> acceptedForks = context.getAcceptedForks();
		Assert.assertNotNull("no accepted forks", acceptedForks);
		Assert.assertFalse("no accepted forks", acceptedForks.isEmpty());
		Fork acceptedFork = acceptedForks.get(0);
		
		
		System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> accepted fork");
		acceptedFork.dump();
	}

	@Test
	public void singleErrorWithRecoveryTest() {

		CCStateMachine stateMachine = setupStateMachine();
		
		List<Terminal> scannerTerminals = Arrays.asList(tSAY, tID, tSEMI, tID, tSAY, tID, tSEMI, tSAY, tID, tSEMI, tSAY, tID, tSEMI, tSAY, tID, tSEMI);
		
		BasicScanner scanner = new BasicScanner(scannerTerminals, stateMachine.model.getEofTerminal());
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();
		ParserContext context = doParse(stateMachine, tokenFactory, scanner);

		List<Fork> acceptedForks = context.getAcceptedForks();
		Assert.assertNotNull("no accepted forks", acceptedForks);
		Assert.assertFalse("no accepted forks", acceptedForks.isEmpty());
		Fork acceptedFork = acceptedForks.get(0);
		
		
		System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> accepted fork");
		acceptedFork.dump();
	}

	
}
