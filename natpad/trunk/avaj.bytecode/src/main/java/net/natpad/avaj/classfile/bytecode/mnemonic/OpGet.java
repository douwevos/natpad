package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;


public class OpGet extends AbstractMnemonic {


	public final int poolIndex;

	
	public OpGet(Operation operation, int offset, int cnt, int poolIndex) {
		super(operation, offset, cnt);
		this.poolIndex = poolIndex;
	}


	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		fillSpaces(buf).append(poolIndex);
		return buf.toString();
	}
}
