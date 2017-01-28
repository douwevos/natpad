package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.util.TextUtil;

public class Literal implements IExpression {

	
	public static final Literal LITERAL_TRUE		= new Literal(new Boolean(true));
	public static final Literal LITERAL_FALSE		= new Literal(new Boolean(false));
	
	public final Object value;
	
	public Literal(Object value) {
		this.value = value;
	}
	
	
	@Override
	public boolean equals(Object other) {
		if (other==this) {
			return true;
		}
		if (other instanceof Literal) {
			Literal that = (Literal)  other;
			return ((that.value==null && value==null) || (that.value!=null && value!=null && value.equals(that.value)));
		}
		return false;
	}
	
	@Override
	public void write(Writer out) {
		out.print(""+value);
	}
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"[value="+value+"]";
	}
}
