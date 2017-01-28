package net.natpad.sht.model;

public class TemplateArgument {

	public final String id;
	Value defaultValue;
	
	public TemplateArgument(String id) {
		this.id = id;
		defaultValue = null;
	}
	
	public void setDefault(Value def) {
		defaultValue = def;
	}

	public Value getDefaultValue() {
		return defaultValue;
	}

	public String getRefId() {
		return id;
	}
	
}
