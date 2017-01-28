package net.natpad.gross.export.glibc;

import java.util.HashMap;

public class InterfaceImplement {

	public final ClassName className;

	public String prefixContent;
	
	public final HashMap<String, Method> methodMappings = new HashMap<String, Method>();
	
	public String initText;
	
	public InterfaceImplement(ClassName className) {
		this.className = className;
	}
	
	
	public void addMethod(String interfaceMethoName, Method method) {
		methodMappings.put(interfaceMethoName, method);
	}
	
}
