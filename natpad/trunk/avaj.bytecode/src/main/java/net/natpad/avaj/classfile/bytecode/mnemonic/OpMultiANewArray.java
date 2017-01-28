package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;

public class OpMultiANewArray extends AbstractMnemonic {
	

	public final int poolIndex;
	public final int dimensions;
	
	
	public OpMultiANewArray(int offset, int cnt, int poolIndex, int dimensions) {
		super(Operation.OP_MULTIANEWARRAY, offset, cnt);
		this.poolIndex = poolIndex;
		this.dimensions = dimensions;
	}

	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		fillSpaces(buf).append(poolIndex).append(", ").append(dimensions);
		return buf.toString();
	}

}
