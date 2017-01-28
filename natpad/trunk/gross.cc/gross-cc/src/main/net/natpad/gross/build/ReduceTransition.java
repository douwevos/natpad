package net.natpad.gross.build;

public class ReduceTransition implements Transition {

	public final String origin;
	public final DotState reduceRule;
	
	public ReduceTransition(String origin, DotState reduceRule) {
		this.origin = origin;
		this.reduceRule = reduceRule;
	}

	@Override
	public boolean isSame(Transition transition) {
		if (transition instanceof ReduceTransition) {
			ReduceTransition other = (ReduceTransition) transition;
			if (reduceRule.production==other.reduceRule.production) {
				return true;
			}
		}
		return false;
	}

	@Override
	public void informMerge(LrarState destState, LrarState mergedState) {
	}	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder("reduce["+origin+"]:");
		buf.append(reduceRule);
		return buf.toString();
	}
	
}
