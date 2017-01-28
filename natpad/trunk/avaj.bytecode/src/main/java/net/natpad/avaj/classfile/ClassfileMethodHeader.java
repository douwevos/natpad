package net.natpad.avaj.classfile;

import java.util.ArrayList;

import net.natpad.avaj.ast.Argument;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.Name;
import net.natpad.avaj.classfile.bytecode.Type;
import net.natpad.avaj.log.Logger;


public class ClassfileMethodHeader {

	static Logger log = Logger.getInstance(Classfile.class);
	
	public final DeclarationType returnType;
	public final String methodName;
	public final boolean isConstructor;
	private final Argument args[];
	
	private ParsedExcpetions parsedExcpetions;
	
	
	public ClassfileMethodHeader(String methodName, boolean isConstructor, DeclarationType returnType, Argument args[]) {
		this.methodName = methodName;
		this.returnType = isConstructor ? null : returnType;
		this.args = args;
		this.isConstructor = isConstructor;
	}
	
	

	public ClassfileMethodHeader(Name className, String methodName, String description) {
		log.debug("methodName="+methodName+", text="+description);
		boolean isConstructor = false;
		if (methodName.equals("<init>")) {
			methodName = className.getShortTypeName();
			isConstructor = true;
		}
		int idx = 0;
		boolean isParameter = true;
		DeclarationType returnType = null;
		ArrayList<Argument> argumentList = new ArrayList<Argument>();
		if (description.charAt(idx++)=='(') {
			boolean expectRef = false;
			int referenceCount = 0;
			Type dPrimitive = null;
			StringBuilder extBuf = new StringBuilder();
			while(idx<description.length()) {
				boolean storeAndCreateNew = false;
				char ch = description.charAt(idx);
				if (expectRef) {
					if (ch==';') {
						storeAndCreateNew = true;
					} else {
						extBuf.append(ch);
					}
				} else {
					switch(ch) {
						case 'B' : {
							dPrimitive = Type.BYTE;
							storeAndCreateNew = true;
						} break;
						case 'C': {
							dPrimitive = Type.CHAR;
							storeAndCreateNew = true;
						} break;
						case 'D': {
							dPrimitive = Type.DOUBLE;
							storeAndCreateNew = true;
						} break;
						case 'F': {
							dPrimitive = Type.FLOAT;
							storeAndCreateNew = true;
						} break;
						case 'I': {
							dPrimitive = Type.INT;
							storeAndCreateNew = true;
						} break;
						case 'J': {
							dPrimitive = Type.LONG;
							storeAndCreateNew = true;
						} break;
						case 'L': {
							expectRef = true;
						} break;
						case 'S': {
							dPrimitive = Type.SHORT;
							storeAndCreateNew = true;
						} break;
						case 'Z': {
							dPrimitive = Type.BOOLEAN;
							storeAndCreateNew = true;
						} break;
						case 'V': {
							dPrimitive = Type.VOID;
							storeAndCreateNew = true;
						} break;
						case '[': {
							referenceCount++;
						} break;
						case ')' : {
							isParameter = false;
						} break;
							
					}
					
				}
				if (storeAndCreateNew) {
					DeclarationType desc = null;
					if (expectRef) {
						Name referencTypeName = new Name(extBuf.toString(), true);
						desc = new DeclarationType(referencTypeName, referenceCount);
					} else {
						desc = new DeclarationType(dPrimitive, referenceCount);
					}
					if (isParameter) {
						String name = new String("arg"+argumentList.size());
						argumentList.add(new Argument(desc, name));
					} else {
						returnType = desc;
						break;
					}
					referenceCount = 0;
					expectRef = false;
					dPrimitive = null;
					extBuf.setLength(0);
				}
				idx++;
			}
		}
		this.args = new Argument[argumentList.size()];
		argumentList.toArray(this.args);
		this.methodName = methodName;
		this.isConstructor = isConstructor;
		this.returnType = returnType;
	}

	
	public void setParsedExceptions(ParsedExcpetions parsedExcpetions) {
		this.parsedExcpetions = parsedExcpetions;
    }
	
	public ParsedExcpetions getParsedExcpetions() {
	    return parsedExcpetions;
    }

	

	public Argument[] getArgs() {
		return args;
	}

	public int argumentCount() {
		return args.length;
	}

	public Argument argumentAt(int fieldIdx) {
		return args[fieldIdx];
	}


	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		if (!isConstructor) {
			buf.append(returnType).append(" ");
		}
		buf.append(methodName).append("(");
		boolean isFirst = true;
		for(Argument arg : args) {
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
