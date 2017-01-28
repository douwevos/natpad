package net.natpad.gross.definition.bnf;

public class AstDescription {

	
	public final ProductionRuleReference ruleRef;
	public final String codeText;
	
	public AstDescription(ProductionRuleReference ruleRef, String codeText) {
		this.ruleRef = ruleRef;
		this.codeText = codeText;
	}
}
