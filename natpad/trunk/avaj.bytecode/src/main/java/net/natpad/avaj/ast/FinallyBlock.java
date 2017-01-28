package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class FinallyBlock extends Block {

	
	public FinallyBlock() {
	}
	
	
	@Override
	public void writeStatement(Writer out) {
		out.print(" finally ");
		super.writeStatement(out);
	}
	
	
	
}
