package net.natpad.gross.build;

import java.io.PrintStream;
import java.util.HashMap;
import java.util.HashSet;

import net.natpad.gross.runtime.Symbol;

public class BuildEntry {

	public final int id;
	public final Kernel kernel;
	public final HashMap<SymbolKey, Kernel> forwardMap = new HashMap<SymbolKey, Kernel>();
	public final HashMap<SymbolKey, Kernel> nonClosedMap = new HashMap<SymbolKey, Kernel>();
	public LrarState state;
	
	
	public BuildEntry(Kernel mainDotSet, int id) {
		this.kernel = mainDotSet;
		this.id = id;
	}
	
	
	public LrarState buildState() {
		state = new LrarState(kernel, id);
		return state;
	}

	public void describe(PrintStream out, HashMap<DotState, DotLink> linkMap) {
		out.println("state["+id+"]");
		out.println("   kernel: "+kernel.id);
		for(DotLink dsWithLL : kernel) {
			out.println("     ≣ "+dsWithLL.dotState);
			if (linkMap!=null) {
				for(DotState referredBy : dsWithLL) {
					String indent = "            ";
					describeLink(out, linkMap, referredBy, indent, new HashSet<DotState>());
				}
			}
		}
		out.println("   expect:");
		for(SymbolKey symbol : forwardMap.keySet()) {
			Kernel dotSet = forwardMap.get(symbol);
			out.println("      "+symbol+" [dotset:"+dotSet.id+"]");
			for(DotLink dotState : dotSet) {
				out.println("         "+dotState.dotState);
			}
		}
	}


	private void describeLink(PrintStream out, HashMap<DotState, DotLink> linkMap, DotState referredBy, String indent, HashSet<DotState> done) {
		DotLink childDotLink = linkMap.get(referredBy);
		out.println(indent+""+childDotLink.dotState);
		indent = indent+"  ";
		for(DotState sub : childDotLink) {
			if (done.add(sub)) {
				describeLink(out, linkMap, sub, indent, done);
			}
		}
	}

}
