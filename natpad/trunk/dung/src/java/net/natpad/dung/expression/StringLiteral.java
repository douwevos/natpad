package net.natpad.dung.expression;

public class StringLiteral implements IExpressionValue {

	public final String text;
	
	public StringLiteral(String text) {
		this.text = text;
	}
	
	
	@Override
	public IExpressionValue getById(Object id) {
		return null;
	}
	
	@Override
	public String toString() {
		return text;
	}
}
