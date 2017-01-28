package net.natpad.gross.runtime;

import java.util.ArrayList;

public class TransitionsForSymbol {

	public final Symbol symbol;
	
	public final ArrayList<Transition> unique = new ArrayList<Transition>();
	
	
	public TransitionsForSymbol(Symbol symbol) {
		this.symbol = symbol;
	}
	
	
	public void addTransition(Transition transition) {
		unique.add(transition);
	}
	
}
