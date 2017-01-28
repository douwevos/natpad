package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.util.TextUtil;

public class InvocationExpression implements IExpression {

	public final IExpression instanceExpression;
	public final Identifier methodName;
	private final IExpression arguments[];
	
	public InvocationExpression(IExpression instanceExpression, IExpression ... arguments) {
		this.instanceExpression = instanceExpression;
		if (instanceExpression==null) {
			throw new RuntimeException();
		}
		methodName = null;
		this.arguments = arguments;
//		assert instanceExpression==null;
	}

	public InvocationExpression(IExpression instanceExpression, Identifier methodName, IExpression ... arguments) {
		this.instanceExpression = instanceExpression;
		if (instanceExpression==null) {
			throw new RuntimeException();
		}
		this.methodName = methodName;
		this.arguments = arguments;
//		assert instanceExpression==null;
	}

	
	
	public IExpression[] getArguments() {
		return arguments;
	}
	
	@Override
	public void write(Writer out) {
		if (instanceExpression.equals(new Identifier("this"))) {
			if (methodName!=null) {
				methodName.write(out);
			} else {
				instanceExpression.write(out);				
			}
		} else {
			instanceExpression.write(out);
			if (methodName!=null) {
				out.print(".");
				methodName.write(out);
			}
		}
		out.print("(");
		
		if (arguments!=null && arguments.length>0) {
			boolean isFirst = true;
			for(IExpression expr : arguments) {
				if (isFirst) {
					isFirst = false;
				} else {
					out.print(", ");
				}
				if (expr==null) {
					out.print("???");
				} else {
					expr.write(out);
				}
			}
		}
		
		out.print(")");
	}
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		if (arguments!=null && arguments.length>0) {
			boolean isFirst = true;
			for(IExpression expr : arguments) {
				if (isFirst) {
					isFirst = false;
				} else {
					buf.append(", ");
				}
				if (expr==null) {
					buf.append("???");
				} else {
					buf.append(expr);
				}
			}
		}
		return TextUtil.getName(getClass())+"[inst="+instanceExpression+", "+methodName+" (("+buf+"))]";
	}
}
