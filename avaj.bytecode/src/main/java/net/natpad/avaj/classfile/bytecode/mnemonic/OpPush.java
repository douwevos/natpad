package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;

public class OpPush extends AbstractMnemonic {

	public final boolean isShort;
	public final int value;
	
	public OpPush(int offset, int cnt, byte b) {
		super(Operation.OP_BIPUSH, offset, cnt);
		isShort = false;
		value = b;
	}

	public OpPush(int offset, int cnt, short b) {
		super(Operation.OP_SIPUSH, offset, cnt);
		isShort = true;
		value = b;
	}
	
	
	public int getValue() {
		return value;
	}
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		return fillSpaces(buf).append(value).toString();
	}
}
