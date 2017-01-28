package net.natpad.cup.state;

import java.util.HashMap;
import java.util.Iterator;

import net.natpad.cup.FatalCupException;

public class LalrStateMap implements Iterable<LalrState> {

	private HashMap<LalrItemSet, LalrState> internalMap = new HashMap<LalrItemSet, LalrState>();

	public void add(LalrState lalrState) {
		if (internalMap.containsKey(lalrState._items)) {
			throw new FatalCupException("Attempt to construct a duplicate LALR state");
		}
		
		internalMap.put(lalrState._items, lalrState);
	}



	/**
	 * Find and return state with a given a kernel item set (or null if not
	 * found). The kernel item set is the subset of items that were used to
	 * originally create the state. These items are formed by "shifting the dot"
	 * within items of other states that have a transition to this one. The
	 * remaining elements of this state's item set are added during closure.
	 * 
	 * @param itms    the kernel set of the state we are looking for.
	 */
	public LalrState getState(LalrItemSet itms) {
		return internalMap.get(itms);
	}

	
	
	@Override
	public Iterator<LalrState> iterator() {
		return internalMap.values().iterator();
	}



	public int count() {
		return internalMap.size();
	}
}
