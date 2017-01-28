package net.natpad.gross.build;

import net.natpad.gross.runtime.Symbol;

public class ProductionPart {

	public final Symbol symbol;
	public final String label;

	public ProductionPart(Symbol symbol) {
		this.symbol = symbol;
		this.label = null;
	}

	
	public ProductionPart(Symbol symbol, String label) {
		this.symbol = symbol;
		this.label = label;
	}
	
	
	@Override
	public int hashCode() {
		return symbol.hashCode();
	}
	
	@Override
	public boolean equals(Object other) {
		if (other instanceof ProductionPart) {
			ProductionPart that = (ProductionPart) other;
			return that.symbol == symbol;
		}
		return false;
	}
	
	@Override
	public String toString() {
		return symbol==null ? "<nil>" : symbol.name;
	}
}
