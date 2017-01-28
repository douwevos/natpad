package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;


public class OpPut extends AbstractMnemonic {

	public enum PutType { FIELD, STATIC}

	public final PutType putType;
	public final int poolIndex;
	
	public OpPut(Operation operation, int offset, int cnt, int poolIndex) {
		super(operation, offset, cnt);
		switch(operation) {
			case OP_PUTSTATIC : putType = PutType.STATIC; break;
			case OP_PUTFIELD : putType = PutType.FIELD; break;
			default : putType = PutType.FIELD; break;
		}
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
