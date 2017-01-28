package net.natpad.gross.definition.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class RhsSub extends RhsRulePart implements RhsEnlistable {

	public final RhsSub sub;
	public final RhsRulePart rec;
	
	public RhsSub(RhsSub sub, RhsRulePart rec) {
		this.sub = sub;
		this.rec = rec;
	}


	public List<RhsRulePart> enlist() {
		ArrayList<RhsRulePart> enlisted = new ArrayList<RhsRulePart>();
		RhsSub tdl = this;
		while(tdl!=null) {
			enlisted.add(tdl.rec);
			tdl = tdl.sub;
		}
		Collections.reverse(enlisted);
		return enlisted;
	}

	
}
