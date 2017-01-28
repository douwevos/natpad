package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class MethodHeader {

	public final Identifier methodName;
	public final DeclarationType returnType;
	public final boolean isConstructor;
	private final FieldDeclaration args[];
	private final Name exceptions[];
	
	public MethodHeader(Identifier methodName, boolean isConstructor, DeclarationType returnType, FieldDeclaration args[], Name exceptions[]) {
		this.methodName = methodName;
		this.returnType = returnType;
		this.args = args;
		this.isConstructor = isConstructor;
		this.exceptions = exceptions;
	}
	
	

	public FieldDeclaration[] getArgs() {
		return args;
	}

	public int exceptionCount() {
		return exceptions==null ? -1 : exceptions.length;
	}
	
	public Name exceptionAt(int index) {
		if (exceptions==null || index<0 || index>=exceptions.length) {
			return null;
		}
		return exceptions[index];
	}
	

	public void write(Writer out) {
		if (!isConstructor) {
			returnType.write(out);
			out.print(" ");
		}
		methodName.write(out);
		out.print("(");
		boolean isFirst = true;
		for(FieldDeclaration arg : args) {
			if (isFirst) {
				isFirst = false;
			} else {
				out.print(", ");
			}
			arg.write(out);
		}
		out.print(")");
		if (exceptions!=null && exceptions.length>0) {
			out.print(" throws ");
			isFirst = true; 
			for(Name typeRef : exceptions) {
				if (isFirst) {
					isFirst = false;
				} else {
					out.print(", ");
				}
				out.print(typeRef.createFQN());
			}
		}
	}

	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		if (!isConstructor) {
			buf.append(returnType).append(" ");
			buf.append(returnType.type).append(" ");
			
		}
		buf.append(methodName).append("(");
		boolean isFirst = true;
		for(FieldDeclaration arg : args) {
			if (isFirst) {
				isFirst = false;
			} else {
				buf.append(", ");
			}
			buf.append(arg);
		}
		buf.append(")");
		return buf.toString();
	}


}
