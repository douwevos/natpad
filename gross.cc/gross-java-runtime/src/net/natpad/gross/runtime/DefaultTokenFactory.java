package net.natpad.gross.runtime;

public class DefaultTokenFactory implements TokenFactory {

	@Override
	public Token createToken(Symbol symbol, Object value, Location location, String symbolDescription) {
		return new DefaultToken(symbol, value);
	}
	
	@Override
	public Location extractLocation(Token token) {
		return null;
	}
	
}
