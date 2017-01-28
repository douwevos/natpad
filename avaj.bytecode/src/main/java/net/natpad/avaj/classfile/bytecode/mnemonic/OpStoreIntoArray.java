package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Type;


public class OpStoreIntoArray extends AbstractMnemonic {

	public final Type storeType;
	
	public OpStoreIntoArray(Operation operation, int offset, int cnt) {
		super(operation, offset, cnt);
		switch(operation) {
			case OP_IASTORE : storeType = Type.INT; break;
			case OP_LASTORE : storeType = Type.LONG; break;
			case OP_FASTORE : storeType = Type.FLOAT; break;
			case OP_DASTORE : storeType = Type.DOUBLE; break;
			case OP_AASTORE : storeType = Type.REFERENCE; break;
			case OP_BASTORE : storeType = Type.BYTE; break;
			case OP_CASTORE : storeType = Type.CHAR; break;
			case OP_SASTORE : storeType = Type.SHORT; break;
			default : storeType = Type.INT; break;
		}
	}

	
	@Override
	public String toString() {
		return operation.text;
	}
	
}
