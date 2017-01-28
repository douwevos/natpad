package net.natpad.gross.build;

import java.io.PrintStream;
import java.util.HashSet;
import java.util.Iterator;

class DotLink implements Iterable<DotState> {

	public final DotState dotState;
	private final HashSet<DotState> referredBy = new HashSet<DotState>();
	
	
	public DotLink(DotState dotState) {
		this.dotState = dotState;
	}
	
	
	@Override
	public String toString() {
		return "DotLink["+dotState+"]";
	}

	public boolean addReferredBy(DotState dotState) {
		return referredBy.add(dotState);
	}
	
	public boolean addReferredBy(DotLink dotLink) {
		return referredBy.addAll(dotLink.referredBy);
	}

	@Override
	public Iterator<DotState> iterator() {
		return referredBy.iterator();
	}
	
	public void describe(PrintStream out) {
		out.println("DotLink");
		out.println("   State:    "+dotState);
		out.println("   Referrers:");
		for (DotState dotState : referredBy) {
			out.println("     "+dotState);
		}
		out.println();
		
	}
}