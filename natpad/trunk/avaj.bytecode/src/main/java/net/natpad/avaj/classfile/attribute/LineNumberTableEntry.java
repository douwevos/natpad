package net.natpad.avaj.classfile.attribute;

import net.natpad.avaj.util.TextUtil;

public class LineNumberTableEntry {

	public final int startPc;
	public final int lineNumber;
	
	public LineNumberTableEntry(int startPc, int lineNumber) {
		this.startPc = startPc;
		this.lineNumber = lineNumber;
	}
	
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"[startPc="+TextUtil.hex(startPc, 4)+", lineNumber="+lineNumber+"]";
	}
	
}
