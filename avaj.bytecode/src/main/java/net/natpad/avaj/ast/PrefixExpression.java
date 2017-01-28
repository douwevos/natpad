package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class PrefixExpression implements IExpression {

	
	public final PostOrPrefixType type;
	public final IExpression expression;
	
	public PrefixExpression(PostOrPrefixType type, IExpression expression) {
		this.type = type;
		this.expression = expression;
	}
	
	@Override
	public void write(Writer out) {
		out.print(type.getText());
		expression.write(out);
	}
	
	

	
}
