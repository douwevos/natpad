package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class FieldAccessExpression implements IExpression {

	public final IExpression resolvedName;
	public final Identifier fieldName;
	
	public FieldAccessExpression(IExpression resolvedName, Identifier fieldName) {
		this.resolvedName = resolvedName;
		this.fieldName = fieldName;
    }

	
	public void write(Writer out) {
		if (resolvedName!=null) {
			resolvedName.write(out);
			out.print(".");
		}
		fieldName.write(out);
	}
	
}
