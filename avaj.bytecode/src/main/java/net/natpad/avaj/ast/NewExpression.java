package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.util.TextUtil;

public class NewExpression implements IExpression {

	public Identifier identifier;
	private final IExpression dimExpressions[];
	
	public NewExpression(Identifier identifier, IExpression ... dimExpressions) {
		this.identifier = identifier;
		this.dimExpressions = dimExpressions;
	}
	
	public int getDimExpressionCount() {
		return dimExpressions.length;
	}

	public IExpression getDimExpressionAt(int index) {
		if (index<0 || index>=dimExpressions.length) {
			return null;
		}
		return dimExpressions[index];
	}

	
	@Override
	public void write(Writer out) {
		out.print("new ");
		identifier.write(out);
		if (dimExpressions!=null) {
			for(IExpression expression : dimExpressions) {
				out.print("[");
				expression.write(out);
				out.print("]");
			}
		}
	}
	
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"["+identifier+"]";
	}
	
}
