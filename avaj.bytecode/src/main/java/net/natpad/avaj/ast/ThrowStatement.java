package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class ThrowStatement implements IStatement {

	public final IExpression expression;
	public int statementLineNr = -1;
	
	public ThrowStatement(IExpression expression) {
		this.expression = expression;
	}
	
	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}

	
	@Override
	public void writeStatement(Writer out) {
		if (expression==null) {
			return;
		}
		out.setAtLeastLineNr(statementLineNr);
		out.print("throw");
		out.print(" ");
		expression.write(out);
		out.print(";\n");
	}
	
}
