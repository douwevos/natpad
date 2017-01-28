package net.natpad.gross.runtime;

public class DefaultToken implements Token {

	public final Symbol symbol;
	public final Object value;
	
	public DefaultToken(Symbol symbol, Object value) {
		this.symbol = symbol;
		this.value = value;
	}
	
	
	@Override
	public Symbol getSymbol() {
		return symbol;
	}
	
	@Override
	public Object getValue() {
		return value;
	}
	
	
	@Override
	public String toString() {
		return "Token["+(symbol==null ? "<nil>" : symbol.name)+", value='"+value+"']";
	}
	
}
