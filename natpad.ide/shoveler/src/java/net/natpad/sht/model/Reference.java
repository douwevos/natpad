package net.natpad.sht.model;

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


	
}
