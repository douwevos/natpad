package net.natpad.gross;

import java.util.Iterator;
import java.util.List;

import net.natpad.gross.runtime.DefaultToken;
import net.natpad.gross.runtime.Scanner;
import net.natpad.gross.runtime.Terminal;
import net.natpad.gross.runtime.Token;

public class BasicScanner implements Scanner {

	
	List<Terminal> terminals;
	Terminal eofTerminal;
	Iterator<Terminal> iter;
	
	public BasicScanner(List<Terminal> terminals, Terminal eofTerminal) {
		this.terminals = terminals;
		this.eofTerminal = eofTerminal;
		iter = terminals.iterator();
	}

	
	@Override
	public Token next() {
		if (iter.hasNext()) {
			return new DefaultToken(iter.next(), null);
		}
		return new DefaultToken(eofTerminal, null);
	}
	
}
