package net.natpad.gross.runtime;

public class FullToken implements Token {

	public final Symbol symbol;
	public final Object value;
	public final Location location;
	public final String symbolDescription;
	
	public FullToken(Symbol symbol, Object value, Location location, String symbolDescription) {
		this.symbol = symbol;
		this.value = value;
		this.location = location;
		this.symbolDescription = symbolDescription;
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
		return "Token["+(symbol==null ? "<nil>" : symbol.name)+", value='"+value+"', location="+location+"]";
	}

}
