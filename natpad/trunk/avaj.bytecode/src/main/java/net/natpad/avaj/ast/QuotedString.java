package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.util.Util;

public class QuotedString implements IExpression {

	public final String value;
	
	public QuotedString(String value) {
		this.value = value;
	}
	
	@Override
	public void write(Writer out) {
		out.print("\"");
		out.print(value);
		out.print("\"");
	}
	
	@Override
	public boolean equals(Object other) {
		if (other==this) {
			return true;
		}
		if (other instanceof QuotedString) {
			QuotedString that = (QuotedString) other;
			return Util.nullSafeEquals(value, that.value);
		}
		return false;
	}
	
	@Override
	public String toString() {
	    return "QuotedString["+value+"]";
	}
	
}
