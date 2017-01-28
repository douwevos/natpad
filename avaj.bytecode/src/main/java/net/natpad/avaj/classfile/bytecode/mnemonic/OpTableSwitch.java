package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;


public class OpTableSwitch extends AbstractMnemonic {

	
	public final int defaultByte;
	public final int lowByte;
	public final int highByte;
	private final int[] table;
	
	public OpTableSwitch(int offset, int cnt, int defaultByte, int lowByte, int highByte, int[] table) {
		super(Operation.OP_TABLESWITCH, offset, cnt);
		this.defaultByte = defaultByte;
		this.lowByte = lowByte;
		this.highByte = highByte;
		this.table = table;
	}
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		fillSpaces(buf);
		buf.append(lowByte).append(", ").append(highByte).append(", ").append(defaultByte).append(", [");
		for(int idx=0; idx<table.length; idx++) {
			if (idx!=0) {
				buf.append(",");
			}
			String adr = "0000"+Integer.toHexString(table[idx]);
			buf.append(adr).substring(adr.length()-4);
		}
		buf.append("]");
		return buf.toString();
	}
}
