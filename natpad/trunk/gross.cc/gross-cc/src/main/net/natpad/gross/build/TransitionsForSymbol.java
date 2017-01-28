package net.natpad.gross.build;

import java.util.ArrayList;
import java.util.Iterator;

import net.natpad.gross.runtime.Symbol;

public class TransitionsForSymbol implements Iterable<Transition> {

	public final Symbol symbol;
	
	public final ArrayList<Transition> rawList = new ArrayList<Transition>();
	
	public final ArrayList<Transition> unique = new ArrayList<Transition>();
	
	
	public TransitionsForSymbol(Symbol symbol) {
		this.symbol = symbol;
	}
	
	
	public boolean addTransition(Transition transition) {
//		rawList.add(transition);
		for(int idx=0; idx<unique.size(); idx++) {
			Transition ut = unique.get(idx);
			if (ut.isSame(transition)) {
				return false;
			}
		}
		unique.add(transition);
		return true;
	}


	public boolean canBeMerged(TransitionsForSymbol sub) {
		if (sub.unique.size()!=unique.size()) {
			return false;
		}
		ArrayList<Transition> workList = new ArrayList<Transition>(sub.unique);
		for(Transition mainTrans : unique) {
			boolean matched = false;
			for(int idx=0; idx<workList.size(); idx++) {
				Transition subTransition = workList.get(idx);
				if (mainTrans.isSame(subTransition)) {
					matched = true;
					workList.remove(idx);
					idx = workList.size();
				}
			}
			if(!matched) {
				return false;
			}
		}
		
		return true;
	}
	
	
	@Override
	public Iterator<Transition> iterator() {
		return unique.iterator();
	}
}
