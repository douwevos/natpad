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

public class MultipleKernsTest extends Base {

	TerminalExt tA, tB, tC;

	
	
	/*
	 * kern   ⇒ hit
	 *        ⇒ jambo
	 *        ⇒ A bba C
	 * hit    ⇒ abba B
	 * jambo  ⇒ baab A
	 * abba   ⇒ A B B A
	 * baab   ⇒ A B B A
	 * bba    ⇒ B B A
	 */
	private CCStateMachine setupStateMachine() {

		CCModel model = new CCModel();
		tA = model.addTerminal("A");
		tB = model.addTerminal("B");
		tC = model.addTerminal("C");
		NonTerminalExt ntKern = model.addNonTerminal("kern");
		NonTerminalExt ntHit = model.addNonTerminal("hit");
		NonTerminalExt ntJambo = model.addNonTerminal("jambo");
		NonTerminalExt ntAbba = model.addNonTerminal("abba");
		NonTerminalExt ntBaab = model.addNonTerminal("baab");
		NonTerminalExt ntBba = model.addNonTerminal("bba");

		/* kern   ⇒ hit */
		/* kern   ⇒ jambo */
		addRule(model, ntKern, ntHit);
		addRule(model, ntKern, ntJambo);
		addRule(model, ntKern, tA, ntBba, tC);


		/* hit    ⇒ abba B */
		addRule(model, ntHit, ntAbba, tB);

		/* jambo  ⇒ baab A */
		addRule(model, ntJambo, ntBaab, tA);

		/* abba   ⇒ A B B A */
		addRule(model, ntAbba, tA, tB, tB, tA);

		/* baab   ⇒ A B B A */
		addRule(model, ntBaab, tA, tB, tB, tA);

		/* bba   ⇒ B B A */
		addRule(model, ntBba, tB, tB, tA);

		CCStateMachine stateMachine = new CCStateMachine(logger, model);
		stateMachine.build();
		return stateMachine;
	}

	@Test
	public void simpleTest() {

		CCStateMachine stateMachine = setupStateMachine();
		
		
		List<Terminal> scannerTerminals = Arrays.asList(tA, tB, tB, tA, tB);
		
		BasicScanner scanner = new BasicScanner(scannerTerminals, stateMachine.model.getEofTerminal());
		
		ParserContext context = doParse(stateMachine, new DefaultTokenFactory(), scanner);

		List<Fork> acceptedForks = context.getAcceptedForks();
		Assert.assertNotNull("no accepted forks", acceptedForks);
		Assert.assertFalse("no accepted forks", acceptedForks.isEmpty());
		Fork acceptedFork = acceptedForks.get(0);
		
		
		System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> accepted fork");
		acceptedFork.dump();
	}

}
