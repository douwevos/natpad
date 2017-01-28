package net.natpad.gross.build;

public class ShiftTransition implements Transition {

	public final String origin;
	public LrarState shiftToState;

	public ShiftTransition(String origin, LrarState shiftToState) {
		this.origin = origin;
		this.shiftToState = shiftToState;
	}


	@Override
	public boolean isSame(Transition transition) {
		if (transition instanceof ShiftTransition) {
			ShiftTransition other = (ShiftTransition) transition;
			if (other.shiftToState==shiftToState) {
				return true;
			}
		}
		return false;
	}
	
	@Override
	public void informMerge(LrarState destState, LrarState mergedState) {
		if (shiftToState==mergedState) {
			shiftToState = destState;
		}
	}
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder("shift[").append(origin).append("]: ");
		buf.append(shiftToState.id);
		return buf.toString();
	}
}
