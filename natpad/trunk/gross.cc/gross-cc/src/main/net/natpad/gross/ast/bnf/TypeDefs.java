package net.natpad.gross.ast.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class TypeDefs {

	public final TypeDefs link;
	public final TypeDef typeDef;
	
	public TypeDefs(TypeDefs typeDefs, TypeDef typeDef) {
		this.link = typeDefs;
		this.typeDef = typeDef;
	}

	public List<TypeDef> enlist() {
		TypeDefs tds = this;
		ArrayList<TypeDef> result = new ArrayList<TypeDef>();
		while(tds!=null) {
			result.add(tds.typeDef);
			tds = tds.link;
		}
		Collections.reverse(result);
		return result;
	}

}
