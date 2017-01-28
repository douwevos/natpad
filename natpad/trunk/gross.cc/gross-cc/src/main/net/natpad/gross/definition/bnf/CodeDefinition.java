package net.natpad.gross.definition.bnf;

public class CodeDefinition {

	public final int id;
	public final ProductionRuleReference prodRuleRef;
	public final String codeText;
	
	public CodeDefinition(int id, ProductionRuleReference prodRuleRef, String codeText) {
		this.id = id;
		this.prodRuleRef = prodRuleRef;
		this.codeText = codeText;
	}

}
