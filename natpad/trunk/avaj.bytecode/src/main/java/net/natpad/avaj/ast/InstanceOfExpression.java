package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class InstanceOfExpression implements IExpression {

	public final IExpression expression;
	public final DeclarationType castType;
	
	
	public InstanceOfExpression(DeclarationType castType, IExpression expression) {
		this.expression = expression;
		this.castType = castType;
	}
	
	
	@Override
	public void write(Writer out) {
		expression.write(out);
		out.print(" instanceof ");
		castType.write(out);
	}
	
}
