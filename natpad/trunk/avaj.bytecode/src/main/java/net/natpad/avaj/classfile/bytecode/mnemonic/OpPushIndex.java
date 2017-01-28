package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;


public class OpPushIndex extends AbstractMnemonic {

	
	public final boolean isWide;
	public final int index;
	public final boolean isLongOrDouble;
	
	public OpPushIndex(Operation operation, int offset, int cnt, boolean isWide, int index, boolean isLongOrDouble) {
		super(operation, offset, cnt);
		this.isWide = isWide;
		this.index = index;
		this.isLongOrDouble = isLongOrDouble;
	}
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		fillSpaces(buf).append(index);
		return buf.toString();
	}
	
}
