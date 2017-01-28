package net.natpad.gross.runtime;

public class ReduceTransition implements Transition {

	public final ReduceLayout reduceLayout;
	public final int rhsCount;
	public final Symbol lhs;
	public final int actionId;

	
	public ReduceTransition(ReduceLayout reduceLayout, int actionId) {
		this.reduceLayout = reduceLayout;
		this.rhsCount = -1;
		this.lhs = null;
		this.actionId = actionId;
	}

	
	public ReduceTransition(int rhsCount, Symbol lhs, int actionId) {
		this.reduceLayout = null;
		this.rhsCount = rhsCount;
		this.lhs = lhs;
		this.actionId = actionId;
	}
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder("reduce:");
		buf.append("lhs=").append(lhs);
		buf.append(",rhsCount=").append(rhsCount);
		buf.append(",actionId=").append(actionId);
		return buf.toString();
	}


	public int getRhsCount() {
		if (reduceLayout!=null) {
			return reduceLayout.getStackRhsCount();
		}
		return rhsCount;
	}

}
