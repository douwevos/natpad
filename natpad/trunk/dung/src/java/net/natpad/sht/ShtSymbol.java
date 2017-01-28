package net.natpad.sht;

import net.natpad.sht.generated.runtime.LrSymbol;

public class ShtSymbol extends LrSymbol {

	public final int leftRow;
	public final int rightRow;
	
	public ShtSymbol(int id, int left, int leftRow, int right, int rightRow, Object val) {
		super(id, left, right, val);
		this.leftRow = leftRow;
		this.rightRow = rightRow;
	}

	@Override
	public String toString() {
		return "ShtSymbol [leftRow=" + leftRow + ", rightRow=" + rightRow + ", sym=" + sym + ", parse_state="
				+ parse_state +  ", left=" + left + ", right=" + right
				+ ", value=" + value + "]";
	}

	
	
	
}
