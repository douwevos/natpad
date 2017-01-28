package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Type;

public class OpStoreFastIndex extends AbstractMnemonic {

	
	public final Type storeType;
	public final int index;
	
	public OpStoreFastIndex(Operation operation, int offset, int cnt) {
		super(operation, offset, cnt);
		switch(operation) {
			case OP_ISTORE_0 : storeType = Type.INT; 	index=0; break;
			case OP_ISTORE_1 : storeType = Type.INT; 	index=1; break;
			case OP_ISTORE_2 : storeType = Type.INT; 	index=2; break;
			case OP_ISTORE_3 : storeType = Type.INT; 	index=3; break;
			case OP_LSTORE_0 : storeType = Type.LONG;	index=0; break;
			case OP_LSTORE_1 : storeType = Type.LONG;	index=1; break;
			case OP_LSTORE_2 : storeType = Type.LONG;	index=2; break;
			case OP_LSTORE_3 : storeType = Type.LONG;	index=3; break;
			case OP_FSTORE_0 : storeType = Type.FLOAT;	index=0; break;
			case OP_FSTORE_1 : storeType = Type.FLOAT;	index=1; break;
			case OP_FSTORE_2 : storeType = Type.FLOAT;	index=2; break;
			case OP_FSTORE_3 : storeType = Type.FLOAT;	index=3; break;
			case OP_DSTORE_0 : storeType = Type.DOUBLE;	index=0; break;
			case OP_DSTORE_1 : storeType = Type.DOUBLE;	index=1; break;
			case OP_DSTORE_2 : storeType = Type.DOUBLE;	index=2; break;
			case OP_DSTORE_3 : storeType = Type.DOUBLE;	index=3; break;
			case OP_ASTORE_0 : storeType = Type.REFERENCE;	index=0; break;
			case OP_ASTORE_1 : storeType = Type.REFERENCE;	index=1; break;
			case OP_ASTORE_2 : storeType = Type.REFERENCE;	index=2; break;
			case OP_ASTORE_3 : storeType = Type.REFERENCE;	index=3; break;
			default : storeType = Type.INT; index=0; break;
		}
	}
	
	
	@Override
	public String toString() {
		return operation.text;
	}
}
