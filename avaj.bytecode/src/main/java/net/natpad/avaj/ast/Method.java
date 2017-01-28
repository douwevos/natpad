package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class Method implements IStatement {

	public final Modifiers modifiers;
	public final MethodHeader methodHeader;
	public final Block statementBlock;
	public int statementLineNr = -1;
	
	public Method(Modifiers modifiers, MethodHeader methodHeader, Block statementBlock) {
		this.modifiers = modifiers;
		this.methodHeader = methodHeader;
		this.statementBlock = statementBlock;
	}
	

	public MethodHeader getMethodHeader() {
		return methodHeader;
	}

	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}

	
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		if (modifiers!=null) {
			for(Modifier modifier : modifiers) {
				out.print(modifier.text);
				out.print(" ");
			}
		}
		methodHeader.write(out);
		out.print(" ");
		statementBlock.writeStatement(out);
	}
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		String txtMods = modifiers.toString();
		buf.append(txtMods);
		if (txtMods.length()>0) {
			buf.append(" ");
		}
		buf.append(methodHeader);
		return buf.toString();
	}
	
}
