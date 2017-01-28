package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class CastExpression implements IExpression {

	public final IExpression expression;
	public final DeclarationType castType;
	
	
	public CastExpression(DeclarationType castType, IExpression expression) {
		this.expression = expression;
		this.castType = castType;
	}
	
	
	@Override
	public void write(Writer out) {
		out.print("(");
		castType.write(out);
		out.print(") ");
		expression.write(out);
	}
	
}
