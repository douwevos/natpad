package net.natpad.gross.definition.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class RhsRuleList implements RhsEnlistable {

	public final RhsRuleList rlsList;
	public final RhsRule rule;
	
	public RhsRuleList(RhsRuleList rlsList, RhsRule rule) {
		this.rlsList = rlsList;
		this.rule = rule;
	}

	public List<RhsRulePart> enlist() {
		ArrayList<RhsRulePart> enlisted = new ArrayList<RhsRulePart>();
		RhsRuleList tdl = this;
		while(tdl!=null) {
			enlisted.add(tdl.rule);
			tdl = tdl.rlsList;
		}
		Collections.reverse(enlisted);
		return enlisted;
		
	}
	
	
}
