package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;

public class ForLoopStatement implements IStatement {

	
	public final IAstNode forInit;
	public final IExpression forCondition;
	public final IAstNode forUpdate;
	public final Block body;
	public int statementLineNr = -1;
	
	
	public ForLoopStatement(IAstNode forInit, IExpression forCondition, IAstNode forUpdate, Block body) {
		this.forInit = forInit;
		this.forCondition = forCondition;
		this.forUpdate = forUpdate;
		this.body = body;
	}
	
	
	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}

	
	@Override
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		out.print("for(");
		forInit.write(out);
		out.print("; ");
		forCondition.write(out);
		out.print("; ");
		forUpdate.write(out);
		out.print(") ");
		body.writeStatement(out);
		out.print("\n");
	}

}
