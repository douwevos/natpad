package net.natpad.gross.definition.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class RhsRulePartList {
	
	
	public final RhsRulePartList partList;
	public final RhsRulePart part;
	
	public RhsRulePartList(RhsRulePartList rrpartlist, RhsRulePart rrpart) {
		partList = rrpartlist;
		part = rrpart;
	}
	
	public List<RhsRulePart> enlist() {
		ArrayList<RhsRulePart> enlisted = new ArrayList<RhsRulePart>();
		RhsRulePartList tdl = this;
		while(tdl!=null) {
			enlisted.add(tdl.part);
			tdl = tdl.partList;
		}
		Collections.reverse(enlisted);
		return enlisted;
	}

	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		List<RhsRulePart>  l = enlist();
		for(RhsRulePart pp : l) {
			if (buf.length()>0) {
				buf.append(",");
			}
			buf.append(pp);
		}
		return "["+buf.toString()+"]";
	}

	

}
