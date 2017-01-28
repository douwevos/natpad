package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class ReturnStatement implements IStatement {

	public final IExpression expression;
	public int statementLineNr = -1;

	public ReturnStatement(IExpression expression) {
		this.expression = expression;
	}

	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}
	
	@Override
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		out.print("return");
		if (expression!=null) {
			out.print(" ");
			expression.write(out);
		}
		out.print(";\n");
	}
	
}
