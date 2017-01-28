package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class PostfixExpression implements IExpression {

	public final PostOrPrefixType type;
	public final IExpression expression;

	public PostfixExpression(PostOrPrefixType type, IExpression expression) {
		this.type = type;
		this.expression = expression;
	}
	
	@Override
	public void write(Writer out) {
		expression.write(out);
		out.print(type.getText());
	}
	
	
}
