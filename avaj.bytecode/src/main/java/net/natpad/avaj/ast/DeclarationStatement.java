package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class DeclarationStatement implements IStatement, IAstNode {

	public final DeclarationType declarationType;
	public final Assignment assignment;
	public int statementLineNr = -1;
	
	public DeclarationStatement(DeclarationType declarationType, Assignment assignment) {
		this.declarationType = declarationType;
		this.assignment = assignment;
	}
	
	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}
	
	@Override
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		if (declarationType!=null) {
			declarationType.write(out);
		} else {
			out.print("???");
		}
		out.print(" ");
		assignment.writeStatement(out);
//		out.print(";\n");
	}		
	
	
	@Override
	public void write(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		if (declarationType!=null) {
			declarationType.write(out);
		} else {
			out.print("???");
		}
		out.print(" ");
		assignment.write(out);
//		out.print(";\n");
		
	}
	
}
