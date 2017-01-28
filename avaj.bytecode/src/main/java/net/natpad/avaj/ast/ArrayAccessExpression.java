package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class ArrayAccessExpression implements IExpression {

	IExpression main;
	IExpression dimExpression;
	
	public ArrayAccessExpression(IExpression main, IExpression dimExpression) {
		this.main = main;
		this.dimExpression = dimExpression;
	}
	
	
	public IExpression getMainExpression() {
		return main;
	}
	
	public IExpression getDimExpression() {
		return dimExpression;
	}
	
	@Override
	public void write(Writer out) {
		main.write(out);
		out.print("[");
		dimExpression.write(out);
		out.print("]");
	}
	
}
