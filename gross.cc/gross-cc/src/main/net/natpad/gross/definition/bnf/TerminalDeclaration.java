package net.natpad.gross.definition.bnf;

import java.util.List;

public class TerminalDeclaration {

	public final IdList idList;
	
	public TerminalDeclaration(IdList idList) {
		this.idList = idList;
	}

	public List<String> enlist() {
		return idList.enlist();
	}

}
