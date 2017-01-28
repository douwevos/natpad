package net.natpad.cup.model.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import net.natpad.cup.FatalCupException;

public class TerminalList implements Iterable<Terminal> {

	protected HashMap<Integer, Terminal> map = new HashMap<Integer, Terminal>();
	protected HashMap<String, Terminal> namedMap = new HashMap<String, Terminal>();

	public Iterator<Terminal> iterator() {
		return map.values().iterator();
	}

	public void add(Terminal terminal) {
		map.put(new Integer(terminal._index), terminal);
		Terminal conflict = namedMap.put(terminal._name, terminal);
		if (conflict != null) {
			throw new FatalCupException("Duplicate terminal (" + terminal._name + ") created");
		}
	}

	
	public int count() {
		return map.size();
	}

	public Terminal get(int pidx) {
		return map.get(new Integer(pidx));
	}

	public Terminal get(String withName) {
		return namedMap.get(withName);
	}

	
	public List<Terminal> getOrderedByIndex() {
		List<Terminal> result = new ArrayList<Terminal>(map.values());
		Collections.sort(result, new Comparator<Terminal>() {
			@Override
			public int compare(Terminal term1, Terminal term2) {
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
