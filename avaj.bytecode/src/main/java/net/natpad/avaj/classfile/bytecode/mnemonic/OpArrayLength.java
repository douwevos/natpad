package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;


public class OpArrayLength extends AbstractMnemonic {

	
	public OpArrayLength(int offset, int cnt) {
		super(Operation.OP_ARRAYLENGTH, offset, cnt);
	}
	
	
	@Override
	public String toString() {
		return "arraylength";
	}
}
