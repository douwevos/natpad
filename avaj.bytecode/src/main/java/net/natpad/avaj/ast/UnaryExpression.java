package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class UnaryExpression implements IConditionalExpression {

	
	public final IConditionalExpression expression;
	
	@Override
	public IConditionalExpression invert() {
		return expression;
	}
	
	private UnaryExpression(IConditionalExpression expression) {
		this.expression = expression;
	}
	
	
	public static IConditionalExpression create(IConditionalExpression expr) {
		if (expr instanceof UnaryExpression) {
			UnaryExpression invExpr = (UnaryExpression) expr;
			return invExpr.expression;
		}
		return new UnaryExpression(expr);
	}
	
	@Override
	public void write(Writer out) {
		out.print("!(");
		expression.write(out);
		out.print(")");
		
	}
	
}
