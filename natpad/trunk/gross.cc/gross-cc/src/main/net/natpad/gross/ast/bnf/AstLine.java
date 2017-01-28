package net.natpad.gross.ast.bnf;

public class AstLine {

	public final RuleRefList ruleRefList;
	public final TypeDescription typeDescription;
	
	public AstLine(RuleRefList ruleRefList, TypeDescription typeDescription) {
		this.ruleRefList = ruleRefList;
		this.typeDescription = typeDescription;
	}

}
