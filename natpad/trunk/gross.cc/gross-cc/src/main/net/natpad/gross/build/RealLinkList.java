package net.natpad.gross.build;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;

public class RealLinkList implements Iterable<ReduceLink> {

	public SymbolSet lookahead;
	
	public final HashSet<ReduceLink> list = new HashSet<ReduceLink>();

	public void merge(RealLinkList linkList) {
		list.addAll(linkList.list);
	}

	public boolean isEmpty() {
		return list.isEmpty();
	}
	
	
	@Override
	public Iterator<ReduceLink> iterator() {
		return list.iterator();
	}
	
	public void enlist(ArrayList<ReduceLink> enlistTo) {
		enlistTo.addAll(list);
	}


}
