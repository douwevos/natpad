package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class Assignment implements IStatement {

	public final IExpression expression;
	public final IExpression identifier;
	public int statementLineNr = -1;
	
	public Assignment(IExpression identifier, IExpression expression) {
		if (expression==null) {
			throw new RuntimeException();
		}
		this.expression = expression;
		this.identifier = identifier;
	}

	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}

	
	@Override
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		write(out);
		out.print(";\n");
	}
	
	public void write(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		identifier.write(out);
		out.print(" = ");
		expression.write(out);
//		expression
	}
	
}
