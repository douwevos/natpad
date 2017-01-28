package net.natpad.gross.definition.bnf;

public class TypeDefinition {

	public final String type;
	public final IdList symbolIdList;
	
	public TypeDefinition(String symType, IdList idList) {
		this.type = symType;
		this.symbolIdList = idList;
	}

}
