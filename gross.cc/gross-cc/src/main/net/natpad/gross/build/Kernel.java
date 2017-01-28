package net.natpad.gross.build;

import java.util.HashMap;
import java.util.Iterator;

import net.natpad.gross.runtime.Symbol;

public class Kernel implements Iterable<DotLink> {

	public final int id;
	
	HashMap<DotState, DotLink> main = new HashMap<DotState, DotLink>();
	HashMap<DotState, DotLink> extra = new HashMap<DotState, DotLink>();
	Integer hash;
	private SymbolSet lookahead;
	
	
	public Kernel(int id) {
		this.id = id;
	}

	public int size() {
		return main.size();
	}

	public SymbolSet getLookaheadSet() {
		if (lookahead==null) {
			
			SymbolSet lah = new SymbolSet();
			for(DotState state : main.keySet()) {
				calculateLoohaheadForDotState(lah, state);
			}
			for(DotState state : extra.keySet()) {
				calculateLoohaheadForDotState(lah, state);
			}
			lookahead = lah;
		}
		return lookahead;
	}

	private DotState calculateLoohaheadForDotState(SymbolSet lah, DotState state) {
		boolean keepShifting = true;
		while(keepShifting) {
			keepShifting = false;
			Symbol symbolAtDot = state.getSymbolAtDot();
			if (symbolAtDot!=null) {
				if (symbolAtDot instanceof NonTerminalExt) {
					NonTerminalExt nte = (NonTerminalExt) symbolAtDot;
					if (nte.getNullable()==Boolean.TRUE) {
						keepShifting = true;
						state = state.shift();
					}
				}
				if (lah.add(symbolAtDot)) {
					if (symbolAtDot instanceof NonTerminalExt) {
						NonTerminalExt nte = (NonTerminalExt) symbolAtDot;
						lah.addAll(nte.firstSet());
					}
				}
			}
		}
		return state;
	}

	
	public boolean add(DotLink dotLink) {
		if (dotLink==null) {
			throw new RuntimeException();
		}
		boolean result = false;
		DotLink put = main.put(dotLink.dotState, dotLink);
		if (dotLink!=put) {
			hash = null;
			result = true;
		}
		return result;
	}

	public boolean addExtra(DotLink dotLink) {
		if (dotLink==null) {
			throw new RuntimeException();
		}
		boolean result = false;
		DotLink put = extra.put(dotLink.dotState, dotLink);
		if (dotLink!=put) {
			result = true;
		}
		return result;
	}

	
	
	@Override
	public Iterator<DotLink> iterator() {
		return main.values().iterator();
	}
	
	
	@Override
	public int hashCode() {
		if (hash==null) {
			int val = 0;
			for(DotState dotState : main.keySet()) {
				val = val*273 + dotState.hashCode()*12;
			}
			hash = new Integer(val);
		}
		return hash.intValue();
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj==this) {
			return true;
		}
		if (obj instanceof Kernel) {
			Kernel other = (Kernel) obj;
			return other.main.keySet().equals(main.keySet());
		}
		return false;
	}


	
	@Override
	public String toString() {
		return ""+id;
	}

	
}
