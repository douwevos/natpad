package net.natpad.gross.build;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import net.natpad.gross.runtime.Symbol;

public class SymbolSet implements Iterable<Symbol> {

	HashSet<Symbol> set = new HashSet<Symbol>();
	Integer hash = null;
	
	public SymbolSet() {
	}

	
	public boolean add(Symbol sym) {
		boolean result = set.add(sym);
		if (result) {
			hash = null;
		}
		return result;
	}
	
	
	@Override
	public int hashCode() {
		if (hash==null) {
			int val = 0;
			for(Symbol sym : set) {
				val = -val*13 + sym.hashCode();
			}
			hash = new Integer(val);
		}
		return hash.intValue();
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj instanceof SymbolSet) {
			SymbolSet other = (SymbolSet) obj;
			return other.set.equals(set);
		}
		return false;
	}


	public void addAll(Set<Symbol> otherSet) {
		if (otherSet!=null) {
			set.addAll(otherSet);
		}
	}
	
	@Override
	public Iterator<Symbol> iterator() {
		return set.iterator();
	}
	
	
	@Override
	public String toString() {
		ArrayList<Symbol> symList = new ArrayList<Symbol>(set);
		Collections.sort(symList, sorter);
		StringBuilder buf = new StringBuilder();
		for (Symbol symbol : symList) {
			buf.append(',').append(symbol.name);
		}
		if (buf.length()==0) {
			return "⸨⸩";
		}
		return "⸨"+buf.substring(1)+"⸩";
	}
	
	static Comparator<Symbol> sorter = new Comparator<Symbol>() {
		@Override
		public int compare(Symbol o1, Symbol o2) {
			if (o1==o2) {
				return 0;
			}
			return o1.name.compareTo(o2.name);
		}
	};

	public boolean isEmpty() {
		return set.isEmpty();
	}


	public boolean addAll(SymbolSet firstSet) {
		return set.addAll(firstSet.set);
	}


	public boolean contains(Symbol sym) {
		return set.contains(sym);
	}
}
