package net.natpad.avaj.vm.type;

import net.natpad.avaj.ast.DeclarationType;

public class ValueTypeDefinition {

	public final DeclarationType typeDefinition;
	public final ValueTypeDefinitionKind kind;
	public boolean isIncluded = true;
	
	public ValueTypeDefinition(DeclarationType typeDefinition, ValueTypeDefinitionKind kind) {
		this.typeDefinition = typeDefinition;
		this.kind = kind;
	}
	
	
	protected ValueTypeDefinition cloneDefinition() {
		ValueTypeDefinition result = new ValueTypeDefinition(typeDefinition, kind);
		result.isIncluded = isIncluded;
		return result;
	}
	
	@Override
	public int hashCode() {
		return typeDefinition.hashCode()+7*kind.hashCode();
	}
	
	
	@Override
	public boolean equals(Object other) {
		if (other == this) {
			return true;
		}
		if (other instanceof ValueTypeDefinition) {
			ValueTypeDefinition that = (ValueTypeDefinition) other;
			return kind==that.kind && typeDefinition.equals(that.typeDefinition);
		}
		return false;
	}
}
