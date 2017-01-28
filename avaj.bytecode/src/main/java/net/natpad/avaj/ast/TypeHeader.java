package net.natpad.avaj.ast;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import net.natpad.avaj.ast.formater.Writer;

public class TypeHeader {

	public final Name className;
	public final Modifiers modifiers;
	private final List<Method> methodList = new ArrayList<Method>();
	private final List<FieldDeclaration> fieldList = new ArrayList<FieldDeclaration>();
	private final Name[] interfaces;
	
	public Name superType;

	public TypeHeader(Name typeReference, Modifiers modifiers, Name superType, Name[] interfaces) {
		this.className = typeReference;
		this.modifiers = modifiers;
		this.superType = superType;
		this.interfaces = interfaces;
	}

	public void write(Writer out) {
		if (modifiers!=null) {
			for(Modifier modifier : modifiers) {
				out.print(modifier.text);
				out.print(" ");
			}
		}
		out.print("class ");
		out.print(className.getShortTypeName());
		if (superType!=null) {
			String fqn = superType.createFQN();
			if (!"java.lang.Object".equals(fqn)) {
				out.print(" extends ");
				out.print(fqn);
			}
		}
		if (interfaces!=null && interfaces.length>0) {
			out.print(" implements ");
			boolean isFirst = true;
			for(Name iface : interfaces) {
				if (isFirst) {
					isFirst = false;
				} else {
					out.print(", ");
				}
				out.print(iface.createFQN());
			}
		}
	}
	
	
	
	public void add(Method method) {
		methodList.add(method);
	}
	
	
	public void add(FieldDeclaration field) {
		fieldList.add(field);
	}
	
	public List<Method> getMethods() {
		return Collections.unmodifiableList(methodList);
	}
	
	public List<FieldDeclaration> getFields() {
		return Collections.unmodifiableList(fieldList);
	}

	public List<Name> getInterfaces() {
		ArrayList<Name> result = new ArrayList<Name>();
		result.addAll(Arrays.asList(interfaces));
		return result;
	}
	
	
	public Name getSuperType() {
		return superType;
	}
	
}
