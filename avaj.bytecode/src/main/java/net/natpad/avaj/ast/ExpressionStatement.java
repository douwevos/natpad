package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class ExpressionStatement implements IStatement, IAstNode {

	public final IExpression expression;
	public int statementLineNr = -1;

	public ExpressionStatement(IExpression expression) {
		this.expression = expression;
	}

	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}

	@Override
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		expression.write(out);
		out.print(";\n");
	}
	
	
	@Override
	public void write(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		expression.write(out);
	}
}
