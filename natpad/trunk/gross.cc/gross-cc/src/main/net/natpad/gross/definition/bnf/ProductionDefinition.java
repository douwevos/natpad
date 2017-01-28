package net.natpad.gross.definition.bnf;

public class ProductionDefinition {

	public final String lhsId;
	public final RhsRuleList rhsRuleList;
	
	public ProductionDefinition(String lhsId, RhsRuleList rhsRuleList) {
		this.lhsId = lhsId;
		this.rhsRuleList = rhsRuleList;
	}

}
