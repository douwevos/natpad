package net.natpad.gross.build;

public class AcceptTransition implements Transition {

	public AcceptTransition() {
	}
	
	
	@Override
	public void informMerge(LrarState destState, LrarState mergedState) {
	}
	
	@Override
	public boolean isSame(Transition transition) {
		return transition instanceof AcceptTransition;
	}
	
	
	@Override
	public String toString() {
		return "ACCEPT";
	}
	
}
