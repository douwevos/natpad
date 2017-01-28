package net.natpad.monkey.model;

public class NodeRecursiveCounter {

	public final int localCount;
	public final int foreignCount;
	
	public NodeRecursiveCounter() {
		this.localCount = 0;
		this.foreignCount = 0;
	}
	
	public NodeRecursiveCounter(int localCount, int foreignCount) {
		this.localCount = localCount;
		this.foreignCount = foreignCount;
	}
	
	public NodeRecursiveCounter setLocalCount(int localCount) {
		return new NodeRecursiveCounter(localCount, foreignCount);
	}

	public NodeRecursiveCounter setForeignCount(int foreignCount) {
		return new NodeRecursiveCounter(localCount, foreignCount);
	}

	
	
	@Override
	public boolean equals(Object obj) {
		if (obj==this) {
			return true;
		}
		if (obj instanceof NodeRecursiveCounter) {
			NodeRecursiveCounter other = (NodeRecursiveCounter) obj;
			return localCount==other.localCount && foreignCount==other.foreignCount;
		}
		return false;
	}
	
}
