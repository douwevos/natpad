package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Type;

public class OpStoreIndex extends AbstractMnemonic {

	public final Type storeType;
	public final int index;
	
	public OpStoreIndex(Operation operation, int offset, int cnt, int index) {
		super(operation, offset, cnt);
		switch(operation) {
			case OP_ISTORE : storeType = Type.INT; break;
			case OP_LSTORE : storeType = Type.LONG; break;
			case OP_FSTORE : storeType = Type.FLOAT; break;
			case OP_DSTORE : storeType = Type.DOUBLE; break;
			case OP_ASTORE : storeType = Type.REFERENCE; break;
			default : storeType = Type.INT; break;
		}
		this.index = index;
	}
	

	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		switch(storeType) {
			case INT : buf.append('i'); break;
			case LONG : buf.append('l'); break;
			case FLOAT : buf.append('f'); break;
			case DOUBLE : buf.append('d'); break;
			case REFERENCE : buf.append('a'); break;
		}
		buf.append("store");
		fillSpaces(buf).append(index);
		return buf.toString();
	}
}
