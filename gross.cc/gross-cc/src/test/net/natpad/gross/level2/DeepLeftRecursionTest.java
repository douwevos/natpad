package net.natpad.gross.level2;

import java.util.Arrays;
import java.util.List;

import org.junit.Test;

import net.natpad.gross.Base;
import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.NonTerminalExt;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.runtime.DefaultTokenFactory;
import net.natpad.gross.runtime.Terminal;

public class DeepLeftRecursionTest extends Base {

	TerminalExt tID, tVAR, tSEMI, tCOMMA;

	
	/*
	 * program        ⇒ statements
	 *                ⇒ 
	 * 
	 * statements     ⇒ statements vardecl
	 *                ⇒
	 *        
	 * vardecl        ⇒ VAR names SEMI
	 * 
	 * names          ⇒ names COMMA ID
	 *                ⇒ ID
	 *        
	 */
	private CCStateMachine setupStateMachine() {

		CCModel model = new CCModel();
		tID = model.addTerminal("ID");
		tVAR = model.addTerminal("VAR");
		tSEMI = model.addTerminal("SEMI");
		tCOMMA = model.addTerminal("COMMA");
		NonTerminalExt ntProgram = model.addNonTerminal("program");
		NonTerminalExt ntStatements = model.addNonTerminal("statements");
		NonTerminalExt ntVardecl = model.addNonTerminal("vardecl");
		NonTerminalExt ntNames = model.addNonTerminal("names");

		
		addRule(model, ntProgram, ntStatements);
		addRule(model, ntProgram);

		addRule(model, ntStatements, ntStatements, ntVardecl);
		addRule(model, ntStatements);
		
		addRule(model, ntVardecl, tVAR, ntNames, tSEMI);

		addRule(model, ntNames, ntNames, tCOMMA, tID);
		addRule(model, ntNames, tID);

		CCStateMachine stateMachine = new CCStateMachine(logger, model);
		stateMachine.build();
		return stateMachine;
	}

	
	
	
	@Test
	public void simpleTest() {

		CCStateMachine stateMachine = setupStateMachine();
		
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();

		
		List<Terminal> scannerTerminals = Arrays.asList();
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(tVAR, tID, tSEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(tVAR, tID, tSEMI, tVAR, tID, tSEMI, tVAR, tID, tSEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		
		scannerTerminals = Arrays.asList(tVAR, tID, tCOMMA, tID, tCOMMA, tID, tSEMI, tVAR, tID, tSEMI, tVAR, tID, tSEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		
	}





	
}
