package net.natpad.cup.model.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import net.natpad.cup.FatalCupException;

public class NonTerminalList implements Iterable<NonTerminal> {

	protected HashMap<Integer, NonTerminal> map = new HashMap<Integer, NonTerminal>();
	protected HashMap<String, NonTerminal> namedMap = new HashMap<String, NonTerminal>();

	

	
	public NonTerminalList() {
	}
	
	@Override
	public Iterator<NonTerminal> iterator() {
		return map.values().iterator();
	}

	public void add(NonTerminal nonTerminal) {
//		System.out.println("adding nonterm-"+nonTerminal);
		map.put(new Integer(nonTerminal._index), nonTerminal);
		NonTerminal conflict = namedMap.put(nonTerminal._name, nonTerminal);
		if (conflict != null) {
			throw new FatalCupException("Duplicate non-terminal (" + nonTerminal._name + ") created");
		}
	}

	
	public int count() {
		return map.size();
	}

	public NonTerminal get(int pidx) {
		return map.get(new Integer(pidx));
	}

	public NonTerminal get(String withName) {
		return namedMap.get(withName);
	}

	
	public List<NonTerminal> getOrderedByIndex() {
		List<NonTerminal> result = new ArrayList<NonTerminal>(map.values());
		Collections.sort(result, new Comparator<NonTerminal>() {
			@Override
			public int compare(NonTerminal term1, NonTerminal term2) {
				if (term1==term2) {
					return 0;
				}
				if (term1==null) {
					return -1;
				}
				if (term2==null) {
					return 1;
				}
				if (term1._index<term2._index) {
					return -1;
				} else if (term1._index>term2._index) {
					return 1;
				}
				return 0;
			}
		});
		return result;
	}

	
}
