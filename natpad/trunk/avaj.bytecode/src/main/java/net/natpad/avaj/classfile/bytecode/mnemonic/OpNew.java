package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;


public class OpNew extends AbstractMnemonic {

	public final int index;
	
	public OpNew(int offset, int cnt, int index) {
		super(Operation.OP_NEW, offset, cnt);
		this.index = index;
	}

	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		fillSpaces(buf).append(index);
		return buf.toString();
	}
	
}
