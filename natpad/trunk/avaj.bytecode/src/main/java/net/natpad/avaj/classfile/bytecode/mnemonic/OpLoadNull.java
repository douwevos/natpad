package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;


public class OpLoadNull extends AbstractMnemonic {

	
	public OpLoadNull(int offset) {
		super(Operation.OP_ACONST_NULL, offset, 1);
	}
	

	@Override
	public String toString() {
		return operation.text;
	}
}
