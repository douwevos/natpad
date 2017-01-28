package net.natpad.gross.definition.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class CodeDefinitionList {

	public final CodeDefinitionList codeDefList;
	public final CodeDefinition codeDef;
	
	public CodeDefinitionList(CodeDefinitionList codeDefList, CodeDefinition codeDef) {
		this.codeDefList = codeDefList;
		this.codeDef = codeDef;
	}

	public List<CodeDefinition> enlist() {
		ArrayList<CodeDefinition> enlisted = new ArrayList<CodeDefinition>();
		CodeDefinitionList tdl = this;
		while(tdl!=null) {
			enlisted.add(tdl.codeDef);
			tdl = tdl.codeDefList;
		}
		Collections.reverse(enlisted);
		return enlisted;
	}

}
