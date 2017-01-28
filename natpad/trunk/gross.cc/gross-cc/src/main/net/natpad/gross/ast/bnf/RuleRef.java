package net.natpad.gross.ast.bnf;

import java.util.Objects;

public class RuleRef {

	public final String lhsName;
	public final String label;
	
	public RuleRef(String lhsName, String label) {
		this.lhsName = lhsName;
		this.label = label;
	}
	
	
	@Override
	public int hashCode() {
		return lhsName.hashCode();
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj instanceof RuleRef) {
			RuleRef other = (RuleRef) obj;
			return Objects.equals(other.label, label)
					&& Objects.equals(other.lhsName, lhsName);
		}
		return false;
	}

	
	@Override
	public String toString() {
		return ""+lhsName+"##"+label;
	}
}
