package net.natpad.cup.model.bnf;

import java.util.HashMap;
import java.util.Iterator;

public class ProductionList implements Iterable<Production> {

	protected HashMap<Integer, Production> map = new HashMap<Integer, Production>();


	
	@Override
	public Iterator<Production> iterator() {
		return map.values().iterator();
	}

	public void addSafe(Production production) {
		map.put(new Integer(production._index), production);
	}

	
	public int count() {
		return map.size();
	}

	public Production get(int pidx) {
		return map.get(new Integer(pidx));
	}

}
