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

public class CirculairWithEmptyTest extends Base {

	TerminalExt tA;
	TerminalExt tB;

	
	
	/*
	 * subs   ⇒ jan subs A B
	 *        ⇒ ∅
	 * jan    ⇒ cupid A
	 * cupid  ⇒ subs
	 */
	private CCStateMachine setupStateMachine() {

		CCModel model = new CCModel();
		tA = model.addTerminal("A");
		tB = model.addTerminal("B");
		NonTerminalExt ntSubs = model.addNonTerminal("subs");
		NonTerminalExt ntJan = model.addNonTerminal("jan");
		NonTerminalExt ntCupid = model.addNonTerminal("cupid");

		/* subs -> jan subs A B */
		addRule(model, ntSubs, ntJan, ntSubs, tA, tB);

		/* subs -> */
		addRule(model, ntSubs);

		/* jan -> cupid A */
		addRule(model, ntJan, ntCupid, tA);

		/* cupid -> subs */
		addRule(model, ntCupid, ntSubs);

		CCStateMachine stateMachine = new CCStateMachine(logger, model);
		stateMachine.build();
		return stateMachine;
	}

	/* [[[[[[∅ ⇒ subs] ⇒ cupid] A] ⇒  jan [∅ ⇒ subs] A B ⇒ subs]  ⇒ cupid] A ⇒ jan [∅ ⇒ subs] A B ⇒ subs]
	 * A A B A A B 
	 */
	@Test
	public void simpleTest() {

		CCStateMachine stateMachine = setupStateMachine();
		
		List<Terminal> scannerTerminals = Arrays.asList(tA,tA,tB,tA,tA,tB);
//		List<Terminal> scannerTerminals = Arrays.asList(tA,tA,tB);
//		List<Terminal> scannerTerminals = Arrays.asList(tA, tA, tA, tA,tA,tB,tA,tB,tA,tB ,tA,tB);
		
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
