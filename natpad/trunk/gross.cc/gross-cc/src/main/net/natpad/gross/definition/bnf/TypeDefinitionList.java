package net.natpad.gross.definition.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class TypeDefinitionList {

	public final TypeDefinitionList typeDefList;
	public final TypeDefinition typeDef;
	
	public TypeDefinitionList(TypeDefinitionList tdlist, TypeDefinition typedef) {
		typeDefList = tdlist;
		typeDef = typedef;
	}


	public List<TypeDefinition> enlist() {
		ArrayList<TypeDefinition> enlisted = new ArrayList<TypeDefinition>();
		TypeDefinitionList tdl = this;
		while(tdl!=null) {
			enlisted.add(tdl.typeDef);
			tdl = tdl.typeDefList;
		}
		Collections.reverse(enlisted);
		return enlisted;
	}
	
}
