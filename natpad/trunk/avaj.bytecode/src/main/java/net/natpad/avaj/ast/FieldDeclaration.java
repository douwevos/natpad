package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;


public class FieldDeclaration {

	public final DeclarationType fieldDescriptor;
	public final Identifier variableName;
	public final Modifiers modifiers;
	public final IExpression value;
	
	public FieldDeclaration(Modifiers modifiers, DeclarationType fieldDescriptor, Identifier variableName, IExpression value) {
		this.fieldDescriptor = fieldDescriptor;
		this.variableName = variableName;
		this.modifiers = modifiers;
		this.value = value;
	}
	
	public Identifier getVariable() {
		return variableName;
	}
	
	public Modifiers getModifiers() {
		return modifiers;
	}
	
	public DeclarationType getFieldDescriptor() {
		return fieldDescriptor;
	}
	
	public IExpression getValue() {
		return value;
	}
	
	
	
	public void write(Writer out) {
		if (modifiers!=null) {
			for(Modifier modifier : modifiers) {
				out.print(modifier.text);
				out.print(" ");
			}
		}
		fieldDescriptor.write(out);
		out.print(" ");
		variableName.write(out);
		if (value!=null) {
			out.print(" = ");
			value.write(out);
		}
		
	}
	
	@Override
	public String toString() {
		return fieldDescriptor+" "+variableName;
	}
	
	
}
