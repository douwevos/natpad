package net.natpad.gross.ast.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class RuleRefList {

	public final RuleRefList link;
	public final RuleRef ruleRef;
	
	public RuleRefList(RuleRefList link, RuleRef ruleRef) {
		this.link = link;
		this.ruleRef = ruleRef;
	}
	

	public List<RuleRef> enlist() {
		ArrayList<RuleRef> result = new ArrayList<RuleRef>();

		RuleRefList rrl = this;
		while(rrl!=null) {
			result.add(rrl.ruleRef);
			rrl = rrl.link;
		}
		
		Collections.reverse(result);
		return result;
	}
	
}
