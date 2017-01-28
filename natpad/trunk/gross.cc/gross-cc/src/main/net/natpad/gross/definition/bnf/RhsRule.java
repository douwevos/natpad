package net.natpad.gross.definition.bnf;

import java.util.ArrayList;
import java.util.List;

public class RhsRule extends RhsRulePart implements RhsEnlistable {

	public final String label;
	public final RhsRulePartList partList;
	
	public RhsRule(String plab, RhsRulePartList rrpartlist) {
		this.label = plab;
		partList = rrpartlist;
	}
	
	
	public List<RhsRulePart> enlist() {
		if (partList==null) {
			return new ArrayList<RhsRulePart>();
		}
		return partList.enlist();
	}

	
	@Override
	public String toString() {
		return "RhsRule["+partList+"]";
	}
}
