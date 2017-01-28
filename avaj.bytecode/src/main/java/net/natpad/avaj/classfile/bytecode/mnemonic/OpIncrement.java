package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;

public class OpIncrement extends AbstractMnemonic {

	public final int index, constant;
	
	
	public OpIncrement(int offset, int cnt, int index, int constant) {
		super(Operation.OP_IINC, offset, cnt);
		this.index = index;
		this.constant = constant;
	}


	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		fillSpaces(buf).append(index).append(',').append(constant);
		return buf.toString();
	}
}
