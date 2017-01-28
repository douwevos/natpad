package net.natpad.avaj.ast;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import net.natpad.avaj.ast.formater.Writer;


public class ImportStatement implements IStatement {

	private final Identifier literals[];
	public int statementLineNr = -1;


	
	public ImportStatement(Name name) {
		literals = new Identifier[name.count()];
		for(int idx=literals.length-1; idx>=0; idx--) {
			literals[idx] = new Identifier(name.get(idx));
		}
	}


	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}

	
	public List<Identifier> getLiterals() {
		ArrayList<Identifier> result = new ArrayList<Identifier>();
		result.addAll(Arrays.asList(literals));
		return result;
	}
	
	
	public boolean write(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		out.print("import ");
		boolean isFirst=true;
		for(Identifier literal : literals) {
			if (isFirst) {
				isFirst = false;
			} else {
				out.print(".");
			}
			literal.write(out);
		}
		return !isFirst;
	}
	
	@Override
	public void writeStatement(Writer out) {
		write(out);
		out.print(";\n");
	}


	public int compare(ImportStatement o2) {
		int result = 0;
		for(int idx=0; result==0 && idx<o2.literals.length && idx<literals.length; idx++) {
			result = literals[idx].text.compareTo(o2.literals[idx].text);
		}
		if (result==0) {
			if (literals.length<o2.literals.length) {
				result = -1;
			} else if (literals.length>o2.literals.length) {
				result = 1;
			}
		}
		
		return result;
	}
	
}
