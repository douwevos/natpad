package net.natpad.gross.runtime;

public class ShiftTransition implements Transition {

	public State forwardState;
	
	public ShiftTransition(State forwardState) {
		this.forwardState = forwardState;
	}

	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder("shift:");
		buf.append(forwardState);
		return buf.toString();
	}
	
}
