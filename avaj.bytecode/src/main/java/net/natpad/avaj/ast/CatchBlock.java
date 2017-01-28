package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class CatchBlock extends Block {

	public FieldDeclaration exceptionField;
	
	public CatchBlock(FieldDeclaration exceptionField) {
		this.exceptionField = exceptionField;
	}
	
	
	@Override
	public void writeStatement(Writer out) {
		out.print(" catch(");
		exceptionField.write(out);
		out.print(") ");
		super.writeStatement(out);
	}
	
	
	
}
