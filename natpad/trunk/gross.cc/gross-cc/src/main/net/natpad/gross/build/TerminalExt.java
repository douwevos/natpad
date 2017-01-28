package net.natpad.gross.build;

import net.natpad.gross.runtime.Terminal;

public class TerminalExt extends Terminal implements SymbolWithType {

	String returnType;
	
	
	public static final TerminalExt EMPTY = new TerminalExt("∅", -1);
	
	public TerminalExt(String name, Integer id) {
		super(name, id);
	}
	
	@Override
	public String getType() {
		return returnType;
	}
	
	@Override
	public void setType(String type) {
		returnType = type;
	}
}
