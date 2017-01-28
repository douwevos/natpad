package net.natpad.gross.runtime;

public class Location {

	public final int beginColumn;
	public final long beginRow;
	public final int endColumn;
	public final long endRow;
	
	public Location(int beginColumn, long beginRow, int endColumn, long endRow) {
		this.beginColumn = beginColumn;
		this.beginRow = beginRow;
		this.endColumn = endColumn;
		this.endRow = endRow;
	}
	

	public Location setEnd(int lastCol, long lastRow) {
		return new Location(beginColumn, beginRow, lastCol, lastRow);
	}

	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append("begin:").append(beginColumn).append("/").append(beginRow);
		buf.append(", end:").append(endColumn).append("/").append(endRow);
		return buf.toString();
	}


	
}
