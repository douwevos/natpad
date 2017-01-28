package net.natpad.gross.export.glibc;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

import net.natpad.gross.export.glibc.Type.Field;

public class Method {

	public String methodName;
	public ClassName returnType;

	public boolean isPrivate = false;
	public boolean isConstructor = false;
	
	public List<Argument> arguments = new ArrayList<Argument>();

	public String content;
	
	public Method(String methodName, ClassName returnType) {
		this.methodName = methodName;
		this.returnType = returnType;
	}
	
	
	public void writeC(Type thisType, PrintWriter out) {
		String indent = "";

		if (returnType!=null) {
			out.print(indent+returnType.fullTypePtr()+methodName+"(");
		} else {
			out.print(indent+"void "+methodName+"(");
		}

		boolean needComma = false;
		if (!isConstructor) {
			out.print(thisType.className.fullTypePtr()+"self");
			needComma = true;
		}
		HashSet<String> argumentNames = new HashSet<String>();
		for(Argument argument :arguments) {
			if (!needComma) {
				needComma = true;
			} else {
				out.print(", ");
				
			}
			out.print(argument.type.fullTypePtr()+argument.label);
			argumentNames.add(argument.label);
		}
		out.println(") {");

		ClassName cn = thisType.className;
		String cmlName = cn.camelPrefix+cn.camelPostfix;
		
		if (isConstructor) {
			out.println("	"+cmlName+" *instance = g_object_new("+cn.upPrefix+"_TYPE_"+cn.upPostfix+", NULL);");
			if (thisType.hasPrivateFields) {
				out.println("	"+cmlName+"Private *priv = "+cn.lowPrefix+"_"+cn.lowPostfix+"_get_instance_private(instance);");
			}
			
			for(Field field : thisType.fieldList.values()) {
				if (field.isPrivate) {
					out.print("	priv->");
				} else {
					out.print("	instance->");
				}
				out.print(field.name+" = ");
				
				if (field.type.isPointerType) {
					if (argumentNames.contains(field.name)) {
						out.println("L_SAFE_REF_PTR("+field.name+");");
					} else {
						out.println("NULL;");
					}
				} else {
					out.println("0;");
				}
			}

			if (content!=null) {
				out.println(content);
			}
			out.println("	return instance;");
		} else {
//			if (thisType.hasPrivateFields) {
//				out.println("	"+cmlName+"Private *priv = "+cn.lowPrefix+"_"+cn.lowPostfix+"_get_instance_private(self);");
//			}
			if (content!=null) {
				out.println(content);
			}
		}
		
		out.println(indent+"}");
		out.println();
	}


	public void writeH(Type thisType, PrintWriter out) {
		String indent = "";

		if (returnType!=null) {
			out.print(indent+returnType.fullTypePtr()+methodName+"(");
		} else {
			out.print(indent+"void "+methodName+"(");
		}

		boolean needComma = false;
		if (!isConstructor) {
			out.print(thisType.className.fullTypePtr()+"self");
			needComma = true;
		}
		for(Argument argument : arguments) {
			if (!needComma) {
				needComma = true;
			} else {
				out.print(", ");
				
			}
			out.print(argument.type.fullTypePtr()+argument.label);
		}
		out.println(");");
	}


}
