package net.natpad.gross.definition.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class RhsSubList extends RhsRulePart implements RhsEnlistable {

	public final RhsSubList subList;
	public final RhsSub sub;
	
	public RhsSubList(RhsSubList subList, RhsSub sub) {
		this.subList = subList;
		this.sub = sub;
	}
	

	public List<RhsRulePart> enlist() {
		ArrayList<RhsRulePart> enlisted = new ArrayList<RhsRulePart>();
		RhsSubList tdl = this;
		while(tdl!=null) {
			enlisted.add(tdl.sub);
			tdl = tdl.subList;
		}
		Collections.reverse(enlisted);
		return enlisted;
	}

	
}
