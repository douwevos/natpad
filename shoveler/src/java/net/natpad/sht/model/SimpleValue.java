package net.natpad.sht.model;

public class SimpleValue extends Value {

	
	public final String value;
	
	public SimpleValue(String value) {
		this.value = value;
	}
	
	
	
	public String getValue() {
		return value;
	}

	
	@Override
	public String toString() {
		return "'"+value+"'";
	}
	
}
