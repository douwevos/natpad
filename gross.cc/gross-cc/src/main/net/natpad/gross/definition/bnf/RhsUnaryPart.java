package net.natpad.gross.definition.bnf;

public class RhsUnaryPart extends RhsRulePart {

	
	public final RhsRulePart rulePart;
	public final UnaryOpp opperation;
	
	public RhsUnaryPart(RhsRulePart rrpart, UnaryOpp opp) {
		rulePart = rrpart;
		opperation = opp;
	}

	public enum UnaryOpp {
		ZERO_OR_MORE,
		ONE_OR_MORE,
		OPTIONAL
	}
}
