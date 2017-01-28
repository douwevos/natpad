package net.natpad.avaj.ast;

import net.natpad.avaj.util.TextUtil;

public class Argument {

	public final DeclarationType declarationType;
	public final String name;
	public final Modifiers modifiers = new Modifiers();
	
	public Argument(DeclarationType declarationType, String name) {
		this.declarationType = declarationType;
		this.name = name;
	}
	

	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"["+name+", "+declarationType+"]";
	}
	
	
}
