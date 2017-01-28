package net.natpad.sht.model;

import java.io.PrintStream;

public class Reference {

	Value value;
	public final String refId;
	
	
	public Reference(String refId) {
		this.refId = refId;
	}
	
	
	public void setValue(Value expr) {
		value = expr;
	}

	public String getId() {
		return refId;
	}

	
	public Value getValue() {
		return value;
	}
	
	@Override
	public String toString() {
		if (value!=null) {
			String exprtxt = value.toString();
			if (exprtxt!=null && exprtxt.startsWith("{")) {
				return refId + " "+value;
			} else {
				return refId + " "+value+";";
			}
		}
		return refId + " nil;";
	}


	public void dump(PrintStream out, String indent) {
		out.println(indent+"Reference["+refId+"]");
		if (value==null) {
			out.println(indent+"   nil");
		} else {
			value.dump(out, indent+"   ");
		}
		
	}


	
}
