package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class IfThenElse implements IStatement {

	public final IExpression expression;
	public final Block thenBlock, elseBlock;
	public int statementLineNr = -1;
	
	public IfThenElse(IExpression expression, Block thenBlock, Block elseBlock) {
		this.expression = expression;
		this.thenBlock = thenBlock;
		this.elseBlock = elseBlock;
	}

	public IfThenElse(IExpression expression, Block thenBlock) {
		this.expression = expression;
		this.thenBlock = thenBlock;
		this.elseBlock = null;
	}

	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}

	
	@Override
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		out.print("if (");
		expression.write(out);
		out.print(") ");
		thenBlock.writeStatement(out);
		if (elseBlock!=null) {
			out.print(" else ");
			elseBlock.writeStatement(out);
		}
		out.print("\n");
	}
}
