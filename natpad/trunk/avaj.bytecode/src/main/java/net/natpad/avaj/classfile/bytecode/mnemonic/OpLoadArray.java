package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Type;


public class OpLoadArray extends AbstractMnemonic {

	
	public final Type loadType;
	
	public OpLoadArray(Operation operation, int offset, int cnt) {
		super(operation, offset, cnt);
		switch(operation) {
			case OP_IALOAD : loadType = Type.INT; break;
			case OP_LALOAD : loadType = Type.LONG; break;
			case OP_FALOAD : loadType = Type.FLOAT; break;
			case OP_DALOAD : loadType = Type.DOUBLE; break;
			case OP_AALOAD : loadType = Type.REFERENCE; break;
			case OP_BALOAD : loadType = Type.BYTE; break;
			case OP_CALOAD : loadType = Type.CHAR; break;
			case OP_SALOAD : loadType = Type.SHORT; break;
			default : loadType = Type.INT; break;
		}
	}
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		return buf.toString();
	}
}
