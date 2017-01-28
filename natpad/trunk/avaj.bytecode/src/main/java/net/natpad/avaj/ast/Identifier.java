package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.util.Util;

public class Identifier implements IConditionalExpression {

	public final String text;
	
	public Identifier(String text) {
		this.text = text;
	}

	public Identifier(Name name) {
		this.text = name.createFQN();
	}

	
	@Override
	public void write(Writer out) {
		out.print(text);	
	}
	
	
	
	@Override
	public IConditionalExpression invert() {
		return UnaryExpression.create(this);
	}
	
	@Override
	public boolean equals(Object other) {
		if (other==this) {
			return true;
		}
		if (other instanceof Identifier) {
			Identifier that = (Identifier) other;
			return Util.nullSafeEquals(that.text, text);
		}
		return false;
	}
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"["+text+"]";
	}
	
}
