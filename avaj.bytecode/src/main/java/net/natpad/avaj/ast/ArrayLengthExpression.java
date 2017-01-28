package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class ArrayLengthExpression implements IExpression {

	IExpression main;
	
	public ArrayLengthExpression(IExpression main) {
		this.main = main;
	}

	
	public IExpression getMainExpression() {
		return main;
	}
	
	@Override
	public void write(Writer out) {
		main.write(out);
		out.print(".length");
	}

	
}
