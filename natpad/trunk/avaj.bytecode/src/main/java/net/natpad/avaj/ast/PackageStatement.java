package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class PackageStatement implements IStatement {

	public final Identifier packageName;
	public int statementLineNr = -1;

	public PackageStatement(Identifier packageName) {
		this.packageName = packageName;
	}

	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}

	
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		out.print("package ");
		packageName.write(out);
		out.print(";\n");
	}
	
}
