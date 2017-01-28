package net.natpad.gross.build;

import net.natpad.gross.runtime.Symbol;

public class SymbolKey {

	public final Symbol symbol;
	public final boolean isNullified;
	
	public SymbolKey(Symbol symbol, boolean isNullified) {
		this.symbol = symbol;
		this.isNullified = isNullified;
	}
	
	@Override
	public int hashCode() {
		return symbol.hashCode() + (isNullified ? 13 : 7);
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj == this) {
			return true;
		}
		if (obj instanceof SymbolKey) {
			SymbolKey other = (SymbolKey) obj;
			return other.symbol == symbol && other.isNullified==isNullified;
		}
		return false;
	}
	
	
	@Override
	public String toString() {
		return (isNullified ? "-" : "+") + symbol.name;
	}
	
	
}
