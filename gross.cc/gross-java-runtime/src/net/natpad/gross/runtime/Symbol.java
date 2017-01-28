package net.natpad.gross.runtime;

public abstract class Symbol {

	public final String name;
	public final Integer id;
	
	public Symbol(String name, Integer id) {
		this.name = name;
		this.id = id;
	}
	
	
	@Override
	public String toString() {
		return name;
	}
}
