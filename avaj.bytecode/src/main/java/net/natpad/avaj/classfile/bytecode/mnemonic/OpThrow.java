package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;


public class OpThrow extends AbstractMnemonic {

	
	public OpThrow(int offset, int cnt) {
		super(Operation.OP_ATHROW, offset, cnt);
	}
	

	@Override
	public String toString() {
		return operation.text;
	}
}
