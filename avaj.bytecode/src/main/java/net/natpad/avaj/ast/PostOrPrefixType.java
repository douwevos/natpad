package net.natpad.avaj.ast;


public enum PostOrPrefixType {

	INCREMENT("++"),
	DECREASE("--");
	
	private final String text;
	
	
	PostOrPrefixType(String text) {
		this.text = text;
	}
	
	public String getText() {
		return text;
	}
	
}
