package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public interface IStatement {

	public void writeStatement(Writer out);

	public void setAtLeastLineNumber(int statementLineNr);
}
