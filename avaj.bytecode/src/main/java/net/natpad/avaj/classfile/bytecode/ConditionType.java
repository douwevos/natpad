package net.natpad.avaj.classfile.bytecode;


public enum ConditionType {

	EQUAL("=="),
	NOT_EQUAL("!="),
	LESS_THEN("<"),
	LESS_OR_EQUAL("<="),
	GREATER_THEN(">"),
	GREATER_OR_EQUAL(">=");
	
	private final String text;
	
	
	ConditionType(String text) {
		this.text = text;
	}
	
	public String getText() {
		return text;
	}
	
	public ConditionType opposite() {
		switch(this) {
			case EQUAL : return NOT_EQUAL;
			case NOT_EQUAL : return EQUAL;
			case LESS_OR_EQUAL : return GREATER_THEN;
			case LESS_THEN : return GREATER_OR_EQUAL;
			case GREATER_THEN : return LESS_OR_EQUAL;
			case GREATER_OR_EQUAL : return LESS_THEN;
		}
		return NOT_EQUAL;
	}
	

	
}
