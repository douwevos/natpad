package net.natpad.gross.runtime;

public class ReduceLayout {

	public final Symbol lhs;
	public final int rhsCount;
	public final int nullified[];
	public final int productionId;
	
	public ReduceLayout(Symbol lhs, int rhsCount, int nullified[], int productionId) {
		if (lhs==null) {
			throw new RuntimeException();
		}
		this.lhs = lhs;
		this.rhsCount = rhsCount;
		this.nullified = nullified;
		this.productionId = productionId;
	}

	public int getStackRhsCount() {
		if (nullified!=null) {
			return rhsCount-nullified.length;
		}
		return rhsCount;
	}

	public int calculateStackOffset(int offset) {
		int result = rhsCount-offset;
		if (nullified!=null) {
			for(int off : nullified) {
				if (off==offset) {
					return -1;
				} else if (off>offset) {
					result--;
				}
			}
		}
		return result;
	}
	
}
