package net.natpad.gross.runtime;

public interface TokenFactory {

	
	Token createToken(Symbol symbol, Object value, Location location, String symbolDescription);
	
	Location extractLocation(Token token);
	
}
