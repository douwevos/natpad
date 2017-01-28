package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class SynchronizedBlock extends Block {

	IExpression syncExpression;
	
	public SynchronizedBlock(IExpression syncExpression) {
		this.syncExpression = syncExpression;
		
	}
	
	@Override
	public void writeStatement(Writer out) {
		out.print("synchronized(");
		syncExpression.write(out);
		out.print(") ");
		super.writeStatement(out);
	}

	
}
