package net.natpad.sht.model;

import java.io.PrintStream;

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

	
	@Override
	public void dump(PrintStream out, String indent) {
		out.println(indent+"'"+value+"'");
	}

}
