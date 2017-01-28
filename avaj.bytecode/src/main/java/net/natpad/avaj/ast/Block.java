package net.natpad.avaj.ast;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import net.natpad.avaj.ast.formater.Writer;


public class Block implements IStatement, Iterable<IStatement> {

	private List<IStatement> statementList = new ArrayList<IStatement>();
	public int statementLineNr = -1;

	
	@Override
	public final void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}

	
	public final void append(IStatement statement) {
		statementList.add(statement);
	}
	
	
	public final IStatement get(int index) {
		if (index>=0 && index<statementList.size()) {
			return statementList.get(index);
		}
		return null;
	}
	
	@Override
	public final Iterator<IStatement> iterator() {
		return statementList.iterator();
	}
	
	
	
	@Override
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		out.print("{\n");
		out.increaseIndent();
		for(IStatement statement : statementList) {
			statement.writeStatement(out);
		}
		out.decreaseIndent();
		out.print("}");
	}

	public final void removeLastReturn() {
		if (statementList.isEmpty()) {
			return;
		}
		IStatement statement = statementList.get(statementList.size()-1);
		if (statement instanceof ReturnStatement) {
			ReturnStatement rs = (ReturnStatement) statement;
			if (rs.expression==null) {
				statementList.remove(statementList.size()-1);
			}
		}
    }

}
