package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class WrappedConditionalExpression implements IConditionalExpression {

	public final IExpression expression;
	
	public WrappedConditionalExpression(IExpression expression) {
		this.expression = expression;
	}
	
	public static IConditionalExpression ensureIsConditional(IExpression expression) {
		if (expression instanceof IConditionalExpression) {
			return (IConditionalExpression) expression;
		}
		return new WrappedConditionalExpression(expression);
	}

	
	public IConditionalExpression invert() {
		return UnaryExpression.create(this);
	}
	
	public void write(Writer out) {
		expression.write(out);
	}
}
