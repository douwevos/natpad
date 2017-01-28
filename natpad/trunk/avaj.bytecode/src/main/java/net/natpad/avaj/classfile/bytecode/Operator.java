package net.natpad.avaj.classfile.bytecode;

public enum Operator {

	POSTFIX_INC("++"),
	POSTFIX_DEC("--"),
	PREFIX_INC("++"),
	PREFIX_DEC("--"),
	UNARY_MINUS("-"),
	UNARY_INVERT("~"),
	UNARY_NOT("!"),
	MULTIPLY("*"),
	DIVIDE("/"),
	ADD("+"),
	SUB("-"),
	REMAINDER("%"),
	SHIFT_LEFT("<<"),
	SHIFT_RIGHT(">>"),
	UNSIGNED_SHIFT_RIGHT(">>>"),
	LESS_THAN("<"),
	GREATER_THAN(">"),
	LESS_OR_EQUAL("<="),
	GREATER_OR_EQUAL(">="),
	INSTANCE_OF("instanceof"),
	EQUAL("=="),
	NOT_EQUAL("!="),
	BITWISE_AND("&"),
	BITWISE_XOR("^"),
	BITWISE_OR("|"),
	LOGICAL_AND("&&"),
	LOGICAL_OR("||")
	;
	
	
	String text;
	
	private Operator(String text) {
		this.text = text;
	}

	public String getText() {
		return text;
	}
	
	@Override
	public String toString() {
		return name()+"'"+text+"'";
	}
	
}
