package net.natpad.cup.model.bnf;


public class ActionDeclaration {

	public final String label;
	public final String stackType;
	public final int offset;
	
	public ActionDeclaration(String label, String stackType, int offset) {
		this.label = label;
		this.stackType = stackType;
		this.offset = offset;
	}
	
}
