package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.IExpression;

public class ExpressionHolder {

	private IExpression expression;
	
	public ExpressionHolder() {
	}
	
	
	public void setExpression(IExpression expression) {
		this.expression = expression;
	}
	
	
	public IExpression getExpression() {
		return expression;
	}
	
}
