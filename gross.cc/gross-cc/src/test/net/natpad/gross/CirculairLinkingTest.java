package net.natpad.gross;

import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import org.junit.Test;

import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.runtime.DefaultTokenFactory;
import net.natpad.gross.runtime.Terminal;

public class CirculairLinkingTest extends Base {


	@Test
	public void simpleTest() throws IOException {

		CCStateMachine stateMachine = createStateMachine(getClass().getResourceAsStream("circulair_linking.gro"));

		
		TerminalExt COLON, COMMA, ID, X, SEMI, ZZ;
		COLON = extractTerminal(stateMachine.model, "COLON");
		COMMA = extractTerminal(stateMachine.model, "COMMA");
		ID = extractTerminal(stateMachine.model, "ID");
		X = extractTerminal(stateMachine.model, "X");
		SEMI = extractTerminal(stateMachine.model, "SEMI");
		ZZ = extractTerminal(stateMachine.model, "ZZ");
		
		DefaultTokenFactory tokenFactory = new DefaultTokenFactory();
		List<Terminal> scannerTerminals = Arrays.asList(ID, SEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(ID, COMMA, ID, SEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(ID, COMMA, ID, COMMA, ID, SEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		scannerTerminals = Arrays.asList(ID, ID, COLON, SEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);

		
//		main                         ⟶ names SEMI;
//
//
//		names                        ⟶ names jan COLON
//		                             ⟶ names COMMA ID
//		                             ⟶ ID;
//
//
//		jan                          ⟶ deep X
//		                             ⟶ deep;
//
//		deep                         ⟶ ZZ
//		                             ⟶ names;
//		        

		
		
//		((ID)names, (ID)names, COMMA, ID, deep, X, COLON, X, COLON, (ZZ)deep, COLON)names, SEMI
//		((ID)names, (ID COMMA ID)names, (ZZ)deep, X, COLON, X, COLON, (ZZ)deep, COLON)names, SEMI
//		((ID)names, (ID COMMA ID)names, (ZZ X)jan, COLON, X, COLON, (ZZ)deep, COLON)names, SEMI
//		((ID)names, (ID COMMA ID ZZ X COLON)names, X, COLON, (ZZ)deep, COLON)names, SEMI
//		((ID)names, (ID COMMA ID ZZ X COLON)deep, X, COLON, (ZZ)deep, COLON)names, SEMI
//		((ID)names, (ID COMMA ID ZZ X COLON X)jan, COLON, (ZZ)deep, COLON)names, SEMI
//		((ID ID COMMA ID ZZ X COLON X COLON)names, (ZZ)deep, COLON)names, SEMI
//		((ID ID COMMA ID ZZ X COLON X COLON)names, (ZZ)jan, COLON)names, SEMI

		/*
		 * ID    ⟶ names    names
		 * ID    ⟶ names    names names
		 * COMMA            names names COMMA
		 * ID               names names COMMA ID ⟶ names names 
		 * ZZ               names names deep
		 * X                names names deep X ⟶ names names jan
		 * COLON            names names jan COLON ⟶ names names
		 * X                names deep X
		 * COLON            names jan COLON
		 * ZZ               names jan COLON deep ⟶ names ZZ ⟶ deep ZZ
		 */
		// ((ID ⟶ names) ((((ID ⟶ names) ((ZZ ⟶ deep) ⟶ jan) COLON ⟶ names ) ⟶ deep) X ⟶ jan) COLON ⟶ names) SEMI
//		scannerTerminals = Arrays.asList(ID, ID, ZZ, COLON, X, COLON, SEMI);
//		scanAndAccept(stateMachine, tokenFactory, scannerTerminals);

		
		scannerTerminals = Arrays.asList(ID, ID, COMMA, ID, ZZ, X, COLON, X, COLON, ZZ, COLON, SEMI);
		scanAndAccept(stateMachine, tokenFactory, scannerTerminals, 1);
		
	}

}
