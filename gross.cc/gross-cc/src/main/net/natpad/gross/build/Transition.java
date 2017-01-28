package net.natpad.gross.build;

public interface Transition {

	boolean isSame(Transition transition);

	void informMerge(LrarState destState, LrarState mergedState);

}
