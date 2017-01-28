package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class WhileStatement implements IStatement {

	
	IExpression expression;
	public final Block block;
	public int statementLineNr = -1;
	
	public WhileStatement(Block block) {
		this.block =block;
	}
	

	public void setCondintionalExpression(IExpression expression) {
		this.expression = expression;
	}
	
	
	public IExpression getExpression() {
		return expression;
	}
	
	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}

	
	@Override
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		out.print("while (");
		expression.write(out);
		out.print(") ");
		block.writeStatement(out);
		out.print("\n");
	}
}
