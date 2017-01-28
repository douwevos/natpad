package net.natpad.gross.runtime;

import java.util.HashMap;

public class State {

	public final int id;
	public HashMap<Symbol, TransitionsForSymbol> transitionMap  = new HashMap<Symbol, TransitionsForSymbol>();

	
	
	public State(int id) {
		this.id = id;
	}

	
	public void addShift(Symbol forSymbol, State forwardState) {
		TransitionsForSymbol transitionsForSymbol = getOrCreateTransitionsForSymbol(forSymbol);
		
		ShiftTransition shiftTransition = new ShiftTransition(forwardState);
		transitionsForSymbol.addTransition(shiftTransition);
	}


	public void addReduce(Symbol followSym, int rhsCount, Symbol lhs, int actionId) {
		TransitionsForSymbol transitionsForSymbol = getOrCreateTransitionsForSymbol(followSym);
		
		ReduceTransition reduceTransition = new ReduceTransition(rhsCount, lhs, actionId);
		transitionsForSymbol.addTransition(reduceTransition);
	}

	public void addReduce(Symbol followSym, ReduceLayout reduceLayout, int actionId) {
		TransitionsForSymbol transitionsForSymbol = getOrCreateTransitionsForSymbol(followSym);
		
		ReduceTransition reduceTransition = new ReduceTransition(reduceLayout, actionId);
		transitionsForSymbol.addTransition(reduceTransition);
	}
	

	private TransitionsForSymbol getOrCreateTransitionsForSymbol(Symbol followSym) {
		TransitionsForSymbol transitionsForSymbol = transitionMap.get(followSym);
		if (transitionsForSymbol==null) {
			transitionsForSymbol = new TransitionsForSymbol(followSym);
			transitionMap.put(followSym, transitionsForSymbol);
		}
		return transitionsForSymbol;
	}

	
	public void addAccept(Symbol symbolAtDot) {
		TransitionsForSymbol transitionsForSymbol = getOrCreateTransitionsForSymbol(symbolAtDot);
		transitionsForSymbol.addTransition(new AcceptTransition());
		
	}
	
	@Override
	public String toString() {
		return "state["+id+"]";
	}
}
