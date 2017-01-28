package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class BreakStatement implements IStatement {

	
	public int statementLineNr = -1;
	
	
	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}

	
	@Override
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		out.print("break;\n");
		
	}
}
