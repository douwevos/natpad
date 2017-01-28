package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Type;

public class OpReturn extends AbstractMnemonic {

	
	public final Type returnType;
	
	public OpReturn(Operation operation, int offset, int cnt) {
		super(operation, offset, cnt);
		switch(operation) {
			case OP_IRETURN : returnType = Type.INT; break;
			case OP_LRETURN : returnType = Type.LONG; break;
			case OP_FRETURN : returnType = Type.FLOAT; break;
			case OP_DRETURN : returnType = Type.DOUBLE; break;
			case OP_ARETURN : returnType = Type.REFERENCE; break;
			case OP_RETURN : returnType = null; break;
			default : returnType = Type.REFERENCE; break;
		}
	}
	
	
	@Override
	public String toString() {
		return operation.text;
	}
}
