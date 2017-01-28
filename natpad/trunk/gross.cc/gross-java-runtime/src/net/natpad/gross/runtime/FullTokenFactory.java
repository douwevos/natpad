package net.natpad.gross.runtime;

public class FullTokenFactory implements TokenFactory {

	@Override
	public Token createToken(Symbol symbol, Object value, Location location, String symbolDescription) {
		return new FullToken(symbol, value, location, symbolDescription);
	}
	
	@Override
	public Location extractLocation(Token token) {
		if (token instanceof FullToken) {
			return ((FullToken) token).location;
		}
		return null;
	}
	
}
