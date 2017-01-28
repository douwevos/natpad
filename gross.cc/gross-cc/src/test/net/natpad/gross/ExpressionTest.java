package net.natpad.gross;

import java.util.Arrays;
import java.util.List;

import org.junit.Assert;
import org.junit.Test;

import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.NonTerminalExt;
import net.natpad.gross.build.Production;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.runtime.DefaultTokenFactory;
import net.natpad.gross.runtime.Fork;
import net.natpad.gross.runtime.ParserContext;
import net.natpad.gross.runtime.Terminal;

public class ExpressionTest extends Base {

	TerminalExt tMUL, tDIV, tADD, tSUB, tNUM, tPAR;

	
	Production e1,e2,e3,f1,f2,f3,t1;
	
	
	/*
	 * expr   ⇒ expr MUL fact
	 *        ⇒ expr DIV fact
	 *        ⇒ fact
	 * fact   ⇒ fact ADD term
	 *        ⇒ fact SUB term
	 *        ⇒ term
	 * term   ⇒ NUM
	 *        ⇒ PAR expr PAR
	 */
	private CCStateMachine setupStateMachine() {

		CCModel model = new CCModel();
		tMUL = model.addTerminal("MUL");
		tDIV = model.addTerminal("DIV");
		tADD = model.addTerminal("ADD");
		tSUB = model.addTerminal("SUB");
		tNUM = model.addTerminal("NUM");
		tPAR = model.addTerminal("PAR");
		NonTerminalExt ntExpr = model.addNonTerminal("expr");
		NonTerminalExt ntFact = model.addNonTerminal("fact");
		NonTerminalExt ntTerm = model.addNonTerminal("term");

		/* expr   ⇒ expr MUL fact */
		/* expr   ⇒ expr DIV fact */
		/* expr   ⇒ fact */
		e1 = addRule(model, ntExpr, ntExpr, tMUL, ntFact);
		e2 = addRule(model, ntExpr, ntExpr, tDIV, ntFact);
		e3 = addRule(model, ntExpr, ntFact);
		

		/* fact   ⇒ fact ADD term */
		/* fact   ⇒ fact SUB term */
		/* fact   ⇒ term */
		f1 = addRule(model, ntFact, ntFact, tADD, ntTerm);
		f2 = addRule(model, ntFact, ntFact, tSUB, ntTerm);
		f3 = addRule(model, ntFact, ntTerm);


		/* term   ⇒ NUM */
		t1 = addRule(model, ntTerm, tNUM);
		t1 = addRule(model, ntTerm, tPAR, ntExpr, tPAR);

		CCStateMachine stateMachine = new CCStateMachine(logger, model);
		stateMachine.build();
		return stateMachine;
	}

	@Test
	public void simpleTest() {

		CCStateMachine stateMachine = setupStateMachine();
		
		List<Terminal> scannerTerminals = Arrays.asList(tNUM, tADD, tNUM);
		
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
