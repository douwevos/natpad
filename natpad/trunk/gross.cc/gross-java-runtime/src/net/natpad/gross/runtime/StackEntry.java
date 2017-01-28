package net.natpad.gross.runtime;

public class StackEntry {

	public final StackEntry parent;
	public final State state;
	public final Token lookahead;
	
	
	
	public StackEntry(StackEntry parent, State state, Token lookahead) {
		this.parent = parent;
		this.state = state;
		this.lookahead = lookahead;
	}
	
}
