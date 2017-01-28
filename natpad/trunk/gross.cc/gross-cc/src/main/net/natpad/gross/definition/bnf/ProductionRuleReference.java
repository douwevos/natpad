package net.natpad.gross.definition.bnf;

public class ProductionRuleReference {

	public final String productionLhsName;
	public final String label;
	public final Integer offset;
	
	public ProductionRuleReference(String prodName) {
		productionLhsName = prodName;
		label = null;
		this.offset = null;
	}

	public ProductionRuleReference(String prodName, String lab) {
		productionLhsName = prodName;
		this.label = lab;
		this.offset = null;
	}

	public ProductionRuleReference(String prodName, int index) {
		productionLhsName = prodName;
		this.label = null;
		this.offset= index;
	}

}
