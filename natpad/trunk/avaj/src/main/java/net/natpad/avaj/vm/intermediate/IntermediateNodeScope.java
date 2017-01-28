package net.natpad.avaj.vm.intermediate;


import net.natpad.avaj.util.TextUtil;

public class IntermediateNodeScope {

	public static final int INDEX_NONE = -1;
	public static final int INDEX_EXIT = -2;
	
	
	int exitIndex, jumpIndex;
	
	public IntermediateNodeScope(int exitIndex, int jumpIndex) {
		this.exitIndex = exitIndex;
		this.jumpIndex = jumpIndex;
	}
	
	public int getExitIndex() {
		return exitIndex;
	}
	
	public int getJumpIndex() {
		return jumpIndex;
	}
	
	public int getOutIndex() {
		int lcJumpIndex = getJumpIndex();
		int lcExitIndex = getExitIndex();
		if (lcJumpIndex==INDEX_NONE && lcExitIndex!=INDEX_NONE) {
			return lcExitIndex;
		}
		if (lcJumpIndex!=INDEX_NONE && lcExitIndex==INDEX_NONE) {
			return lcJumpIndex;
		}
		
		return INDEX_NONE;
	}

	public void setExitIndex(int newExitIndex) {
		this.exitIndex = newExitIndex;
	}

	public void setJumpIndex(int jumpIndex2) {
		this.jumpIndex = jumpIndex2;
	}
	
	
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder(TextUtil.getName(getClass())).append("[");
		boolean mainFirst = true;
		if (exitIndex!=INDEX_NONE) {
			buf.append("exit-index=").append(toString(exitIndex));
			mainFirst=false;
		}
		mainFirst = appendIndex(buf, "jump-index", jumpIndex, mainFirst);
//		mainFirst = appendIndex(buf, "entry-index", entryIndex, mainFirst);

		return buf.append("]").toString();
	}

	private boolean appendIndex(StringBuilder buf, String fieldName, int fieldValue, boolean mainFirst) {
		if (fieldValue!=INDEX_NONE) {
			if (mainFirst) {
				buf.append(", ");
			} else {
				mainFirst = false;
			}
			buf.append(", ").append(fieldName).append("=").append(toString(fieldValue));
		}
		return mainFirst;
	}


	
	public String toString(int index) {
		switch(index) {
			case INDEX_EXIT : {
				return "exit";
			}
			case INDEX_NONE : {
				return "none";
			}
		}
		return ""+index;
	}


}
