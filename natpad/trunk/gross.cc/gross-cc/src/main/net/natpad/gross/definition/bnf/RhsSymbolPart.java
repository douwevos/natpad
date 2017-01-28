package net.natpad.gross.definition.bnf;

public class RhsSymbolPart extends RhsRulePart {

	public final String symbolId;
	public final String label;
	
	public RhsSymbolPart(String symId, String label) {
		symbolId = symId;
		this.label = label;
	}

	
	@Override
	public String toString() {
		return "SymPart["+symbolId+":"+label+"]";
	}
}
