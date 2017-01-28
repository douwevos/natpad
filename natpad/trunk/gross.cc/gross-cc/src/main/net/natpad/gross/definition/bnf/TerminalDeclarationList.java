package net.natpad.gross.definition.bnf;

import java.util.ArrayList;
import java.util.Collections;

public class TerminalDeclarationList {

	
	public final TerminalDeclarationList termDeclList;
	public final TerminalDeclaration termDecl;
	
	public TerminalDeclarationList(TerminalDeclarationList termDeclList, TerminalDeclaration termDecl) {
		this.termDeclList = termDeclList;
		this.termDecl = termDecl;
	}
	
	public TerminalDeclaration[] enlist() {
		ArrayList<TerminalDeclaration> enlisted = new ArrayList<TerminalDeclaration>();
		TerminalDeclarationList tdl = this;
		while(tdl!=null) {
			enlisted.add(tdl.termDecl);
			tdl = tdl.termDeclList;
		}
		Collections.reverse(enlisted);
		return enlisted.toArray(new TerminalDeclaration[enlisted.size()]);
	}

}
